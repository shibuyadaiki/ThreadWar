#pragma once
#include "../PlayerAction.h"

class JumpAction : public PlayerAction{
public:
	JumpAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	~JumpAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	float jumpSpeed;
	Vector3 inertiaVec;
	float startRotate;
	Vector3 cameraMoveVecUp;
	bool cameraNorChangeFlag;
	bool plusFlag;
	float frontAngle;
	float leftAngle;
	Vector3 secondMoveVecUp;
	bool hover;
};