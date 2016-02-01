#include "CrystalCenter.h"
#include  "../Other/Device.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtility.h"
#include "../Graphic/Graphic.h"
#include "ID.h"
#include <string>
#include "Collision.h"

CrystalCenter::CrystalCenter(IWorld& world_,ACTOR_ID crystalID) :Actor(world_){
	parameter.isDead = false;
	parameter.id = crystalID;
	parameter.cubeScale = vector3(10.0f, 3.0f, 10.0f);
	parameter.radius = 1.0f;
	float size2 = 3.0f;
	parameter.matrix = RCMatrix4::matrix(
		vector3(size2, size2, size2),
		0.0f,
		0,
		0.0f,
		vector3(-1, -2.0f, 0));

	point = 0;

	crystalParameter.crystalISEnemy = false;
	crystalParameter.crystalISPlayer = false;
	keep = false;
	angle = 0;
	scaleStart = vector3(0, 0, 0);
	startFlag = false;
}

CrystalCenter::~CrystalCenter(){

}
void CrystalCenter::Initialize(){

}
void CrystalCenter::Update(float frameTime){
	if (point >= 10){
		crystalParameter.crystalISPlayer = true;
		keep = true;
	}
	else if (point <= -10){
		crystalParameter.crystalISEnemy = true;
		keep = true;
	}
	
	if (keep){
		if(crystalParameter.crystalISPlayer && point <= 0)
		{
			crystalParameter.crystalISPlayer = false;
			keep = false;
		}
		else if(crystalParameter.crystalISEnemy && point >= 0)
		{
			crystalParameter.crystalISEnemy = false;
			keep = false;
		}
	}

	point = min(10, point);
	point = max(-10, point);
	if (crystalParameter.crystalISEnemy || crystalParameter.crystalISPlayer){
		angle += 1000.0f * frameTime;
		startFlag = true;
	}
	else{
		angle = 0.0f;
		startFlag = false;
	}

	if (startFlag){
		scaleStart += vector3(3.0f, 3.0f, 3.0f) * frameTime * 2.0f / 3.0f;
		if (scaleStart.x > 1.0f){
			scaleStart = vector3(1, 1, 1);
		}
	}
	else{
		scaleStart = vector3(0, 0, 0);
	}
}

void CrystalCenter::Draw(CAMERA_ID cID) const{
	Graphic::GetInstance().SetShader(SHADER_ID::CRYSTAL_SHADER);

	Vector3 color = vector3(1, 1, 1);

	if (point > 0)color = vector3(1, 1.0f - 0.1f * point, 1.0f - 0.1f * point);
	else if (point < 0)color = vector3(1.0f - 0.1f * abs(point), 1.0f - 0.1f * abs(point), 1);
	if (crystalParameter.crystalISPlayer)color = vector3(1, 0, 0);
	if (crystalParameter.crystalISEnemy)color = vector3(0, 0, 1);
	if (parameter.id == ACTOR_ID::CRYSTAL_CENTER_ACTOR){
		Graphic::GetInstance().DrawMesh(MODEL_ID::CRYSTAL_CENTER_MODEL, &parameter.matrix, cID, &D3DXCOLOR(color.x, color.y, color.z, 1), true);
		if (crystalParameter.crystalISEnemy || crystalParameter.crystalISPlayer)
			DrawEffect(vector3(-2.4f, -1.3f, -1.5f) + (vector3(0.5f, 5.5f, 1.5f) - vector3(-2.4f, -1.3f, -1.5f)) * 0.5f, scaleStart, cID, color - vector3(-0.5f, -0.5f, -0.5f));
		//	Graphic::GetInstance().DrawCube(vector3(-2.4f, -1.3f, -1.5f), vector3(0.5f, 5.5f, 1.5f), cID);
	}
	else if (parameter.id == ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR){
		Graphic::GetInstance().DrawMesh(MODEL_ID::CRYSTAL_PLAYERSIDE_MODEL, &parameter.matrix, cID, &D3DXCOLOR(color.x, color.y, color.z, 1), true);
		if (crystalParameter.crystalISEnemy || crystalParameter.crystalISPlayer)
			DrawEffect(vector3(-2.4f, -1.3f, 62.0f) + (vector3(0.5f, 5.5f, 65.3f) - vector3(-2.4f, -1.3f, 62.0f)) * 0.5f, scaleStart, cID, color - vector3(-0.5f, -0.5f, -0.5f));
		//	Graphic::GetInstance().DrawCube(vector3(-2.4f, -1.3f, 62.0f), vector3(0.5f, 5.5f, 65.3f), cID);

	}
	else if (parameter.id == ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR){
		Graphic::GetInstance().DrawMesh(MODEL_ID::CRYSTAL_ENEMYSIDE_MODEL, &parameter.matrix, cID, &D3DXCOLOR(color.x, color.y, color.z, 1), true);
		if (crystalParameter.crystalISEnemy || crystalParameter.crystalISPlayer)
			DrawEffect(vector3(-2.4f, -1.3f, -67.0f) + (vector3(0.5f, 5.5f, -64.0f) - vector3(-2.4f, -1.3f, -67.0f)) * 0.5f, scaleStart, cID, color - vector3(-0.5f, -0.5f, -0.5f));
		//	Graphic::GetInstance().DrawCube(vector3(-2.4f, -1.3f, -67.0f), vector3(0.5f, 5.5f, -64.0f), cID);
	}
}

