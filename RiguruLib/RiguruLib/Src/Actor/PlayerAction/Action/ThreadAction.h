#pragma once
#include "../PlayerAction.h"
#include "../../ThreadParamter.h"
class Thread;

class ThreadAction : public PlayerAction{
public:
	ThreadAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	~ThreadAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
private:
	float startRotate;

	ANIM_ID frontBackID;
	ANIM_ID leftRightID;
	bool animFront;
	float frontRotate;

	ThreadParameter hitTparam;
	Vector3 hitPos;
	bool groundFirstFrame;

};