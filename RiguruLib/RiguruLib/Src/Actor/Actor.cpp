#include "Actor.h"
#include "../Math/Converter.h"
#include "../Graphic/Graphic.h"
#include "Collision.h"
#include <float.h>
#include "Thread.h"
#include "ThreadWeb.h"
#include "ThreadBullet.h"
Actor::Actor(IWorld& world_) :world(world_)
{
	cubeCount = 0;
	colFunc[COL_ID::SPHERE_SPHERE_COLL] = std::bind(&Actor::SphereSphere, this, std::placeholders::_1);
	colFunc[COL_ID::THREAD_PLAYER_COLL] = std::bind(&Actor::ThreadPlayer, this, std::placeholders::_1);
	colFunc[COL_ID::RAY_MODEL_COLL] = std::bind(&Actor::RayModel, this, std::placeholders::_1);
	colFunc[COL_ID::OBB_OBB_COLL] = std::bind(&Actor::OBBOBB, this, std::placeholders::_1);
	colFunc[COL_ID::RAY_MODEL_NATURAL_COLL] = std::bind(&Actor::RayModelNatural, this, std::placeholders::_1);
	colFunc[COL_ID::SPHERE_MODEL_COLL] = std::bind(&Actor::SphereModel, this, std::placeholders::_1);
	colFunc[COL_ID::SPHERE_MODEL_NATURAL_COLL] = std::bind(&Actor::SphereModelNatural, this, std::placeholders::_1);
	colFunc[COL_ID::SPHERE_MODEL_THREADWEB_COLL] = std::bind(&Actor::SphereModelThreadWeb, this, std::placeholders::_1);
	colFunc[COL_ID::CAPSULE_MODEL_COLL] = std::bind(&Actor::CapsuleModel, this, std::placeholders::_1);
	colFunc[COL_ID::RAY_MODEL_STEP_COLL] = std::bind(&Actor::RayModelStep, this, std::placeholders::_1);
	colFunc[COL_ID::RAY_MODEL_COLL] = std::bind(&Actor::RayModel, this, std::placeholders::_1);
	colFunc[COL_ID::SPHERE_LINE_COLL] = std::bind(&Actor::SphereLine, this, std::placeholders::_1);
	colFunc[COL_ID::LINE_SPHERE_COLL] = std::bind(&Actor::LineSphere, this, std::placeholders::_1);
	colFunc[COL_ID::SPHERE_LINE_JUMP_COLL] = std::bind(&Actor::SphereLineJump, this, std::placeholders::_1);
	colFunc[COL_ID::TRIANGLE_LINE_COLL] = std::bind(&Actor::TriangleLine, this, std::placeholders::_1);
	colFunc[COL_ID::TRIANGLE_SPHERE_COLL] = std::bind(&Actor::TriangleSphere, this, std::placeholders::_1);
	colFunc[COL_ID::TRIANGLE_LINE_T_COLL] = std::bind(&Actor::TriangleLine_T, this, std::placeholders::_1);
	colFunc[COL_ID::TRIANGLE_LINE_JUMP_COLL] = std::bind(&Actor::TriangleLineJump, this, std::placeholders::_1);
	colFunc[COL_ID::SPHERE_MODEL_MOVE_COLL] = std::bind(&Actor::SphereModelMove, this, std::placeholders::_1);
	//colFunc[COL_ID::RAY_MODEL_ONTHREAD_COLL] = std::bind(&Actor::RayModelOnThread, this, std::placeholders::_1);
}

Actor::~Actor(){

}

