#pragma once
#include "../PlayerAction.h"
class Thread;
class ThreadWeb;
class NormalAction : public PlayerAction{
public:
	NormalAction(IWorld& world,std::weak_ptr<Player> player_,int padNum_,CAMERA_ID cID_);
	~NormalAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	bool beforeActionCurl;

	float beforeActionCurlCount;

	ANIM_ID frontBackID;
	ANIM_ID leftRightID;
	float startRotate;
	Vector3 beforeVec;
};