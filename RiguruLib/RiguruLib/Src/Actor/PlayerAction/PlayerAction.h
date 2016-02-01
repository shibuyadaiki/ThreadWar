#pragma once

#include "../../Other/DX11User.h"
#include "../Actor.h"

class Player;
class Thread;
class ThreadWeb;

//�d��
const float  GRABITY = (-10.0f / 60.0f);
//�f�t�H���g�̃X�s�[�h
const float SPEED = 3.0f;
//���̏�̃X�s�[�h
const float THREADSPEED = 3.0f * 1.5f;
//�W�����v�̏����x
const float JUMPSPEED = 15.0f;
//�X�e�b�v�̏����x
const float STEPSPEED = 25.0f;

const float WALKANIMSPEED = 50.0f;
const float WALKANIMBLEND = 5.0f;

const float THREADANIMSPEED = 60.0f;

const float JUMPANIMSPEED = 30.0f;
const float JUMPANIMBLEND = 5.0f;

const float CURLANIMSPEED = 50.0f;
const float CURLANIMBLEND = 20.0f;

const float HOVERANIMSPEED = 10.0f;
const float HOVERANIMBLEND = 5.0f;

const float STEPRANIMSPEED = 50.0f;
const float STEPRANIMBLEND = 10.0f;

const float STARTNORCHANGESPEED = 8.0f;

const float FRONTLEFTCHANGESPEED = 600.0f;

const float REWINDSPEED = 24.0f;

const float STEPDELAY = 0.5f;

struct PlayerControl{
	bool firstFrameFlag = false;
	bool secondFrameFlag = false;
	bool startNorChangeFlag = false;
	bool isGoingEnd = false;
};

class PlayerAction{
public:
	PlayerAction(IWorld& world_, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_, ACTION_ID id);
	~PlayerAction();
	void PlayerActionUpdate(float frameTime, Vector3* grabity);
	void GetControl();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) = 0;
	void Control();
	virtual void Update(float frameTime) = 0;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) = 0;
	bool IsDead(){ return isDead; }
	ACTION_ID NextAction(){ return nextAction; }
	PlayerControl ReturnPlayerControl(){ return playerControlFlag; };
	void SetSecondFrameFlag(bool pc){ playerControlFlag.secondFrameFlag = pc; };
	void ChangeAction(ACTION_ID id);
	void SetIsDead(bool isDead_){ isDead = isDead_; }
	Vector3 ReturnUp(){ return beforeUp; }
	void GrabityReset(Vector3* grabity);
	bool ReturnRide(){ return ride; }
	bool ReturnHitGround(){ return hitGround; }
private:
protected:
	bool ride;
	bool hitGround;
	std::vector<std::weak_ptr<Thread>> threadVec;
	std::vector<std::weak_ptr<ThreadWeb>> threadWebVec;
	bool change;
	IWorld& world;
	std::weak_ptr<Player> player;
	int padNum;
	CAMERA_ID cID;
	Vector3 controlVec;
	Vector3 controlVecNor;
	ACTION_ID nextAction, beforeAction;
	bool isDead;
	bool cameraMove;
	PlayerControl playerControlFlag;
	Vector3 moveVecUp, beforeUp;
	Vector3 moveVecPos;
	Vector3 startNor;
	Vector3 startPos;
	//�ړ��̂Ƃ��ɓ����蔻����s�����ƒw偂̑�(����)
	ACTOR_ID friendThreadID;
	ACTOR_ID friendThreadWebID;
	//�ړ��̂Ƃ��ɓ����蔻����s�����ƒw偂̑�(�G)
	ACTOR_ID enemyThreadID;
	ACTOR_ID enemyThreadWebID;

	bool norUp;
	ACTION_ID thisAction;
};