void Actor::Collide(COL_ID id, Actor& other){
	//CapsulevsCapsule();
	CollisionParameter colpara = colFunc[id](other);
	if (colpara.colFlag)
	{
		OnCollide(other, colpara);
		other.OnCollide(*this, colpara);
	}
}
ActorParameter Actor::GetParameter()
{
	return parameter;
}
//球と球のあたり判定
CollisionParameter Actor::SphereSphere(const Actor& other) const{
	CollisionParameter colpara;
	if (parameter.id == ACTOR_ID::PLAYER_ACTOR) colpara.id = COL_ID::PLAYER_ENEMY_COLL;
	else colpara.id = COL_ID::ENEMY_PLAYER_COLL;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	Vector3 myPos = RCMatrix4::getPosition(parameter.matrix);
	Vector3 otherPos = RCMatrix4::getPosition(other.parameter.matrix);
	float distance = RCVector3::distance_(myPos, otherPos);
	float r = parameter.radius + other.parameter.radius;
	colpara.colFlag = false;
	colpara.colPos = myPos;

	float size2 = 3.0f;
	Matrix4 stageMat = RCMatrix4::matrix(
		vector3(size2, size2, size2),
		0.0f,
		0,
		0.0f,
		vector3(-1, -2.0f, 0));

	while (distance <= r){
		colpara.colFlag = true;
		colpara.colPos = colpara.colPos + RCVector3::normalize(myPos - otherPos) * 0.1f;// parameter.moveVec;
		CollisionParameter c = RayModelNaturalTest(colpara.colPos);
		if (c.colFlag)colpara.colPos = c.colPos;
		distance = RCVector3::distance_(colpara.colPos, otherPos);
	}
	return colpara;
}
//球と球のあたり判定
CollisionParameter Actor::ThreadPlayer(const Actor& other) const{
	CollisionParameter colpara;
	colpara.id = COL_ID::THREAD_PLAYER_COLL;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	Vector3 myPos = parameter.lastNor;
	Vector3 otherPos = RCMatrix4::getPosition(other.parameter.matrix);
	float distance = RCVector3::distance_(myPos, otherPos);
	float r = parameter.radius + other.parameter.radius;
	//Graphic::GetInstance().DrawSphere(myPos, parameter.radius, CAMERA_ID::PLAYER_CAMERA);
	//Graphic::GetInstance().DrawSphere(otherPos, other.parameter.radius, CAMERA_ID::PLAYER_CAMERA);
	colpara.colFlag = false;
	colpara.colPos = myPos;
	if (distance <= r)
		colpara.colFlag = true;
	return colpara;
}
//レイとモデルのあたり判定
CollisionParameter Actor::RayModel(const Actor& other)const{
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix);
	float rad = parameter.radius;
	Vector3 down = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 end = pos + down* rad;
	//Graphic::GetInstance().DrawLine(pos, end);
	return ModelRay(other.parameter.matrix, other.parameter.octId, pos, end);
}
//レイとモデルのあたり判定
CollisionParameter Actor::RayModelStep(const Actor& other)const{
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix) + RCMatrix4::getUp(parameter.matrix) * 3.0f;
	Vector3 rad = RCVector3::normalize(parameter.moveVec);
	Vector3 start = pos + rad	* 0.5f;	
	Vector3 end = pos - rad		* 0.5f;
	//Graphic::GetInstance().DrawLine(start, end);
	
	CollisionParameter colpara = ModelRay(other.parameter.matrix, other.parameter.octId, start, end);
	Vector3 nor = colpara.colNormal;
	if (!colpara.colFlag)return colpara;

	while (true){
		pos = pos - rad * 0.1f;
		start = pos + rad	* 0.5f;
		end = pos - rad		* 0.5f;
		colpara = ModelRay(other.parameter.matrix, other.parameter.octId, start, end);
		if (!colpara.colFlag){
			colpara.colFlag = true;
			colpara.id = COL_ID::RAY_MODEL_STEP_COLL;
			colpara.colPos = pos - rad * 0.3f;
			colpara.colNormal = nor;
			break;
		}
	}

	return colpara;
}
//カプセルとモデルのあたり判定
CollisionParameter Actor::CapsuleModel(const Actor& other)const{
	Vector3 down = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix) - down;
	Vector3 end = down * 2.0f + pos;
	float len = RCVector3::length(end - pos);

	return ModelCapsule(other.parameter.matrix, other.parameter.octId, CreateCapsule(pos,end,parameter.radius));
}
//レイとモデルのあたり判定(蜘蛛版)
CollisionParameter Actor::RayModelNatural(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	float downVec = 2.0f;
	Vector3 down = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 up = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * 0.3f;
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix) +up;
	Vector3 end = down * downVec + pos;
	float len = RCVector3::length(end - pos);
	colpara = ModelRay(other.parameter.matrix, other.parameter.octId, pos, end);

	if (colpara.colFlag){
	float zure = 0.2f;
	float rad =0.45f;
	Vector3 down1 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 up1 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 pos1 = RCMatrix4::getPosition(parameter.matrix) + RCVector3::normalize(RCMatrix4::getLeft(parameter.matrix)) * 1 * rad + up;
	Vector3 end1 = pos1 + down1 * downVec + RCVector3::normalize(pos1 - pos) * zure;
	float len1 = RCVector3::length(end1 - pos);
	CollisionParameter colpara1 = ModelRay(other.parameter.matrix, other.parameter.octId, pos1, end1);

	Vector3 down2 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 up2 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 pos2 = RCMatrix4::getPosition(parameter.matrix) - RCVector3::normalize(RCMatrix4::getLeft(parameter.matrix)) * 1 * rad + up;
	Vector3 end2 = pos2 + down2 * downVec + RCVector3::normalize(pos2 - pos) * zure;
	float len2 = RCVector3::length(end2 - pos);
	CollisionParameter colpara2 = ModelRay(other.parameter.matrix, other.parameter.octId, pos2, end2);

	Vector3 down3 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 up3 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 pos3 = RCMatrix4::getPosition(parameter.matrix) + RCVector3::normalize(RCMatrix4::getFront(parameter.matrix)) * 1 * rad + up;
	Vector3 end3 = pos3 + down3 * downVec + RCVector3::normalize(pos3 - pos) * zure;
	float len3 = RCVector3::length(end3 - pos);
	CollisionParameter colpara3 = ModelRay(other.parameter.matrix, other.parameter.octId, pos3, end3);

	Vector3 down4 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 up4 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 pos4 = RCMatrix4::getPosition(parameter.matrix) - RCVector3::normalize(RCMatrix4::getFront(parameter.matrix)) * 1 * rad + up;
	Vector3 end4 = pos4 + down4 * downVec + RCVector3::normalize(pos4 - pos) * zure;
	float len4 = RCVector3::length(end4 - pos);
	CollisionParameter colpara4 = ModelRay(other.parameter.matrix, other.parameter.octId, pos4, end4);

	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
		/*Graphic::GetInstance().DrawLine(pos, end);
		Graphic::GetInstance().DrawLine(pos1, end1);
		Graphic::GetInstance().DrawLine(pos2, end2);
		Graphic::GetInstance().DrawLine(pos3, end3);
		Graphic::GetInstance().DrawLine(pos4, end4);*/
	}

	if (!colpara1.colFlag || !colpara2.colFlag || !colpara3.colFlag || !colpara4.colFlag){
		colpara.id = COL_ID::RAY_MODEL_NATURAL_COLL;
			return colpara;
		}
		Vector3 centerLeft = RCVector3::lerp(colpara1.colPos, colpara2.colPos, 0.5f);
		Vector3 centerFront = RCVector3::lerp(colpara3.colPos, colpara4.colPos, 0.5f);

		Vector3 dummyHitPos;
		down = (down + RCMatrix4::getPosition(parameter.matrix));
		if (RCVector3::length(centerFront - down) >=
			RCVector3::length(centerLeft - down)){
			dummyHitPos = centerFront;
		}
		else dummyHitPos = centerLeft;
		if (RCVector3::length(colpara.colPos - down) >= RCVector3::length(dummyHitPos - down)){
			dummyHitPos = colpara.colPos;
		}
		colpara.colPos = dummyHitPos;
		Vector3 left = RCVector3::normalize(colpara1.colPos - colpara2.colPos);
		Vector3 front =RCVector3::normalize(colpara3.colPos - colpara4.colPos);

		colpara.colNormal = -RCVector3::cross(left, front);
	}
	colpara.id = COL_ID::RAY_MODEL_NATURAL_COLL;
	return colpara;
}

