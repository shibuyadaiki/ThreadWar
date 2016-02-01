#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"
class Stage;
class UI :public Actor{
public:
	UI(IWorld& world_,std::weak_ptr<Stage> stage_);
	~UI();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	void SetHP(float hp);

private:
	Matrix4 mouseMat;
	float angle = 0;
	float angleZ = 0;
	float bvolume = 50;
	float svolume = 50;
	float animCount = 0;
	std::weak_ptr<Stage> stage;
	float hp;
	//bool drawFlag = true;
	float cursorAngle;
	float cursorSize;
	float cursorSizeTimeInterbal;
	float cursorSizeTime;
};