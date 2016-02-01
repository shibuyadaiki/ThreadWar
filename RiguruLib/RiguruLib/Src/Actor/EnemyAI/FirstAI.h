#pragma once
#include <array>
#include "EnemyAI.h"
#include "../Base.h"
#include "../../math/Vector3Utility.h"
#include "../../math/Vector2Utility.h"
#include "../../Graphic/Graphic.h"
#include "../Player.h"
#include "../../Math/Quaternion.h"
#include "../../Math/V3Calculation.h"
#include "../../Actor/Stage.h"
#include "../ID.h"
#include "../PlayerAction/PlayerActionManager.h"

//�s��
enum AI_ACTION{
	Curl,
	RandomMove,
	RandomLook,
	TurnLook,
	JumpShotRewind,
	FrontLook,
	TargetPointLookH,
	TargetPointLookV,
	TargetPointLook,
	TargetPointLookAttack,
	TargetPointMove,
	TargetPointMoveAttack,
	CrystalPointMoveAttack,
	TargetJumpShotRewind,
	GettingThread,
	CreateThreadWeb,
};

struct AILevel{
	//����p(���g��flont����̊p�x)
	float viewAngle;
	//���싗��
	float viewLength;
	//�U���͈͊p(�J������flont����̊p�x)
	float attackAngle;
	//�U���͈͋���
	float attackLength;
	//��������Đڋ߂��鎞�̍ŒZ����(�����艓���ꍇ�͊�������Đڋ�)
	float rewindLength;
	//���z����
	float bestLength;
	//�����I�ɐ퓬��ԂɈڍs���鋗��
	float dangerLength;
	//�ǂ��ђʂ��Č�����b��
	float penetrationTime;
	//���E�ړ���؂�ւ���b���Ԋu
	float inputXChangeTimeMax, inputXChangeTimeMin;
	//�ʏ펞�̃X�e�b�v����b���Ԋu
	float stepTimeMax, stepTimeMin;
	//�퓬���̃X�e�b�v����b���Ԋu
	float battleStepTimeMax, battleStepTimeMin;
};

//�ڕW�̕ߑ��Ɋւ���p�����[�^�[
struct TargetParameter{
	//�ڕW�̃}�g���b�N�X
	Matrix4 mat;
	//�ڕW�n�_
	Vector3 pos;
	//��������ڕW�n�_�ւ̕���
	Vector3 vec;
	//���E���ɖڕW�����邩�H
	bool isLooking;
	//�Ō�Ɏ��E���ɑ������ڕW�n�_
	Vector3 lastLookingPos;
	//�Ō�Ɏ��E���ɑ������ڕW�n�_�ւ̕���
	Vector3 lastLookingVec;
	//���g�ƖڕW�n�_�̋���
	float distance;
	//�퓬��Ԃ��H(���E���ɖڕW�𑨂���`�������Ă���2�b�o�߁@�̊�)
	bool isBattle;
	//�U���͈͓��ɂ��邩�H
	bool isAttack;

};

class FirstAI : public EnemyAI{
public:
	FirstAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_);
	virtual void Initialize() override;
	virtual Vector3 UpdateAI(float frameTime) override;
	virtual void DrawAI() const override{}