//レイとモデルのあたり判定(蜘蛛版)
CollisionParameter Actor::RayModelNaturalTest(Vector3 pos_)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	float downVec = 2.0f;
	Vector3 down = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
	Vector3 up = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * 0.3f;
	Vector3 pos = pos_ + up;
	Vector3 end = down * downVec + pos;
	float len = RCVector3::length(end - pos);

	float size2 = 3.0f;
	Matrix4 stageMat = RCMatrix4::matrix(
		vector3(size2, size2, size2),
		0.0f,
		0,
		0.0f,
		vector3(-1, -2.0f, 0));

	colpara = ModelRay(stageMat, OCT_ID::STAGE_OCT, pos, end);

	if (colpara.colFlag){
		float zure = 0.2f;
		float rad = 0.45f;
		Vector3 down1 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 up1 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 pos1 = pos_ + RCVector3::normalize(RCMatrix4::getLeft(parameter.matrix)) * 1 * rad + up;
		Vector3 end1 = pos1 + down1 * downVec + RCVector3::normalize(pos1 - pos) * zure;
		float len1 = RCVector3::length(end1 - pos);
		CollisionParameter colpara1 = ModelRay(stageMat, OCT_ID::STAGE_OCT, pos1, end1);

		Vector3 down2 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 up2 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 pos2 = pos_ - RCVector3::normalize(RCMatrix4::getLeft(parameter.matrix)) * 1 * rad + up;
		Vector3 end2 = pos2 + down2 * downVec + RCVector3::normalize(pos2 - pos) * zure;
		float len2 = RCVector3::length(end2 - pos);
		CollisionParameter colpara2 = ModelRay(stageMat, OCT_ID::STAGE_OCT, pos2, end2);

		Vector3 down3 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 up3 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 pos3 = pos_ + RCVector3::normalize(RCMatrix4::getFront(parameter.matrix)) * 1 * rad + up;
		Vector3 end3 = pos3 + down3 * downVec + RCVector3::normalize(pos3 - pos) * zure;
		float len3 = RCVector3::length(end3 - pos);
		CollisionParameter colpara3 = ModelRay(stageMat, OCT_ID::STAGE_OCT, pos3, end3);

		Vector3 down4 = -RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 up4 = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		Vector3 pos4 = pos_ - RCVector3::normalize(RCMatrix4::getFront(parameter.matrix)) * 1 * rad + up;
		Vector3 end4 = pos4 + down4 * downVec + RCVector3::normalize(pos4 - pos) * zure;
		float len4 = RCVector3::length(end4 - pos);
		CollisionParameter colpara4 = ModelRay(stageMat, OCT_ID::STAGE_OCT, pos4, end4);

		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
			/*Graphic::GetInstance().DrawLine(pos, end);
			Graphic::GetInstance().DrawLine(pos1, end1);
			Graphic::GetInstance().DrawLine(pos2, end2);
			Graphic::GetInstance().DrawLine(pos3, end3);
			Graphic::GetInstance().DrawLine(pos4, end4);*/
		}

		if (!colpara1.colFlag || !colpara2.colFlag || !colpara3.colFlag || !colpara4.colFlag){
			colpara.id = COL_ID::RAY_MODEL_NATURAL_COLL;
			return colpara;
		}
		Vector3 centerLeft = RCVector3::lerp(colpara1.colPos, colpara2.colPos, 0.5f);
		Vector3 centerFront = RCVector3::lerp(colpara3.colPos, colpara4.colPos, 0.5f);

		Vector3 dummyHitPos;
		down = (down + pos_);
		if (RCVector3::length(centerFront - down) >=
			RCVector3::length(centerLeft - down)){
			dummyHitPos = centerFront;
		}
		else dummyHitPos = centerLeft;
		if (RCVector3::length(colpara.colPos - down) >= RCVector3::length(dummyHitPos - down)){
			dummyHitPos = colpara.colPos;
		}
		colpara.colPos = dummyHitPos;
		Vector3 left = RCVector3::normalize(colpara1.colPos - colpara2.colPos);
		Vector3 front = RCVector3::normalize(colpara3.colPos - colpara4.colPos);

		colpara.colNormal = -RCVector3::cross(left, front);
	}
	colpara.id = COL_ID::RAY_MODEL_NATURAL_COLL;
	return colpara;
}

