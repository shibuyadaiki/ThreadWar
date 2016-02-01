#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"

struct CrystalParameter{
	bool crystalISPlayer;
	bool crystalISEnemy;
};

class CrystalCenter :public Actor, public std::enable_shared_from_this<CrystalCenter>{
public:
	CrystalCenter(IWorld& world_, ACTOR_ID crystalID);
	~CrystalCenter();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	void DrawEffect(Vector3 pos, Vector3 scale_, CAMERA_ID cID, Vector3 color) const;
	void Damage(bool player);
	bool IsPlayer();
	bool IsEnemy();
	//pointをリセットする
	void Reset();
	CrystalParameter ReturnCrystalParameter(){ return crystalParameter; }
private:
	CrystalParameter crystalParameter;
	int point;
	bool crystalISPlayer, crystalISEnemy;
	bool keep;//色をポイントが0になるまでキープする
	float angle;
	Vector3 scaleStart;
	bool startFlag;
};