private:
	//�p�����[�^���AAI���s�O�ɕK�v�Ȍv�Z
	void ParamCalc();
	//AI���s
	void AIRun(float frameTime);
	//���͏�Ԃ����Z�b�g
	void InputReset();

	//�X�e�[�g��ς���
	void ChangeAction(AI_ACTION s);
	//���g���V��ɂ��邩�H
	bool IsCeiling();

	/*********=�s��=*********/
	//�R�b�Ԋۂ܂�
	void Curl(float frameTime);
	//���G�ړI�̃����_���Ȉړ�(�΂ߑO���Ƀ����_���ړ�)
	void RandomMove(float frameTime);
	//���G�ړI�̃����_���Ȍ����ύX
	void RandomLook(float frameTime);
	//���G�ړI�̂P�����̌����ύX
	void TurnLook(float frameTime);
	//���G�ړI�̊�����荂���ړ�
	void JumpShotRewind(float frameTime);
	//�΂ߏ���������W�����v�����ˁ������@�V��ւ̈ړ���i
	void SlantingShotRewind(float frameTime);
	//���������c�������w�肷��ƌ����Ă����
	void LookV(float frameTime, Vector3 v);
	//���ʂ�����
	void FrontLook(float frameTime);
	//�������̂ݖڕW�n�_������
	void TargetPointLookH(float frameTime);
	//�c�����̂ݖڕW�n�_������
	void TargetPointLookV(float frameTime);
	//�ڕW�n�_������
	void TargetPointLook(float frameTime);
	//�ڕW�n�_�֍U������
	void TargetPointLookAttack(float frameTime);
	//�ڕW�n�_�ֈړ�����
	void TargetPointMove(float frameTime);
	//�ڕW�n�_�ֈړ����Ȃ���U������
	void TargetPointMoveAttack(float frameTime);
	void CrystalPointMoveAttack(float frameTime);
	//�ڕW�n�_�������Ȃ���W�����v�����ˁ������@�ڕW�n�_�ւ̋}���ڐڋߎ�i
	void TargetJumpShotRewind(float frameTime);
	//���ɏ��
	void GettingThread(float frameTime);
	//�w偂̑������
	void CreateThreadWeb(float frameTime);

	/*********=���G=*********/
	//������ɓG�����邩
	bool Search();
	//�����\�� 
	void SearchLineDraw();
	//�U���͈͓��ɓG�����邩
	bool AttackArea();
	//�U���͈͂�\�� 
	void AttackAreaLineDraw();

	/*********=���́E����=*********/
	//�ۂ܂�
	void InputCurlOn();
	//�ۂ܂���I��
	void InputCurlOff();
	//�W�����v
	void InputJump();
	//�X�e�b�v
	void InputStep();
	//���̔���
	void InputShotThread();
	//�����������
	void InputRewind();
	//�X�e�b�v�ۂ܂�(�X�e�b�v�̊������󂯌p�����ۂ܂�)
	void InputStepCurl();

	//�ŏI�I�ȓ��͏���
	void FinalInput();

private:
	//�v���C���[�N���X���������Ă���
	std::weak_ptr<Stage> stage;

private:
	/*****=AI���s=******/
	//�^�C�}�[
	float aiTimer;
	//�퓬��Ԏ��A���E���ɖڕW�𑨂����Ɍo�߂�������
	float lostTimer;

	/*****=�s��=******/

	//1�`�[���̐l��
	const int teamMemberCount = 4;
	//�G4�̂̃}�g���b�N�X
	std::array<Matrix4, 4> targetMats;
	//�G4�̂܂ł̋���
	std::array<float, 4> targetDists;
	//�G
	int targetNum;
	//�T�[�`�����Ώ�(�G�̃A�N�^�[ID)
	ACTOR_ID searchActor;

	//�ڕW
	TargetParameter target;

	//�N���X�^���̈ʒu�@����
	const Vector3 centerCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, -1.5f), vector3(0.5f, 5.5f, 1.5f), 0.5);
	//�X��(PlayerSide)
	const Vector3 iceCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, 62.0f), vector3(0.5f, 5.5f, 65.3f), 0.5);
	//���A��(EnemySide)
	const Vector3 caveCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, -67.0f), vector3(0.5f, 5.5f, -64.0f), 0.5);
	//�e�N���X�^���܂ł̋���
	float centerCrystalDist, iceCrystalDist, caveCrystalDist;
	//��ԋ߂��U���ΏۃN���X�^��
	Vector3 nearestCrystalPos;//�ʒu
	float nearestCrystalDist;//����
	//�N���X�^����_�����H
	bool targetCrystal;

	//�ėp�t���O
	bool flag;
	//�ėp�^�C�}�[
	float timer;
	//�ėp�J�E���^�[
	int counter;
	//�s�����I��������H
	bool stateEnd;

	//���E�ړ��؂�ւ����ԁA�X�e�b�v�����^�C�}�A�����o�^�C�}�A�W�����v�^�C�}
	float moveTime, stepTime, threadTime, jumpTime;
	float moveTimeMax, stepTimeMax, threadTimeMax;
	//���E�ړ����͒l
	float inputVecX;
	float inputCameraX;

	//���g�̃p�����[�^�[
	Matrix4 myMat;
	Vector3 myPos;
	Vector2 myPos2D;
	PlayerParameter* myParam;

	//�J����
	Camera* camera;
	//�J�����̉�]�s��
	Matrix4 cameraRotMat;
	Vector3 cameraFront;
	Vector3 cameraLeft;
	Vector3 cameraUp;

	//���݂̍s��
	AI_ACTION currentAction;
	std::map<AI_ACTION, std::function<void(float)>> actionFunc;

	/*****=����=******/
	//���g�̑O�㍶�E�ړ��̓���
	Vector3 inputVec = vector3(0, 0, 0);
	//�J�����̕�������
	Vector2 inputAngle = vector2(0.0f, 0.0f);

	/*****=��Փx�EAI�x�����x��=******/
	std::array<AILevel, 3> level;
	int levelNum;
};