//球とモデルのあたり判定
CollisionParameter Actor::SphereModel(const Actor& other)const{
	float r = RCVector3::length(parameter.moveVec);
	if (r < parameter.radius) r = parameter.radius;
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix) + RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * r;
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LSHIFT))
	//	Graphic::GetInstance().DrawSphere(pos, r, CAMERA_ID::PLAYER_CAMERA);
	CollisionParameter colpara = ModelSphere(other.parameter.matrix, other.parameter.octId, pos, r);
	while (colpara.colFlag){
		pos -= RCVector3::normalize(parameter.moveVec) * 0.1f;
		colpara = ModelSphere(other.parameter.matrix, other.parameter.octId, pos, r);
	}
	colpara.colPos = pos;
	colpara.id = COL_ID::SPHERE_MODEL_COLL;
	return colpara;
}
//球とモデルのあたり判定(蜘蛛版)
CollisionParameter Actor::SphereModelNatural(const Actor& other)const{
	float r = RCVector3::length(parameter.moveVec) ;
	if (r < parameter.radius) r = parameter.radius;
	r += 0.1f;
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix) + RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * r;
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LSHIFT))
	//Graphic::GetInstance().DrawSphere(pos, r,CAMERA_ID::PLAYER_CAMERA);
	CollisionParameter colpara = ModelSphere(other.parameter.matrix, other.parameter.octId, pos, r);
	colpara.id = COL_ID::SPHERE_MODEL_NATURAL_COLL;
	return colpara;
}
//球とモデルのあたり判定(蜘蛛版)
CollisionParameter Actor::SphereModelThreadWeb(const Actor& other)const{
	float r = parameter.radius / 3.0f;
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix);
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LSHIFT))
	//Graphic::GetInstance().DrawSphere(pos, r,CAMERA_ID::PLAYER_CAMERA);
	CollisionParameter colpara = ModelSphere(other.parameter.matrix, other.parameter.octId, pos, r);
	colpara.id = COL_ID::SPHERE_MODEL_THREADWEB_COLL;
	return colpara;
}

