#pragma once
#include "../PlayerAction.h"

class DangleAction : public PlayerAction{
public:
	DangleAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	~DangleAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	Vector3	dangleInertiaVec;
	float dangleSpeed;
	Vector3 wallVec;
	float startRotate;
	Vector3 grabity;
};