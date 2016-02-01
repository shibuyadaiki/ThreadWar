#include "ThreadAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../../Math/Quaternion.h"
#include "../../ThreadWeb.h"

const float FRONTROTATESPEED = 60.0f;

ThreadAction::ThreadAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :
PlayerAction(world, player_, padNum_, cID_, ACTION_ID::THREAD_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
}
ThreadAction::~ThreadAction(){

}
bool ThreadAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	cameraMove = true;
	playerControlFlag.firstFrameFlag = true;
	playerControlFlag.startNorChangeFlag = true;
	playerControlFlag.secondFrameFlag = false;
	moveVecUp = player._Get()->ReturnPlayerParameter()->nor;
	moveVecPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	leftRightID = ANIM_ID::NEPHILA_THREADLEFT_ANIM;
	frontBackID = ANIM_ID::NEPHILA_THREADFRONT_ANIM;

	Vector3 cameraVec = RCVector3::normalize(RCMatrix4::getFront(player._Get()->ReturnPlayerParameter()->angleMat));
	Vector3 ghostFront = RCVector3::normalize(player._Get()->ReturnThread()._Get()->GetThreadParameter().startNormal);
	animFront = true;
	float ang = Math::acos(RCVector3::dot(cameraVec, ghostFront));
	Vector3 newFront = ghostFront;
	for (int i = 1; i < 4; i++){
		Vector3 nor = player._Get()->ReturnPlayerParameter()->nor;
		if (ang > Math::acos(RCVector3::dot(cameraVec, RCVector3::normalize(ghostFront * RCQuaternion::rotate(nor, 90 * i))))){
			newFront = RCVector3::normalize(ghostFront*RCQuaternion::rotate(nor, 90 * i));
			ang = Math::acos(RCVector3::dot(cameraVec, RCVector3::normalize(ghostFront * RCQuaternion::rotate(nor, 90 * i))));
			if (i == 1 || i == 3)animFront = false;
			else animFront = true;
		}
	}
	player._Get()->SetAnimation(
		(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		THREADANIMSPEED, true, true, animFront == true ? 1 : 0, WALKANIMBLEND);

	startRotate = 0;

	frontRotate = 0;
	hitGround = false;
	groundFirstFrame = true;
	return true;
}
void ThreadAction::Update(float frameTime){
	change = false;
	if (player._Get()->ReturnThread().lock() == nullptr){
		ChangeAction(ACTION_ID::HOVER_ACTION);
	}
	threadWebVec.clear();
	threadVec.clear();

	//	if (startNorChangeFlag){
	//		//徐々にupをvector3(0,1,0)に近づける
	//		Vector3 nor = RCVector3::lerp((moveVecUp), startNor, startRotate);
	//		player._Get()->SetNor(nor);
	//		startRotate += STARTNORCHANGESPEED * frameTime;
	//		if (startRotate > 1.0f || moveVecUp == startNor)startNorChangeFlag = false;
	//	}
	if (playerControlFlag.startNorChangeFlag && !playerControlFlag.firstFrameFlag){
		//徐々にupをvector3(0,1,0)に近づける
		Vector3 nor = RCVector3::lerp((moveVecUp), startNor, startRotate);
		Vector3 pos = RCVector3::lerp((moveVecPos), startPos, startRotate);
		player._Get()->SetNor(nor);
		player._Get()->SetPos(pos);
		startRotate += STARTNORCHANGESPEED * frameTime;
		if (startRotate > 1.0f){
			playerControlFlag.startNorChangeFlag = false;
		}
	}
	//糸との判定
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadID, COL_ID::SPHERE_LINE_COLL);
	//蜘蛛の巣との判定
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadWebID, COL_ID::TRIANGLE_SPHERE_COLL);
	//蜘蛛の巣との判定
	//world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::SPHERE_MODEL_COLL);
	Matrix4 angleMat = player._Get()->ReturnPlayerParameter()->angleMat;
	Vector3 moveVecFront = RCVector3::normalize(RCMatrix4::getFront(angleMat));
	Vector3 moveVecLeft = RCVector3::normalize(-RCMatrix4::getLeft(angleMat));
	Vector3 vec = player._Get()->ReturnPlayerParameter()->vec;

	Vector3 cameraVec = RCVector3::normalize(RCMatrix4::getFront(player._Get()->ReturnPlayerParameter()->angleMat));
	Vector3 ghostFront = RCVector3::normalize(player._Get()->ReturnThread()._Get()->GetThreadParameter().startNormal);
	bool beforeAnimFront = animFront;
	animFront = true;
	float ang = Math::acos(RCVector3::dot(cameraVec, ghostFront));
	Vector3 newFront = ghostFront;
	bool front = true;
	bool left = true;
	for (int i = 1; i < 4; i++){
		Vector3 nor = player._Get()->ReturnPlayerParameter()->nor;
		if (ang > Math::acos(RCVector3::dot(cameraVec, RCVector3::normalize(ghostFront * RCQuaternion::rotate(nor, 90 * i))))){
			newFront = RCVector3::normalize(ghostFront * RCQuaternion::rotate(nor,90 * i));
			ang = Math::acos(RCVector3::dot(cameraVec, RCVector3::normalize(ghostFront * RCQuaternion::rotate(nor, 90 * i))));
			if (i == 1 || i == 3){
				animFront = false;
				if (i == 3)left = false;
			}
			else {
				front = false;
				animFront = true;
			}
		}
	}
	if (beforeAnimFront != animFront)
		player._Get()->SetAnimation(
		(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		THREADANIMSPEED, true, true, animFront == true ? 1 : 0, WALKANIMBLEND);

	if (!(controlVec == vector3(0, 0, 0))){
		float rotateVec = abs(-vec.z) > abs(vec.x) ? -vec.z : vec.x;
		if (animFront){
			player._Get()->SetAnimTime(THREADANIMSPEED * min(abs(RCVector3::length(controlVec)), 10.0f) * rotateVec);
			if (front)
				frontRotate -= vec.x * FRONTROTATESPEED * frameTime;
			else
				frontRotate += vec.x * FRONTROTATESPEED * frameTime;
		}
		else{
			player._Get()->SetAnimTime(THREADANIMSPEED * min(abs(RCVector3::length(controlVec)), 10.0f) * rotateVec);
			if (!left)
				frontRotate -= vec.z * FRONTROTATESPEED * frameTime;
			else

				frontRotate += vec.z * FRONTROTATESPEED * frameTime;
		}

		player._Get()->SetAnimBlend(WALKANIMBLEND);
	}
	else {
		player._Get()->SetAnimTime(0);
		player._Get()->SetAnimBlend(WALKANIMBLEND);
	}
	if (!playerControlFlag.startNorChangeFlag && !change)
	{
		Vector3 moveVec = (moveVecFront * vec.z + moveVecLeft * vec.x) * THREADSPEED * frameTime;
		player._Get()->SetMoveVec(moveVec);
	}

	hitGround = false;
}
void ThreadAction::OnCollide(Actor& other, CollisionParameter colpara){
	if (colpara.id == COL_ID::SPHERE_LINE_COLL && !change){
		if (player._Get()->ReturnThread()._Get() == &other){
			//当たった地点へ移動
			player._Get()->SetPos(colpara.colPos);

			//上方向を計算
			Thread* thread_ = static_cast<Thread*>(&other);
			ThreadParameter tparam = thread_->GetThreadParameter();
			Vector3 front = tparam.endNormal;
			Vector3 left = front * RCMatrix4::rotateY(90);
			left = vector3(left.x, 0, left.z);

			if (playerControlFlag.firstFrameFlag){
				playerControlFlag.firstFrameFlag = false;
				playerControlFlag.secondFrameFlag = true;

				startNor = RCVector3::normalize(RCVector3::cross(front, left) * RCQuaternion::rotate(front, 0));
				moveVecUp = RCVector3::normalize(moveVecUp);
				float startAngle = Math::acos(RCVector3::dot(startNor, moveVecUp));
				int plusFlag = 10;
				float secondAngle = startAngle;
				int saveAngle = 0;
				for (int i = 0; i < 35; i++){
					startNor = RCVector3::normalize(RCVector3::cross(front, left) * RCQuaternion::rotate(front, plusFlag));
					//secondAngle = Math::acos(RCVector3::dot(startNor, moveVecUp));
					if (secondAngle > Math::acos(RCVector3::dot(startNor, moveVecUp))){
						secondAngle = Math::acos(RCVector3::dot(startNor, moveVecUp));
						saveAngle = i * 10 + 10;
					}

					plusFlag += 10;
				}

				startNor = RCVector3::normalize(RCVector3::cross(front, left) * RCQuaternion::rotate(front, saveAngle));

				//if (!startNorChangeFlag)
				//上方向をセット
			}
			//std::string s = "うんこ.png";
			//auto a = s.find_last_of(".");
			//s = s.substr(0, a);
			player._Get()->SetNor(RCVector3::normalize(startNor * RCQuaternion::rotate(front, frontRotate)));

			//ボタン入力で巻取りを開始
			if ((Device::GetInstance().GetInput()->MouseButtonDown(INPUTMOUSEBUTTON::RIGHT_BUTTON, true) ||
				Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R2, true)) &&
				player._Get()->ReturnPlayerNum() == 0){
				ChangeAction(ACTION_ID::REWIND_ACTION);
			}
			float radius = player._Get()->GetParameter().radius;
			Vector3 pos = colpara.colPos;
			if (playerControlFlag.firstFrameFlag){
				if (RCVector3::length(pos - tparam.startPosition) < RCVector3::length(pos - tparam.currentEndPosition))pos = tparam.startPosition;
				else pos = tparam.currentEndPosition;
			}
			CollisionParameter colpara_;
			if (tparam.startPosIsGroundTouch){
				colpara_ = IsCollideSphereToSphere(tparam.startPosition, pos, radius, radius);
				if (colpara_.colFlag)pos = tparam.startPosition;
				while (colpara_.colFlag)
				{
					pos += RCVector3::normalize(tparam.currentEndPosition - tparam.startPosition) * 0.1f;
					colpara_ = IsCollideSphereToSphere(tparam.startPosition, pos, radius, radius);
				}
			}
			colpara_ = IsCollideSphereToSphere(tparam.currentEndPosition, pos, radius, radius);
			if (colpara_.colFlag)pos = tparam.currentEndPosition;
			while (colpara_.colFlag)
			{
				pos += RCVector3::normalize(tparam.startPosition - tparam.currentEndPosition) * 0.1f;
				colpara_ = IsCollideSphereToSphere(tparam.currentEndPosition, pos, radius, radius);
			}

			player._Get()->SetPos(pos);
			startPos = pos;
			radius = player._Get()->GetParameter().radius + 0.1f;

			float size2 = 3.0f;
			Matrix4 matStage = RCMatrix4::matrix(
				vector3(size2, size2, size2),
				0.0f,
				0.0f,
				0.0f,
				vector3(-1, -2.0f, 0));

			if (((IsCollideSphereToSphere(tparam.startPosition, pos, radius, radius).colFlag && tparam.startPosIsGroundTouch) ||
				IsCollideSphereToSphere(tparam.currentEndPosition, pos, radius, radius).colFlag)){
				hitGround = true;
				if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
					Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R3, true)) &&
					!groundFirstFrame){
					ChangeAction(ACTION_ID::NORMAL_ACTION);
					if (RCVector3::length(tparam.startPosition - pos) < RCVector3::length(tparam.currentEndPosition - pos)){
						player._Get()->SetPos(tparam.startPosition);
						player._Get()->SetNor(ModelSphere(matStage, OCT_ID::STAGE_OCT, tparam.startPosition, 0.3f).colNormal);
					}
					else{
						player._Get()->SetPos(tparam.currentEndPosition);
						player._Get()->SetNor(ModelSphere(matStage, OCT_ID::STAGE_OCT, tparam.currentEndPosition, 0.3f).colNormal);
					}
				}
			}
			groundFirstFrame = false;

		}
		else if (other.GetParameter().id == friendThreadID&& !static_cast<Thread*>(&other)->IsShot()){
			threadVec.push_back(static_cast<Thread*>(&other)->shared_from_this());
		}
	}
	else if (colpara.id == COL_ID::TRIANGLE_SPHERE_COLL && other.GetParameter().id == friendThreadWebID){
		threadWebVec.push_back(static_cast<ThreadWeb*>(&other)->shared_from_this());
	}
	else if (colpara.id == COL_ID::SPHERE_MODEL_COLL){
		player._Get()->SetPos(colpara.colPos);
	}
}