//ＯＢＢとＯＢＢのあたり判定
CollisionParameter Actor::OBBOBB( const Actor& other)const{
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R)){
		/*Graphic::GetInstance().DrawCube(parameter.matrix, parameter.cubeScale);
		Graphic::GetInstance().DrawCube(other.parameter.matrix, other.parameter.cubeScale);*/
	}
	CollisionParameter colpara;
	OBB o1 = CreateOBB(parameter.matrix, parameter.cubeScale);
	OBB o2 = CreateOBB(other.parameter.matrix, other.parameter.cubeScale);
	colpara.colFlag = OBBvsOBB(o1, o2);
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	return colpara;
}
//プレイヤーと糸のあたり判定
CollisionParameter Actor::SphereLine(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);
	
	//otherが糸かどうかを調べる
	//糸でなければ即終了
	ThreadParameter tparam;
	if (other.parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR || other.parameter.id == ACTOR_ID::ENEMY_THREAD_ACTOR)
		tparam = static_cast<Thread*>(const_cast<Actor*>(&other))->GetThreadParameter();
	else if (other.parameter.id == ACTOR_ID::PLAYER_THREADBULLET_ACTOR || other.parameter.id == ACTOR_ID::ENEMY_THREADBULLET_ACTOR)
		tparam = static_cast<ThreadBullet*>(const_cast<Actor*>(&other))->GetThreadParameter();

	colpara = IsCollideLineToSphere(tparam.startPosition, tparam.currentEndPosition, RCMatrix4::getPosition(parameter.matrix), parameter.radius);
	colpara.id = COL_ID::SPHERE_LINE_COLL;
	colpara.enemyFlag = false;
	if (colpara.colFlag &&
		((parameter.id == ACTOR_ID::PLAYER_ACTOR &&  other.parameter.id == ACTOR_ID::ENEMY_THREAD_ACTOR) ||
		(parameter.id == ACTOR_ID::ENEMY_ACTOR &&  other.parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR)))
		colpara.enemyFlag = true;


	return colpara;
}
//プレイヤーと糸のあたり判定
CollisionParameter Actor::LineSphere(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	//colpara.colPos = vector3(0, 0, 0);
	//colpara.colNormal = vector3(0, 1, 0);
	//
	////otherが糸かどうかを調べる
	////糸でなければ即終了
	//ThreadParameter tparam;
	//tparam = static_cast<ThreadBullet*>(const_cast<Actor*>(this))->GetThreadParameter();
	//colpara = IsCollideLineToSphere(tparam.startPosition, tparam.currentEndPosition, RCMatrix4::getPosition(other.parameter.matrix), other.parameter.radius);
	//colpara.id = COL_ID::LINE_SPHERE_COLL;
	//colpara.enemyFlag = false;
	//if (colpara.colFlag &&
	//	((parameter.id == ACTOR_ID::PLAYER_ACTOR &&  other.parameter.id == ACTOR_ID::ENEMY_THREAD_ACTOR) ||
	//	(parameter.id == ACTOR_ID::ENEMY_ACTOR &&  other.parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR)))
	//	colpara.enemyFlag = true;

	return colpara;
}
//プレイヤーと糸の末端のあたり判定
CollisionParameter Actor::SphereLineEnd(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	//otherが糸かどうかを調べる
	//糸でなければ即終了
	if (other.parameter.id != ACTOR_ID::PLAYER_THREAD_ACTOR && other.parameter.id != ACTOR_ID::ENEMY_THREAD_ACTOR) return colpara;
	ThreadParameter tparam = static_cast<Thread*>(const_cast<Actor*>(&other))->GetThreadParameter();

	colpara = IsCollideLineToSphere(tparam.startPosition, tparam.endPosition, RCMatrix4::getPosition(parameter.matrix), parameter.radius);
	colpara.id = COL_ID::SPHERE_LINE_END_COLL;
	colpara.enemyFlag = false;
	if (colpara.colFlag &&
		((parameter.id == ACTOR_ID::PLAYER_ACTOR &&  other.parameter.id == ACTOR_ID::ENEMY_THREAD_ACTOR) ||
		(parameter.id == ACTOR_ID::ENEMY_ACTOR &&  other.parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR)))
		colpara.enemyFlag = true;

	return colpara;
}
//プレイヤーと糸のあたり判定（ジャンプ中）
CollisionParameter Actor::SphereLineJump(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	//otherが糸かどうかを調べる
	//糸でなければ即終了
	if (other.parameter.id != ACTOR_ID::PLAYER_THREAD_ACTOR && other.parameter.id != ACTOR_ID::ENEMY_THREAD_ACTOR) return colpara;
	ThreadParameter tparam = static_cast<Thread*>(const_cast<Actor*>(&other))->GetThreadParameter();

	colpara = IsCollideLineToSphere(tparam.startPosition, tparam.endPosition, RCMatrix4::getPosition(parameter.matrix) + parameter.moveVec, parameter.radius);
	colpara.id = COL_ID::SPHERE_LINE_JUMP_COLL;
	return colpara;
}
//プレイヤーと蜘蛛の巣のあたり判定
CollisionParameter Actor::TriangleLine(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	if (other.parameter.id != ACTOR_ID::PLAYER_THREADWEB_ACTOR && other.parameter.id != ACTOR_ID::ENEMY_THREADWEB_ACTOR) return colpara;
	ThreadWeb* web = static_cast<ThreadWeb*>(const_cast<Actor*>(&other));
	ThreadWebParameter webParam = web->GetThreadWebParameter();

	float rad = parameter.radius - 50.0f;
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix);
	Vector3 up = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * 0.5f;
	Vector3 down = RCVector3::normalize(-RCMatrix4::getUp(parameter.matrix)) * 0.5f;

	//Graphic::GetInstance().DrawLine(pos + up, pos + down);

	colpara = IsCollideTriangleLine(webParam.a, webParam.b, webParam.c, pos + up, pos + down);
	colpara.id = COL_ID::TRIANGLE_LINE_COLL;

	colpara.enemyFlag = false;
	if (colpara.colFlag &&
		((parameter.id == ACTOR_ID::PLAYER_ACTOR &&  other.parameter.id == ACTOR_ID::ENEMY_THREADWEB_ACTOR) ||
		(parameter.id == ACTOR_ID::ENEMY_ACTOR &&  other.parameter.id == ACTOR_ID::PLAYER_THREADWEB_ACTOR)))
		colpara.enemyFlag = true;
	return colpara;
}

