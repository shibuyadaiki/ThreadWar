#include "DangleAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../../Math/Quaternion.h"

DangleAction::DangleAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :
PlayerAction(world, player_, padNum_, cID_, ACTION_ID::DANGLE_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
}
DangleAction::~DangleAction(){

}
bool DangleAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	player._Get()->SetAnimation(
		(ANIM_ID)(ANIM_ID::NEPHILA_WALKFRONT_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(ANIM_ID::NEPHILA_WALKFRONT_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		HOVERANIMSPEED, false, true, 0, HOVERANIMBLEND);
	beforeAction = beforeId;
	cameraMove = true;
	dangleInertiaVec = player._Get()->GetParameter().inertiaVec;
	dangleSpeed = RCVector3::length(dangleInertiaVec);
	dangleInertiaVec = RCVector3::normalize(dangleInertiaVec);
	Vector3 nor = RCVector3::normalize(player._Get()->ReturnThread()._Get()->GetThreadParameter().endPosition - RCMatrix4::getPosition(player._Get()->GetParameter().matrix));
	///if (nor.y > 0)
	///	player._Get()->SetNor(nor);
	//�ۂ܂�O�̈ړ��ʂƃm�[�}������ǎC��x�N�g�������߂�
	Vector3 cl = (dangleInertiaVec - RCVector3::dot(dangleInertiaVec, nor) * nor);
	wallVec = cl * dangleSpeed * 0.5f;// player._Get()->GetParameter().moveVec;
	//wallVec = vector3(0, 0, 0);
	playerControlFlag.firstFrameFlag = false;

	if (nor.y > 0){
		norUp = true;
	}
	else norUp = false;
	grabity = player._Get()->GetParameter().moveVec;
	moveVecUp = RCMatrix4::getUp(player._Get()->GetParameter().matrix);



	startRotate = 0;

	//startRotate = 0;
	//if (!startNorChangeFlag)
	//��������Z�b�g

	return true;
}
void DangleAction::Update(float frameTime){
	//�W�����v���̎��Ƃ̔���
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadID, COL_ID::SPHERE_LINE_COLL);
	//�W�����v���̒w偂̑��Ƃ̔���
	world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadWebID, COL_ID::TRIANGLE_LINE_JUMP_COLL);
	//�����蔻����ۂ�
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::SPHERE_MODEL_NATURAL_COLL);

	if (player._Get()->ReturnThread().lock() == nullptr){
		ChangeAction(ACTION_ID::HOVER_ACTION);
	}

	Vector3 nor = RCVector3::normalize(player._Get()->ReturnThread()._Get()->GetThreadParameter().endPosition - RCMatrix4::getPosition(player._Get()->GetParameter().matrix));
	Vector3 moveVec = vector3(0, 0, 0);//
	if (norUp){
		//�v�Z�p�d��
		Vector3 g = vector3(0, GRABITY, 0);
		//�d�͂Ɩ@������ǎC��x�N�g�������ߑO��̂ɑ����Ċ��炩�ɉ������Ă���(0.75f�͖��C)
		wallVec += (g - RCVector3::dot(g, nor) * nor) * frameTime;
		//���̕ǎC��x�N�g������ǎC��x�N�g�������ߒ����n�ʂ�˂������ʂ悤�ɂ���
		wallVec = wallVec - RCVector3::dot(wallVec, nor) * nor;
		//�ǎC��x�N�g�������Z����
		moveVec += wallVec;

		//if (!startNorChangeFlag)
		player._Get()->SetMoveVec(moveVec);

		if (!player._Get()->ReturnThread()._Get()->IsShot()){
			player._Get()->SetNor(RCVector3::lerp(moveVecUp, nor, startRotate));
			startRotate += 3.0f * frameTime;
		}
	}

	player._Get()->ReturnThread()._Get()->SetStartPos(RCMatrix4::getPosition(player._Get()->GetParameter().matrix) + moveVec);


	//�{�^�����͂Ŋ������J�n
	if ((Device::GetInstance().GetInput()->MouseButtonDown(INPUTMOUSEBUTTON::RIGHT_BUTTON, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R2, true)) && !player._Get()->ReturnThread()._Get()->IsShot()){
		ChangeAction(ACTION_ID::REWIND_ACTION);
	}
	grabity = vector3(0, 0, 0);

	{
		Vector3 g = vector3(0, GRABITY, 0);
		grabity += g * frameTime;
		player._Get()->SetMoveVec(grabity);
	}
}
void DangleAction::OnCollide(Actor& other, CollisionParameter colpara){
	//�����ۂ��������Ă�����
	if (colpara.id == COL_ID::SPHERE_MODEL_NATURAL_COLL){
		ChangeAction(ACTION_ID::NORMAL_ACTION);
		//���������ꏊ�ֈړ�
		player._Get()->SetPos(colpara.colPos);
		//Up���Z�b�g
		player._Get()->SetNor(colpara.colNormal);
		player._Get()->ReturnThread()._Get()->SetStartPos(colpara.colPos);
		player._Get()->ReturnThread()._Get()->SetDangleFlag(false);
		//player._Get()->ReturnThread() = NULL;
		//Vector3 endPos = player._Get()->ReturnThread()->GetThreadParameter().endPosition;
		//	startNor = colpara.colNormal;
	}
	else if (colpara.id == COL_ID::SPHERE_LINE_COLL && !static_cast<Thread*>(&other)->ReturnDangleFlag()){
		////���������ꏊ�ֈړ�
		//player._Get()->SetPos(colpara.colPos);
		////Up���Z�b�g
		//player._Get()->SetNor(colpara.colNormal);
		//
		//player._Get()->ReturnThread()->SetStartPos(colpara.colPos);
		//player._Get()->ReturnThread()->SetDangleFlag(false);
		//player._Get()->ReturnThread() = NULL;
		//ChangeAction(ACTION_ID::THREAD_ACTION);
	}
}