#pragma once
#include "ID.h"
#include "../math/Matrix4.h"
#include "../math/Vector3.h"
#include "../math/Vector3Utility.h"
#include "../math/MatrixUtility.h"
#include "../Graphic/Rgr/OctreeUser.h"
#include "../Other/DX11User.h"
#include <functional>
#include "../world/IWorld.h"
//あたり判定のパラメータ
struct CollisionParameter;
//アクターのパラメータ
struct ActorParameter
{
	ACTOR_ID id;
	Matrix4 matrix;
	bool isDead = false;
	Vector3 cubeScale;
	float radius;
	OCT_ID octId = OCT_ID::NULL_OCT;
	Vector3 moveVec;
	Vector3 inertiaVec;
	Vector3 lastNor;

};
enum TEAM_ID{
	FIRST_TEAM,
	SECOND_TEAM,
};
class Actor{
public:
	Actor(IWorld& world_);
	~Actor();
	virtual void Initialize() = 0;
	virtual void Update(float frameTime) = 0;
	virtual void Draw(CAMERA_ID cID) const = 0;
	void Collide(COL_ID id, Actor& other);
	virtual bool IsDead() const{ return parameter.isDead; }
	void SetIsDead(bool flag){ parameter.isDead = flag; }
	ActorParameter GetParameter();
protected:
	virtual void OnCollide(Actor& other,CollisionParameter colpara) = 0;
	ActorParameter parameter;
	IWorld& world;

private:
	std::map<COL_ID, std::function<CollisionParameter(const Actor&)>> colFunc;
	//球と球のあたり判定
	CollisionParameter SphereSphere(const Actor& other)const;
	//球と球のあたり判定
	CollisionParameter ThreadPlayer(const Actor& other)const;
	//レイとモデルのあたり判定
	CollisionParameter RayModel(const Actor& other)const;
	//レイとモデルのあたり判定(蜘蛛版ステップ)
	CollisionParameter RayModelStep(const Actor& other)const;
	//レイとモデルのあたり判定(蜘蛛版)
	CollisionParameter RayModelNatural(const Actor& other)const;
	CollisionParameter RayModelNaturalTest(Vector3 pos_)const;
	//球とモデルのあたり判定
	CollisionParameter SphereModel(const Actor& other)const;
	//球とモデルのあたり判定(蜘蛛版)
	CollisionParameter SphereModelNatural(const Actor& other)const;
	//球とモデルのあたり判定(巣の上)
	CollisionParameter SphereModelThreadWeb(const Actor& other)const;
	//ＯＢＢとＯＢＢのあたり判定
	CollisionParameter OBBOBB(const Actor& other)const;
	//プレイヤーと糸のあたり判定
	CollisionParameter SphereLine(const Actor& other)const;
	//プレイヤーと糸のあたり判定
	CollisionParameter LineSphere(const Actor& other)const;
	//プレイヤーと糸の末端のあたり判定
	CollisionParameter SphereLineEnd(const Actor& other)const;
	//プレイヤーと糸のあたり判定（ジャンプ中）
	CollisionParameter SphereLineJump(const Actor& other)const;
	//プレイヤーと蜘蛛の巣のあたり判定
	CollisionParameter TriangleLine(const Actor& other)const;
	//プレイヤーと蜘蛛の巣のあたり判定
	CollisionParameter TriangleSphere(const Actor& other)const;
	//糸と蜘蛛の巣のあたり判定
	CollisionParameter TriangleLine_T(const Actor& other)const;
	//プレイヤーと蜘蛛の巣のあたり判定（ジャンプ中）
	CollisionParameter TriangleLineJump(const Actor& other)const;
	//球とモデルのあたり判定(プレイヤー用、移動方向の先に判定球を置く)
	CollisionParameter SphereModelMove(const Actor& other)const;
	////レイとモデルのあたり判定(プレイヤー用、糸の上にいるとき)
	//CollisionParameter RayModelOnThread(const Actor& other)const;

	CollisionParameter CapsuleModel(const Actor& other)const;
	int cubeCount;
};