//プレイヤーと蜘蛛の巣のあたり判定
CollisionParameter Actor::TriangleSphere(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	if (other.parameter.id != ACTOR_ID::PLAYER_THREADWEB_ACTOR && other.parameter.id != ACTOR_ID::ENEMY_THREADWEB_ACTOR) return colpara;
	ThreadWeb* web = static_cast<ThreadWeb*>(const_cast<Actor*>(&other));
	ThreadWebParameter webParam = web->GetThreadWebParameter();

	float rad = parameter.radius - 50.0f;
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix);
	Vector3 up = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * 0.5f;
	Vector3 down = RCVector3::normalize(-RCMatrix4::getUp(parameter.matrix)) * 0.5f;

	//Graphic::GetInstance().DrawLine(pos + up, pos + down);
	colpara = IsCollideTriangleSphere(webParam.a, webParam.b, webParam.c, pos, parameter.radius);
	colpara.enemyFlag = false;
	if (((parameter.id == ACTOR_ID::PLAYER_ACTOR &&  other.parameter.id == ACTOR_ID::ENEMY_THREADWEB_ACTOR) ||
		(parameter.id == ACTOR_ID::ENEMY_ACTOR &&  other.parameter.id == ACTOR_ID::PLAYER_THREADWEB_ACTOR)))
		colpara.enemyFlag = true;
	//if (colpara.enemyFlag)
	{
		Vector3 nor = -RCVector3::CreateNormal(webParam.a, webParam.b, webParam.c);
		if (!colpara.colFlag){
			colpara = IsCollideLineToSphere(webParam.a, webParam.b, pos, parameter.radius, -nor);
		}
		if (!colpara.colFlag){
			colpara = IsCollideLineToSphere(webParam.b, webParam.c, pos, parameter.radius, -nor);
		}
		if (!colpara.colFlag){
			colpara = IsCollideLineToSphere(webParam.c, webParam.a, pos, parameter.radius, -nor);
		}
	}
	//Graphic::GetInstance().DrawSphere(pos, parameter.radius, CAMERA_ID::PLAYER_CAMERA);
	colpara.id = COL_ID::TRIANGLE_SPHERE_COLL;

	return colpara;
}

