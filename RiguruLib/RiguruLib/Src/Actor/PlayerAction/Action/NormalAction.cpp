#include "NormalAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../ThreadWeb.h"

NormalAction::NormalAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :PlayerAction(world, player_, padNum_, cID_, ACTION_ID::NORMAL_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
	beforeVec = vector3(0, 0, 0);
}
NormalAction::~NormalAction(){

}
bool NormalAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	beforeVec = vector3(0, 0, 0);
	bool animTimeReset = false;
	if (beforeId == ACTION_ID::GROUND_CURL_ACTION || beforeId == ACTION_ID::AIR_CURL_ACTION || beforeId == ACTION_ID::STEP_ACTION)beforeActionCurl = true;
	else beforeActionCurl = false;
	if (beforeId != ACTION_ID::JUMP_ACTION && beforeId != ACTION_ID::HOVER_ACTION && beforeId != ACTION_ID::STEP_ACTION)
		animTimeReset = true;
	beforeActionCurlCount = 0;
	cameraMove = true;
	if (!(controlVec == vector3(0, 0, 0))){
		player._Get()->SetAnimTime(WALKANIMSPEED);
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

	player._Get()->SetAnimBlend(WALKANIMBLEND);
	player._Get()->SetAnimation(
		(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		WALKANIMSPEED, animTimeReset, true, abs(controlVecNor.z), WALKANIMBLEND);

	moveVecUp = beforeUp;
	moveVecPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	startRotate = 0;

	playerControlFlag.firstFrameFlag = true;
	playerControlFlag.startNorChangeFlag = true;
	playerControlFlag.secondFrameFlag = false;
	//startNorChangeFlag = false;
	//cameraDrawChange = false;
	//firstFrameFlag = true;
	change = false;
	return true;
}
void NormalAction::Update(float frameTime){
	if (playerControlFlag.startNorChangeFlag && !playerControlFlag.firstFrameFlag){
		//���X��up��vector3(0,1,0)�ɋ߂Â���
		Vector3 nor;
		Vector3 pos;
		if (beforeAction != ACTION_ID::THREAD_ACTION && beforeAction != ACTION_ID::REWIND_ACTION){
			nor = RCVector3::lerp((moveVecUp), startNor, startRotate);
			pos = RCVector3::lerp((moveVecPos), startPos, startRotate);
			if (beforeAction == ACTION_ID::JUMP_ACTION || beforeAction == ACTION_ID::HOVER_ACTION){
				pos = startPos;
			}
		}
		else {
			nor = RCVector3::lerp((moveVecUp), startNor, startRotate);
			pos = RCVector3::lerp((moveVecPos), startPos, startRotate);
			//nor = moveVecUp;
			//pos = moveVecPos;
			//playerControlFlag.startNorChangeFlag = false;
		}
		player._Get()->SetNor(nor);
		player._Get()->SetPos(pos);
		startRotate += STARTNORCHANGESPEED * frameTime;
		if (startRotate > 1.0f){
			playerControlFlag.startNorChangeFlag = false;
		}
	}

	//*upAniTime +=10.0f * frameTime;
	//�����蔻����S�{�̃��C��
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::RAY_MODEL_NATURAL_COLL);
	//���Ƃ̔���
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadID, COL_ID::SPHERE_LINE_COLL);
	//�w偂̑��Ƃ̔���
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadWebID, COL_ID::TRIANGLE_SPHERE_COLL);

	Matrix4 angleMat = player._Get()->ReturnPlayerParameter()->angleMat;
	Vector3 moveVecFront = RCVector3::normalize(RCMatrix4::getFront(angleMat));
	Vector3 moveVecLeft = RCVector3::normalize(-RCMatrix4::getLeft(angleMat));
	bool changeFlag = false;
	if (!(beforeVec == controlVec)){
		if (!(controlVec == vector3(0, 0, 0))){
			changeFlag = true;
		}
	}

	CAMERA_PARAMETER c = *Device::GetInstance().GetCamera(cID)->CameraParam();
	if (!(controlVec == vector3(0, 0, 0))){
		player._Get()->SetAnimTime(WALKANIMSPEED * min(abs(RCVector3::length(controlVec)), 10.0f));
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
		//�ړ��������ɉ�]�����Ă�����(�E���)
		if (c.InputAngle.x > 0){
			if (leftRightID != ANIM_ID::NEPHILA_TURNLEFT_ANIM)
				changeFlag = true;
			//�E��]���Z�b�g
			leftRightID = ANIM_ID::NEPHILA_TURNLEFT_ANIM;
			frontBackID = ANIM_ID::NEPHILA_TURNLEFT_ANIM;

			player._Get()->SetAnimTime(-6.0f * min(abs(c.InputAngle.x), 10.0f));
			player._Get()->SetAnimBlend(WALKANIMBLEND);
		}
		//�ړ��������ɉ�]�����Ă�����(�E���)
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

	if (beforeActionCurl)player._Get()->SetAnimBlend(CURLANIMBLEND);
	if (beforeActionCurlCount < 1.0f)
		beforeActionCurlCount += WALKANIMBLEND * frameTime;
	else beforeActionCurl = false;

	if (changeFlag || playerControlFlag.firstFrameFlag)	player._Get()->SetAnimation(
		(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		WALKANIMSPEED, false, true, abs(controlVecNor.z), WALKANIMBLEND);

	Vector3 moveVec = (moveVecFront * controlVec.z + moveVecLeft * controlVec.x) * SPEED * frameTime;
	if (!playerControlFlag.startNorChangeFlag && !change)
		player._Get()->SetMoveVec(moveVec);

	beforeVec = controlVec;
	

	threadVec.clear();
	threadWebVec.clear();
}
void NormalAction::OnCollide(Actor& other, CollisionParameter colpara){
	//�����S�{�̃��C���������Ă�����
	if (colpara.id == COL_ID::RAY_MODEL_NATURAL_COLL){
		//���������ꏊ�ֈړ�
		if (playerControlFlag.firstFrameFlag){
			startNor = colpara.colNormal;
			startPos = colpara.colPos;
			playerControlFlag.firstFrameFlag = false;
			playerControlFlag.secondFrameFlag = true;
		}
		if (!playerControlFlag.startNorChangeFlag){
			//��������Z�b�g
			player._Get()->SetNor(colpara.colNormal);
			player._Get()->SetPos(colpara.colPos);
		}
	}
	else if (colpara.id == COL_ID::SPHERE_LINE_COLL && other.GetParameter().id == friendThreadID && !static_cast<Thread*>(&other)->IsShot()){
		threadVec.push_back(static_cast<Thread*>(&other)->shared_from_this());
	}
	else if (colpara.id == COL_ID::TRIANGLE_SPHERE_COLL && other.GetParameter().id == friendThreadWebID){
		threadWebVec.push_back(static_cast<ThreadWeb*>(&other)->shared_from_this());
	}
}