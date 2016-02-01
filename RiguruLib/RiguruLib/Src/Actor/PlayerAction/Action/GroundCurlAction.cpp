#include "GroundCurlAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../../Math/Quaternion.h"

GroundCurlAction::GroundCurlAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :
PlayerAction(world, player_, padNum_, cID_, ACTION_ID::GROUND_CURL_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
}
GroundCurlAction::~GroundCurlAction(){

}
bool GroundCurlAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	if (beforeAction == ACTION_ID::DANGLE_ACTION || beforeAction == ACTION_ID::REWIND_ACTION)player._Get()->ReturnThread()._Get()->SetIsDead(true);
	//�ǎC��x�N�g��������
	curlInertiaVec = player._Get()->GetParameter().inertiaVec;
	curlSpeed = RCVector3::length(curlInertiaVec);
	curlInertiaVec = RCVector3::normalize(curlInertiaVec);

	player._Get()->SetAnimation(
		(ANIM_ID)(ANIM_ID::NEPHILA_ROUND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(ANIM_ID::NEPHILA_ROUND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		CURLANIMSPEED, true, true, 0, CURLANIMBLEND);
	cameraMove = true;
	playerControlFlag.firstFrameFlag = false;
	wallVec = vector3(0, 0, 0);

	return true;
}
void GroundCurlAction::Update(float frameTime){
	Vector3 nor = player._Get()->ReturnPlayerParameter()->nor;
	//�ۂ܂�O�̈ړ��ʂƃm�[�}������ǎC��x�N�g�������߂�
	Vector3 moveVec = (curlInertiaVec - RCVector3::dot(curlInertiaVec,nor) * nor) * curlSpeed;
	curlSpeed -= (curlSpeed * 1.25f) * frameTime;
	if (curlSpeed < 0) curlSpeed = 0;
	//�v�Z�p�d��
	Vector3 g = vector3(0, GRABITY, 0);
	//�d�͂Ɩ@������ǎC��x�N�g�������ߑO��̂ɑ����Ċ��炩�ɉ������Ă���(0.75f�͖��C)
	wallVec += (g - RCVector3::dot(g, nor) * nor) * 0.3f;
	//���̕ǎC��x�N�g������ǎC��x�N�g�������ߒ����n�ʂ�˂������ʂ悤�ɂ���
	wallVec = wallVec - RCVector3::dot(wallVec, nor) * nor;
	//�ǎC��x�N�g�������Z����
	moveVec += wallVec * frameTime;

	//if (!startNorChangeFlag)
	player._Get()->SetMoveVec(moveVec);
	//�n�ʂƂ̂����蔻��
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::RAY_MODEL_NATURAL_COLL);
	//�ǂɂ߂荞�܂Ȃ��悤�ɂ���Step�p�̏���
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::RAY_MODEL_STEP_COLL);
}
void GroundCurlAction::OnCollide(Actor& other, CollisionParameter colpara){
	//�����ۂ��������Ă�����
	if (colpara.id == COL_ID::RAY_MODEL_NATURAL_COLL){
		//���������ꏊ�ֈړ�
		player._Get()->SetPos(colpara.colPos);

		//��������Z�b�g
		player._Get()->SetNor(colpara.colNormal);

		if (colpara.colNormal.y < 0.0f){
			ChangeAction(ACTION_ID::AIR_CURL_ACTION);
		}

	}
	//����Step�����C���ǂɂ߂荞�񂾂�
	else if (colpara.id == COL_ID::RAY_MODEL_STEP_COLL){
		//���������ꏊ�ֈړ�
		player._Get()->SetPos(colpara.colPos);

		//��������Z�b�g
		player._Get()->SetNor(colpara.colNormal);
	}
}