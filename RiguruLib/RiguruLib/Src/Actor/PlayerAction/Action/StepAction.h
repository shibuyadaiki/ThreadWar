#pragma once
#include "../PlayerAction.h"

class StepAction : public PlayerAction{
public:
	StepAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	~StepAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	void UpdateStepDelay(float frameTime);

private:
	bool jumpStepFlag;
	Vector3 stepVec;
	float stepSpeed;

	ANIM_ID frontBackID;
	ANIM_ID leftRightID;

	bool hover;

	float stepDelay;
};