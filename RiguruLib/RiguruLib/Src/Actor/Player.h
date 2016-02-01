#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "ID.h"
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"
#include "PlayerAction\PlayerActionManager.h"
struct CAMERA_PARAMETER;
class EnemyAI;
class Stage;
class ThreadWeb;
enum AI_STATE;
class Thread;

struct PlayerParameter{
	Vector3 vec;
	Matrix4 angleMat;
	//�ݒu���Ă邩�ǂ���
	bool onGroundFlag;
	bool onThreadFlag;
	Vector3 nor;
	float hp;
};

class Player :public Actor, public std::enable_shared_from_this<Player>{
public:
	Player(IWorld& world_, std::weak_ptr<Stage> stage_,CAMERA_ID cID_,int padNum_,int playerNum_);
	~Player();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//�}�g���b�N�X��Ԃ�
	Matrix4* ReturnMat();
	//�|�W�V�������Z�b�g
	void SetPos(Vector3 position);
	//�ړ��x�N�g�����Z�b�g
	void SetMoveVec(Vector3 moveVec);
	//�v���C���[�p�����[�^��Ԃ�
	PlayerParameter* ReturnPlayerParameter();
	//��������Z�b�g
	void SetNor(Vector3 nor_);
	//�v���C���̃`�[��ID���擾�B
	TEAM_ID GetTeam();
	//���̔��ˁB
	void ShotThread();
	//�S�����ԓ����B
	bool IsBind();
	//���ݍS������Ă��邩�𔻒肵�A�S������Ă����������Ԃ��B
	float BindPow(float pow);
	//���݂ǂ�ID�ɂ���čS������Ă��邩���肵�AID���Ƃɐݒ肳�ꂽ�s���̌����l��Ԃ��B
	//�S������Ă��Ȃ����1.0���Ԃ�A�������Ȃ��Brec�͋t�����ǂ����B
	float BindPow(bool rec = false);

	float* animSpeed;
	float* blendSpeed;
	void SetAnimTime(float time);
	void SetAnimBlend(float blend);
	void SetAnimation(ANIM_ID firstActionId, ANIM_ID secondActionId, float animUpdateTime, bool animTimeReset, bool roop, float blendLevel, float beforeAnimBlendLevel);
	void SetThread(std::weak_ptr<Thread> thread_);
	std::weak_ptr<Thread> ReturnThread();
	void SetThreadWeb(std::weak_ptr<ThreadWeb> threadWeb_);
	std::weak_ptr<ThreadWeb> ReturnThreadWeb();
	void SetBindFlag(bool flag){ bindFlag = flag; }
	void SetBindTime(float bindTime_);
	bool ReturnBindFlag(){ return bindFlag; }
	float ReturnBindTime(){ return bindTime; }
	int ReturnPlayerNum(){ return playerNum; }

	void Damage(float damagePoint);
private:
	PlayerParameter playerParam;
	PlayerActionManager pAM;
	//����
	Vector3 Control(float frameTime, CAMERA_PARAMETER c);
	//�m�[�}������Rotate���v�Z
	Matrix4 NorToRotateMat(CAMERA_PARAMETER c, Vector3 normal, Matrix4 matrix, float angle);

	Player* thisCopy;

	//bool dokusai,playerAI;

private:
	//PlayerAction playerAction;
	/********��{�p�����[�^********/
	//�X�e�[�W�̃|�C���^�AThread�ɑ���p
	std::weak_ptr<Stage> stage;
	//�e�X�g�\���p�̃t���O
	bool hitFlag;
	//�v���C���[�̃|�W�V����
	Vector3 pos;
	//�O�̉�]�ɑ����Ă����̂ō��������߂����
	float angleF;
	//�`��p�}�g���b�N�X
	Matrix4 drawMatrix;
	//�J�����p�̃}�g���b�N�X
	Matrix4 cameraMatrix;

	float curlAngle;

	Vector3 nor;

	CAMERA_ID cID;

	int padNum;

	Matrix4 paramMat;

	int hpHealTime;//HP�̎��R�񕜃^�C�}�B
	int damageCoolTime;//�_���[�W�̃N�[���^�C���B
	int playerNum;//�v���C���[�ԍ� 4vs4 0�`7�܂�

	Vector3 respawnPoint;//���X�|�[��������W
	float respawnTimer;//���X�|�[������܂ł̎��Ԍv��
	bool isRespawn;//���X�|�[�����邩�H
	bool stageOut;

	float nodamageTimer;//���X�|�[������̖��G����
	bool isNodamage;//���G���Ԃ��H


	/**************��****************/
	bool bindFlag;
	float bindTime;
	bool threadHit;
	float frameTime_;

	Vector3 startNor;
	Vector3 dangleStartFront;
	float dangleRotate;
	/**************AI****************/
	AI_STATE currentAI;
	std::map<AI_STATE, std::shared_ptr<EnemyAI>> ai;
	TEAM_ID teamID;
	Vector3 targetPos;

	/*************����***************/
public:
	void SetInputCurlOn(bool set);
	void SetInputCurlOff(bool set);
	void SetInputJump(bool set);
	void SetInputStep(bool set);
	void SetInputShot(bool set);
private:
	//AI�Ŏg�p
	bool inputAI;
	bool inputCurlOn;
	bool inputCurlOff;
	bool inputJump;
	bool inputStep;
	bool inputShot;

	bool firstFlag;
};