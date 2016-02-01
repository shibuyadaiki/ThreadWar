#pragma once
#include "../PlayerAction.h"

class AirCurlAction : public PlayerAction{
public:
	AirCurlAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_);
	~AirCurlAction();
	virtual bool Initialize(ACTION_ID beforeId, Vector3 beforeUp) override;
	virtual void Update(float frameTime) override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	Vector3 curlInertiaVec;
	Vector3 wallVec;
	float curlSpeed;
};