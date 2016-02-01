#include "JumpAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../ThreadWeb.h"
#include "../../../Math/Quaternion.h"

JumpAction::JumpAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :PlayerAction(world, player_, padNum_, cID_, ACTION_ID::JUMP_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
}
JumpAction::~JumpAction(){

}
bool JumpAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	if (beforeAction == ACTION_ID::DANGLE_ACTION || beforeAction == ACTION_ID::REWIND_ACTION)player._Get()->ReturnThread()._Get()->SetIsDead(true);
	
	if ((player._Get()->ReturnPlayerParameter()->onGroundFlag || 
		player._Get()->ReturnPlayerParameter()->onThreadFlag) &&
		beforeAction != ACTION_ID::GROUND_CURL_ACTION && 
		beforeAction != ACTION_ID::JUMP_CURL_ACTION ||
		beforeAction == ACTION_ID::DANGLE_ACTION ||
		beforeAction == ACTION_ID::REWIND_ACTION ||
		beforeAction == ACTION_ID::THREAD_WEB_ACTION){
		cameraMove = false;
		jumpSpeed = JUMPSPEED;
		//上をセット
		moveVecUp = beforeUp;
		//慣性を保存
		inertiaVec = player._Get()->GetParameter().moveVec;

		startRotate = 0;
		float upAniTime = JUMPANIMSPEED * player._Get()->BindPow(true);
		float blendAnim = JUMPANIMBLEND * player._Get()->BindPow(true);
		player._Get()->SetAnimation(
			(ANIM_ID)(ANIM_ID::NEPHILA_JUMP_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
			(ANIM_ID)(ANIM_ID::NEPHILA_JUMP_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
			upAniTime, true, false, 0, blendAnim);

		playerControlFlag.startNorChangeFlag = true;
		playerControlFlag.firstFrameFlag = true;
		cameraMoveVecUp = Device::GetInstance().GetCamera(cID)->CameraParam()->Up;
		cameraNorChangeFlag = true;
		plusFlag = true;

		frontAngle = 0;
		leftAngle = 0;
		secondMoveVecUp = vector3(0, 1, 0);
		hover = false;
		return true;
	}
	return false;
}
void JumpAction::Update(float frameTime){
	//蜘蛛の巣との判定
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadWebID, COL_ID::TRIANGLE_SPHERE_COLL);
	//ジャンプ中の糸との判定
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadID, COL_ID::SPHERE_LINE_COLL);
	//あたり判定を丸に
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::SPHERE_MODEL_NATURAL_COLL);
	//加速度の計算
	jumpSpeed -= 25.0f * frameTime * player._Get()->BindPow(true);
	if (jumpSpeed <= 10.0f){
		if (!hover){
			hover = true;
			float upAniTime = HOVERANIMSPEED;
			float blendAnim = HOVERANIMBLEND * player._Get()->BindPow(true);
			player._Get()->SetAnimation(
				(ANIM_ID)(ANIM_ID::NEPHILA_WALKFRONT_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
				(ANIM_ID)(ANIM_ID::NEPHILA_WALKFRONT_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
				upAniTime, true, true, 0, blendAnim);
		}
		
	}
	if (jumpSpeed <= 0){
		//ChangeAction(ACTION_ID::HOVER_ACTION);
		jumpSpeed = 0.0f;
	}
	//上方向と慣性を足す（要調整）
	Vector3 moveVec = (moveVecUp)* jumpSpeed * frameTime + RCVector3::normalize(inertiaVec) * jumpSpeed / 8.0f * frameTime;

	Matrix4 angleMat = player._Get()->ReturnPlayerParameter()->angleMat;
	Vector3 moveVecFront = RCVector3::normalize(RCMatrix4::getFront(angleMat));
	Vector3 moveVecLeft = RCVector3::normalize(-RCMatrix4::getLeft(angleMat));
	Vector3 vec = player._Get()->ReturnPlayerParameter()->vec;

	moveVec += (moveVecFront * vec.z + moveVecLeft * vec.x) * SPEED * frameTime;
	//if (!startNorChangeFlag)
	player._Get()->SetMoveVec(moveVec);

	cameraMove = true;
	if (cameraNorChangeFlag){
		Vector3 nor = RCVector3::lerp(moveVecUp, cameraMoveVecUp, frontAngle);
		frontAngle += STARTNORCHANGESPEED * 2.0f * frameTime;
		player._Get()->SetNor(nor);
		cameraMove = false;
		playerControlFlag.firstFrameFlag = false;
		if (frontAngle > 1.0f){
			cameraNorChangeFlag = false;
			frontAngle = 0;
			playerControlFlag.firstFrameFlag = true;
		}
	}
	else{
		if (playerControlFlag.startNorChangeFlag){
			if (cameraMoveVecUp.y < 0){
				//Graphic::GetInstance().DrawLine(RCMatrix4::getPosition(player._Get()->GetParameter().matrix), nords * 100, CAMERA_ID::PLAYER_CAMERA);
				//徐々にupをvector3(0,1,0)に近づける
				Vector3 nor = cameraMoveVecUp * RCQuaternion::rotate(moveVecFront, frontAngle);// frontM;// RCVector3::lerp((moveVecUp), vector3(0, 1, 0), startRotate);
				float startFrontDot = RCVector3::dot(nor, vector3(0, 1, 0));
				frontAngle += FRONTLEFTCHANGESPEED * frameTime;
				nor = cameraMoveVecUp * RCQuaternion::rotate(moveVecFront, frontAngle);
				bool plusFlag_ = true;
				float frontDot = RCVector3::dot(nor, vector3(0, 1, 0));
				if ((startFrontDot) >(frontDot)){
					frontAngle -= FRONTLEFTCHANGESPEED * 2 * frameTime;
					if (playerControlFlag.firstFrameFlag)
						plusFlag = false;
					plusFlag_ = false;
					nor = cameraMoveVecUp * RCQuaternion::rotate(moveVecFront, frontAngle);
					frontDot = RCVector3::dot(nor, vector3(0, 1, 0));
				}
				if ((startFrontDot) > (frontDot)){
					playerControlFlag.startNorChangeFlag = false;
					frontAngle = 0.0f;
					secondMoveVecUp = nor;
	
				}
				player._Get()->SetNor(nor);
				cameraMove = false;
				playerControlFlag.firstFrameFlag = false;
				if (plusFlag != plusFlag_){
					playerControlFlag.startNorChangeFlag = false;
					frontAngle = 0.0f;
					secondMoveVecUp = nor;
				}
			}
			else{
				Vector3 nor = RCVector3::lerp(cameraMoveVecUp, vector3(0, 1, 0), frontAngle);
				frontAngle += STARTNORCHANGESPEED * frameTime;
				player._Get()->SetNor(nor);
				if (frontAngle > 1.0f){
					playerControlFlag.startNorChangeFlag = false;
				}
				playerControlFlag.firstFrameFlag = false;
			}
		}
		else{
			Vector3 nor = RCVector3::lerp(secondMoveVecUp, vector3(0, 1, 0), frontAngle);
			frontAngle += STARTNORCHANGESPEED * frameTime;
			player._Get()->SetNor(nor);
			if (frontAngle > 1.0f)playerControlFlag.startNorChangeFlag = false;
			playerControlFlag.firstFrameFlag = false;
		}
	}
}
void JumpAction::OnCollide(Actor& other, CollisionParameter colpara){
	//もし丸が当たっていたら
	if (colpara.id == COL_ID::SPHERE_MODEL_NATURAL_COLL){
		ChangeAction(ACTION_ID::NORMAL_ACTION);
		//当たった場所へ移動
		player._Get()->SetPos(colpara.colPos);
		//Upをセット
		player._Get()->SetNor(colpara.colNormal);
		//	startNor = colpara.colNormal;
	}
	else if (colpara.id == COL_ID::SPHERE_LINE_COLL && jumpSpeed < JUMPSPEED - 1.0f && !static_cast<Thread*>(&other)->IsShot()){
		if (static_cast<Thread*>(&other)->GetParameter().id == ACTOR_ID::PLAYER_THREAD_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ||
			static_cast<Thread*>(&other)->GetParameter().id == ACTOR_ID::ENEMY_THREAD_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::ENEMY_ACTOR){
			ChangeAction(ACTION_ID::THREAD_ACTION);
			player._Get()->SetThread(static_cast<Thread*>(&other)->shared_from_this());
		}
		//if (!dynamic_cast<Thread*>(&other)->ReturnDangleFlag())
		//ChangeAction(ACTION_ID::THREAD_ACTION);
	}
	else if (colpara.id == COL_ID::TRIANGLE_SPHERE_COLL && jumpSpeed < JUMPSPEED - 1.0f){
		if (static_cast<ThreadWeb*>(&other)->GetParameter().id == ACTOR_ID::PLAYER_THREADWEB_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ||
			static_cast<ThreadWeb*>(&other)->GetParameter().id == ACTOR_ID::ENEMY_THREADWEB_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::ENEMY_ACTOR){
			ChangeAction(ACTION_ID::THREAD_WEB_ACTION);
			player._Get()->SetThreadWeb(static_cast<ThreadWeb*>(&other)->shared_from_this());
		}
	}
}