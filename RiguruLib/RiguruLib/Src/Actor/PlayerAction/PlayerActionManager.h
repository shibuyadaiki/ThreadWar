#pragma once
#include <memory>
#include "../../Other/DX11User.h"
#include "PlayerAction.h"
#include "PlayerActionPtr.h"

#include "../ID.h"
class Thread;
class IWorld;
class ThreadWeb;
class PlayerActionManager{
public:
	PlayerActionManager();
	~PlayerActionManager();
	void Initialize(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	void Control();
	void Update(float frameTime);
	void Collide(Actor& other,CollisionParameter colpara);
	void SetAnimation(ANIM_ID firstActionId, ANIM_ID secondActionId, float animUpdateTime, bool animTimeReset, bool roop, float blendLevel, float beforeAnimBlendLevel);
	void SetAnimTime(float time);
	void SetAnimBlend(float blend);
	void ChangeAction(ACTION_ID actionID);
	PlayerControl ReturnPlayerControl();
	void SetSecondFrameFlag(bool pc);
	ACTION_ID ReturnActionID(){ return a_id; };
	ACTION_ID ReturnBeforeActionID(){ return before_id; };
	void SetThread(std::weak_ptr<Thread> thread_,bool beforeThreadKill = false);
	std::weak_ptr<Thread> ReturnThread();
	void SetThreadWeb(std::weak_ptr<ThreadWeb> threadWeb_);
	std::weak_ptr<ThreadWeb> ReturnThreadWeb();
	PlayerActionPtr ReturnPlayerActionPtr(){ return playerAction[a_id]; }
private:
	std::map<ACTION_ID, PlayerActionPtr> playerAction;
	ACTION_ID a_id,before_id;
	std::weak_ptr<Player> player;
	float* animUpdateTime;
	float* beforeAnimBlendLevel;
	float animTime;
	Vector3 grabity;
	std::weak_ptr<Thread> thread;
	std::weak_ptr<ThreadWeb> threadWeb;
};