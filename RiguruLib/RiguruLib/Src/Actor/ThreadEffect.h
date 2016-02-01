#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <list>
#include <memory>
#include "Actor.h"

class Player;

class ThreadEffect :public Actor, public std::enable_shared_from_this<ThreadEffect>{
public:
	ThreadEffect(IWorld& world_, Matrix4& playerMat_,bool& bindFlag_, CAMERA_ID cID);
	~ThreadEffect();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	
private:
	Matrix4& playerMat;
	float upAngle;
	bool& bindFlag;
	Vector3 scaleUp;
	bool finishFlag;
};