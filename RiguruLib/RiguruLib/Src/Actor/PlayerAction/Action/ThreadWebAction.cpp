#include "ThreadWebAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../../Math/Quaternion.h"
#include "../../ThreadWeb.h"

ThreadWebAction::ThreadWebAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :
PlayerAction(world, player_, padNum_, cID_, ACTION_ID::THREAD_WEB_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
	hit = true;
	beforePos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	hitGround = false;
	beforeVec = vector3(0, 0, 0);
}
ThreadWebAction::~ThreadWebAction(){

}
bool ThreadWebAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	cameraMove = true;
	playerControlFlag.firstFrameFlag = true;
	playerControlFlag.startNorChangeFlag = true;
	playerControlFlag.secondFrameFlag = false;
	moveVecUp = player._Get()->ReturnPlayerParameter()->nor;
	moveVecPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	beforeVec = vector3(0, 0, 0);

	beforePos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);

	if (!(controlVec == vector3(0, 0, 0))){
		player._Get()->SetAnimTime(THREADANIMSPEED);
		if (controlVec.x >= 0){
			leftRightID = ANIM_ID::NEPHILA_WALKRIGHT_ANIM;
		}
		else{
			leftRightID = ANIM_ID::NEPHILA_WAITLEFT_ANIM;
		}

		if (controlVec.z >= 0){
			frontBackID = ANIM_ID::NEPHILA_WALKBACK_ANIM;
		}
		else {
			frontBackID = ANIM_ID::NEPHILA_WALKFRONT_ANIM;
		}
	}
	else{
		player._Get()->SetAnimTime(0.0f);
		frontBackID = ANIM_ID::NEPHILA_WAIT_ANIM;
		leftRightID = ANIM_ID::NEPHILA_WAIT_ANIM;
	}
	player._Get()->SetAnimation(
		(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		THREADANIMSPEED, false, true, abs(controlVecNor.z), WALKANIMBLEND);
	startRotate = 0;
	change = false;
	return true;
}
void ThreadWebAction::Update(float frameTime){
	if (player._Get()->ReturnThreadWeb().lock() == nullptr){
		ChangeAction(ACTION_ID::HOVER_ACTION);
	}

	if (!hit)player._Get()->SetPos(beforePos);
	else beforePos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);

	if (playerControlFlag.startNorChangeFlag && !playerControlFlag.firstFrameFlag){
		//™X‚Éup‚ðvector3(0,1,0)‚É‹ß‚Ã‚¯‚é
		Vector3 nor = RCVector3::lerp((moveVecUp), startNor, startRotate);
		Vector3 pos = RCVector3::lerp((moveVecPos), startPos, startRotate);
		player._Get()->SetNor(nor);
		player._Get()->SetPos(pos);
		startRotate += STARTNORCHANGESPEED * frameTime;
		if (startRotate > 1.0f){
			playerControlFlag.startNorChangeFlag = false;
		}
	}

	//‚ ‚½‚è”»’è‚ðŠÛ‚É
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::SPHERE_MODEL_THREADWEB_COLL);
	//’wå‚Ì‘ƒ‚Æ‚Ì”»’è
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadWebID, COL_ID::TRIANGLE_SPHERE_COLL);
	//’wå‚Ì‘ƒ‚Æ‚Ì”»’è
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadID, COL_ID::SPHERE_LINE_COLL);


	Matrix4 angleMat = player._Get()->ReturnPlayerParameter()->angleMat;
	Vector3 moveVecFront = RCVector3::normalize(RCMatrix4::getFront(angleMat));
	Vector3 moveVecLeft = RCVector3::normalize(-RCMatrix4::getLeft(angleMat));

	bool changeFlag = false;
	if (!(beforeVec == controlVec)){
		if (!(controlVec == vector3(0, 0, 0))){
			changeFlag = true;
		}
	}

	if (!(controlVec == vector3(0, 0, 0))){
		player._Get()->SetAnimTime(THREADANIMSPEED * min(abs(RCVector3::length(controlVec)), 10.0f));
		player._Get()->SetAnimBlend(WALKANIMBLEND);
		if (controlVec.x >= 0){
			leftRightID = ANIM_ID::NEPHILA_WALKRIGHT_ANIM;
		}
		else{
			leftRightID = ANIM_ID::NEPHILA_WAITLEFT_ANIM;
		}

		if (controlVec.z >= 0){
			frontBackID = ANIM_ID::NEPHILA_WALKBACK_ANIM;
		}
		else {
			frontBackID = ANIM_ID::NEPHILA_WALKFRONT_ANIM;
		}
	}
	else{
		player._Get()->SetAnimTime(0);
		CAMERA_PARAMETER c = *Device::GetInstance().GetCamera(cID)->CameraParam();
		//ˆÚ“®‚ð‚¹‚¸‚É‰ñ“]‚ð‚µ‚Ä‚¢‚½‚ç(‰E‰ñ‚è)
		if (c.InputAngle.x > 0){
			if (leftRightID != ANIM_ID::NEPHILA_TURNLEFT_ANIM)
				changeFlag = true;
			//‰E‰ñ“]‚ðƒZƒbƒg
			leftRightID = ANIM_ID::NEPHILA_TURNLEFT_ANIM;
			frontBackID = ANIM_ID::NEPHILA_TURNLEFT_ANIM;

			player._Get()->SetAnimTime(-6.0f * min(abs(c.InputAngle.x), 10.0f));
			player._Get()->SetAnimBlend(WALKANIMBLEND);
		}
		//ˆÚ“®‚ð‚¹‚¸‚É‰ñ“]‚ð‚µ‚Ä‚¢‚½‚ç(‰E‰ñ‚è)
		else if (c.InputAngle.x < 0){
			if (leftRightID != ANIM_ID::NEPHILA_TURNLEFT_ANIM)
				changeFlag = true;
			leftRightID = ANIM_ID::NEPHILA_TURNLEFT_ANIM;
			frontBackID = ANIM_ID::NEPHILA_TURNLEFT_ANIM;

			player._Get()->SetAnimTime(6.0f * min(abs(c.InputAngle.x), 10.0f));
			player._Get()->SetAnimBlend(WALKANIMBLEND);

		}
		else{
			if (leftRightID == ANIM_ID::NEPHILA_WAIT_ANIM && frontBackID == ANIM_ID::NEPHILA_WAIT_ANIM)
				player._Get()->SetAnimBlend(WALKANIMBLEND);
			else
				player._Get()->SetAnimBlend(0.0f);

			changeFlag = false;
		}
	}

	if (changeFlag || playerControlFlag.firstFrameFlag)	
		player._Get()->SetAnimation(
		(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		THREADANIMSPEED, false, true, abs(controlVecNor.z), WALKANIMBLEND);
	beforeVec = controlVec;
	Vector3 moveVec = (moveVecFront * controlVec.z + moveVecLeft * controlVec.x) * THREADSPEED * frameTime;
	if (!playerControlFlag.startNorChangeFlag && !change)
		player._Get()->SetMoveVec(moveVec);

	hit = false;
	hitGround = false;
	threadWebVec.clear();
	threadVec.clear();
}
void ThreadWebAction::OnCollide(Actor& other, CollisionParameter colpara){
	if (!change){
		if (colpara.id == COL_ID::TRIANGLE_SPHERE_COLL && other.GetParameter().id == friendThreadWebID){
			if (playerControlFlag.firstFrameFlag)
				playerControlFlag.secondFrameFlag = true;
			playerControlFlag.firstFrameFlag = false;
			if (player._Get()->ReturnThreadWeb()._Get() == &other){
				startPos = colpara.colPos;
				Vector3 playerUp = RCVector3::normalize(RCMatrix4::getUp(player._Get()->GetParameter().matrix));
				Vector3 colNor = RCVector3::normalize(colpara.colNormal);
				if (RCVector3::length(playerUp - colNor) > RCVector3::length(playerUp + colNor))colNor = -colNor;
				startNor = colNor;
				if (!playerControlFlag.startNorChangeFlag){
					player._Get()->SetPos(colpara.colPos);
					player._Get()->SetNor(colNor);
				}
				hit = true;
			}
			else{
				threadWebVec.push_back(static_cast<ThreadWeb*>(&other)->shared_from_this());
			}
		}
		else if (colpara.id == COL_ID::SPHERE_LINE_COLL && other.GetParameter().id == friendThreadID&& !static_cast<Thread*>(&other)->IsShot()){
			threadVec.push_back(static_cast<Thread*>(&other)->shared_from_this());
		}
		else if (colpara.id == COL_ID::SPHERE_MODEL_THREADWEB_COLL){
			hitGround = true;
			if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
				Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R3, true)) &&
				!playerControlFlag.firstFrameFlag){
				ChangeAction(ACTION_ID::NORMAL_ACTION);
				player._Get()->SetPos(colpara.colPos);
				player._Get()->SetNor(colpara.colNormal);
			}
		}
	}
}