//糸と蜘蛛の巣のあたり判定
CollisionParameter Actor::TriangleLine_T(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	if (other.parameter.id != ACTOR_ID::PLAYER_THREADWEB_ACTOR && other.parameter.id != ACTOR_ID::ENEMY_THREADWEB_ACTOR) return colpara;
	ThreadWeb* web = static_cast<ThreadWeb*>(const_cast<Actor*>(&other));
	ThreadWebParameter webParam = web->GetThreadWebParameter();

	ThreadParameter tparam = static_cast<Thread*>(const_cast<Actor*>(this))->GetThreadParameter();
	Vector3 up = tparam.startPosition;
	Vector3 down = tparam.currentEndPosition + RCVector3::normalize(tparam.currentEndPosition - tparam.startPosition) * 0.5f;

	colpara = IsCollideTriangleLine(webParam.a, webParam.b, webParam.c, up, down);
	colpara.id = COL_ID::TRIANGLE_LINE_T_COLL;

	colpara.enemyFlag = false;
	if (colpara.colFlag &&
		((parameter.id == ACTOR_ID::PLAYER_THREAD_ACTOR &&  other.parameter.id == ACTOR_ID::ENEMY_THREADWEB_ACTOR) ||
		(parameter.id == ACTOR_ID::ENEMY_THREAD_ACTOR &&  other.parameter.id == ACTOR_ID::PLAYER_THREADWEB_ACTOR)))
		colpara.enemyFlag = true;

	return colpara;
}