void CrystalCenter::DrawEffect(Vector3 pos, Vector3 scale_, CAMERA_ID cID, Vector3 color) const{
	Matrix4 scale, angleMat, trans;
	scale = RCMatrix4::scale(scale_);
	trans = RCMatrix4::translate(pos);
	angleMat = RCMatrix4::rotateZ(15) * RCMatrix4::rotateX(15);
	Matrix4 mat = scale * angleMat * RCMatrix4::rotateY(angle) * trans;


	//プレイヤー用のシェーダーセット
	Graphic::GetInstance().SetShader(SHADER_ID::THREAD_EFFECT_SHADER);
	//テクニックとパスをセット
	Graphic::GetInstance().SetTechniquePass(SHADER_ID::THREAD_EFFECT_SHADER, "TShader", "P2");
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, &D3DXCOLOR(color.x, color.y, color.z, 0.5f), true);
	angleMat = RCMatrix4::rotateZ(angle) * RCMatrix4::rotateX(-15);
	trans = RCMatrix4::translate(pos + vector3(0, 1.5f, 0));
	scale = RCMatrix4::scale(scale_ * 0.85f);
	mat = scale * angleMat * RCMatrix4::rotateY(-angle) * trans;
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, &D3DXCOLOR(color.x, color.y, color.z, 0.5f), true);
	angleMat = RCMatrix4::rotateZ(5) * RCMatrix4::rotateX(-angle);
	trans = RCMatrix4::translate(pos + vector3(0, -1.5f, 0));
	mat = scale * angleMat * RCMatrix4::rotateY(angle) * trans;
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, &D3DXCOLOR(color.x, color.y, color.z, 0.5f), true);
	angleMat = RCMatrix4::rotateZ(angle) * RCMatrix4::rotateX(-32);
	trans = RCMatrix4::translate(pos + vector3(0, 0.75f, 0));
	mat = scale * angleMat * RCMatrix4::rotateY(-angle) * trans;
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, &D3DXCOLOR(color.x, color.y, color.z, 0.5f), true);
	scale = RCMatrix4::scale(scale_ * 0.75f);
	angleMat = RCMatrix4::rotateZ(53) * RCMatrix4::rotateX(-angle);
	trans = RCMatrix4::translate(pos + vector3(0, -0.75f, 0));
	mat = scale * angleMat * RCMatrix4::rotateY(angle) * trans;
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, &D3DXCOLOR(color.x, color.y, color.z, 0.5f), true);
}

void CrystalCenter::OnCollide(Actor& other, CollisionParameter colpara){

}

void CrystalCenter::Damage(bool player){ 
	if (player)point++; 
	else point--;
}

bool CrystalCenter::IsPlayer(){
	return crystalParameter.crystalISPlayer;
}
bool CrystalCenter::IsEnemy(){
	return crystalParameter.crystalISEnemy;
}

void CrystalCenter::Reset()
{
	point = 0;
}