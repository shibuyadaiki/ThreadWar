#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"
class CrystalCenter;
static float GameOverCount = 300.0f;
static int pointMax = 200;

class Stage :public Actor{
public:
	Stage(IWorld& world_, std::weak_ptr<CrystalCenter> crystalCenter_, std::weak_ptr<CrystalCenter> crystalPlayerSide_, std::weak_ptr<CrystalCenter> crystalEnemySide_,bool bgmPlay);
	~Stage();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	Matrix4* ReturnMat();
	void CrystalDamage(ACTOR_ID threadID, ACTOR_ID crystalID);
	std::weak_ptr<CrystalCenter> ReturnCrystal(ACTOR_ID crystalID);
	void AddPoint(ACTOR_ID id);//id:���񂾃v���C���[�̂����Ă���parameter.id
	float ReturnGameTime(){ return GameOverCount - gameTimer; }
	int ReturnTeamPoint(){ return teamPoint; }
	int ReturnPlayerTeamPoint(){ return playerPoint; };
	int ReturnEnemyTeamPoint(){ return enemyPoint; };
	int ReturnTotalPoint(){ return totalPoint; };
	float ReturnStartTime(){ return startTime; }
	VICTORY_ID ReturnWinner();
private:
	void CountDown(float count);
	float angle = 0;
	float angleZ = 0;
	float bvolume = 50;
	float svolume = 50;
	float animCount = 0;
	float yAngle;
	std::weak_ptr<CrystalCenter> crystalCenter, crystalPlayerSide, crystalEnemySide;

	//���Z�����|�C���g
	int addPoint = 0;
	//�`�[���̃|�C���g(0�`200�A200�ɋ߂��قǃv���C���[�`�[�����L���A0�ɋ߂��قǃG�l�~�[�`�[�����L��)
	int teamPoint = 100;
	int playerPoint = 0;
	int enemyPoint = 0;
	int totalPoint = 0;
	//�N���X�^���̏�Ԃ����Z�b�g���邽�߂̎���
	float crystalTimer = 0;
	//�Q�[���̌o�ߎ���
	float gameTimer = 0;
	//�Q�[���I�����ǂ���
	bool gameEnd = false;
	////�v���C���[�̏������ǂ����itrue:�v���C���[�`�[�������Afalse:�G�l�~�[�`�[�������j
	//bool playerTeamWin = true;
	////�����������ǂ���
	//bool draw = false;
	//���������`�[���B
	VICTORY_ID winner;
	bool gameStart;
	float startTime;
	bool bgmPlay;
};