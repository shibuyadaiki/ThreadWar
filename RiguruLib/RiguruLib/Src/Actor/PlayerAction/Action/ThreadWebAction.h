#pragma once
#include "../PlayerAction.h"
class Thread;
class ThreadWeb;
class ThreadWebAction : public PlayerAction{
public:
	ThreadWebAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	~ThreadWebAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
private:
	float startRotate;

	ANIM_ID frontBackID;
	ANIM_ID leftRightID;
	bool animFront;
	float frontRotate;
	bool hit;
	Vector3 beforePos;
	Vector3 beforeVec;

	Vector3 hitPos,hitNor;
};