//プレイヤーと蜘蛛の巣のあたり判定（ジャンプ中）
CollisionParameter Actor::TriangleLineJump(const Actor& other)const{
	CollisionParameter colpara;
	colpara.colFlag = false;
	colpara.colPos = vector3(0, 0, 0);
	colpara.colNormal = vector3(0, 1, 0);

	if (other.parameter.id != ACTOR_ID::PLAYER_THREADWEB_ACTOR && other.parameter.id != ACTOR_ID::ENEMY_THREADWEB_ACTOR) return colpara;
	ThreadWeb* web = static_cast<ThreadWeb*>(const_cast<Actor*>(&other));
	ThreadWebParameter webParam = web->GetThreadWebParameter();

	float rad = parameter.radius - 50.0f;
	Vector3 pos = RCMatrix4::getPosition(parameter.matrix);
	Vector3 up = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * 0.3f;

	//Graphic::GetInstance().DrawLine(pos + up, pos + parameter.moveVec);

	colpara = IsCollideTriangleLine(webParam.a, webParam.b, webParam.c,
		pos, pos + parameter.moveVec);
	colpara.id = COL_ID::TRIANGLE_LINE_JUMP_COLL;
	return colpara;
}

//球とモデルのあたり判定(プレイヤー用、移動方向の先に判定球を置く)
CollisionParameter Actor::SphereModelMove(const Actor& other)const{
	Vector3 sphere = RCMatrix4::getPosition(parameter.matrix) + parameter.moveVec;
	/*if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_R))
		Graphic::GetInstance().DrawSphere(sphere, parameter.radius, vector3(1, 0, 0));*/
	CollisionParameter colpara = ModelSphere(other.parameter.matrix, other.parameter.octId, sphere, parameter.radius);
	colpara.id = COL_ID::SPHERE_MODEL_MOVE_COLL;
	return colpara;
}

////レイとモデルのあたり判定(プレイヤー用、糸の上にいるとき)
//CollisionParameter Actor::RayModelOnThread(const Actor& other)const{
//	Vector3 pos = RCMatrix4::getPosition(parameter.matrix);
//	float raylength = 1.0f;
//	Vector3 down = RCVector3::normalize(parameter.moveVec);
//	Vector3 end = down * raylength + pos;
//
//	Graphic::GetInstance().DrawLine(pos, end, vector3(0, 1, 1));
//	CollisionParameter colpara = ModelRay(other.parameter.matrix, other.parameter.octId, pos, end);
//	colpara.id = COL_ID::RAY_MODEL_ONTHREAD_COLL;
//
//	return colpara;
//}