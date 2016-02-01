#pragma once
#include "../PlayerAction.h"

class HoverAction : public PlayerAction{
public:
	HoverAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	~HoverAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	Vector3 cameraMoveVecUp;
	bool cameraNorChangeFlag;
	bool plusFlag;
	float frontAngle;
	float leftAngle;
	float startRotate;
	bool beforeActionCurl;
	Vector3 secondMoveVecUp;
};