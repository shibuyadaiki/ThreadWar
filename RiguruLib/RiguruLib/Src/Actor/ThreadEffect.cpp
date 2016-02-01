#include "ThreadEffect.h"
#include "Player.h"
#include "../Graphic/Graphic.h"
#include "ID.h"
#include "../Math/Quaternion.h"
#include "Collision.h"

ThreadEffect::ThreadEffect(IWorld& world_, Matrix4& playerMat_, bool& bindFlag_, CAMERA_ID cID)
:Actor(world_),
playerMat(playerMat_),bindFlag(bindFlag_){
	parameter.isDead = false;
	upAngle = 0;
	scaleUp = vector3(2,2,2);

	finishFlag = false;
}

ThreadEffect::~ThreadEffect(){
}

void ThreadEffect::Initialize(){
}

void ThreadEffect::Update(float frameTime){
	upAngle += 1000.0f * frameTime;
	if (!bindFlag || finishFlag){
		scaleUp += vector3(8.0f * frameTime, 8.0f * frameTime, 8.0f * frameTime);
		finishFlag = true;
	}
	if (scaleUp.x >= 3.2f){
		parameter.isDead = true;
	}
}

void ThreadEffect::Draw(CAMERA_ID cID) const{
	//プレイヤー用のシェーダーセット
	Graphic::GetInstance().SetShader(SHADER_ID::THREAD_EFFECT_SHADER);
	//テクニックとパスをセット
	Graphic::GetInstance().SetTechniquePass(SHADER_ID::THREAD_EFFECT_SHADER, "TShader", "P1");
	Matrix4 mat = (playerMat);
	Matrix4 rotate = RCMatrix4::Identity();
	RCMatrix4::setLeft(rotate, RCMatrix4::getLeft(mat));
	RCMatrix4::setFront(rotate, RCMatrix4::getFront(mat));
	RCMatrix4::setUp(rotate, RCMatrix4::getUp(mat));

	Matrix4 rotateF = rotate * RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getFront((playerMat))), 30);
	rotateF *= RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getLeft((playerMat))), 30);

	Matrix4 upRotate = RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getUp((rotateF))), upAngle);
	Matrix4 trans = RCMatrix4::translate(RCMatrix4::getPosition(mat) + RCVector3::normalize(RCMatrix4::getUp(playerMat)) * 0.2f);

	mat = RCMatrix4::scale(scaleUp) * rotateF * upRotate * trans;
	//プレイヤーを描画
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, 0, true);
	
	rotateF = rotate * RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getFront((playerMat))), -30);
	rotateF *= RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getLeft((playerMat))), -15);
	upRotate = RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getUp((rotateF))), upAngle);
	//プレイヤーを描画
	mat = RCMatrix4::scale(scaleUp * 0.85f) * rotateF * upRotate * trans;
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, 0, true);
	

	rotateF = rotate * RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getFront((playerMat))), 0);
	rotateF *= RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getLeft((playerMat))), 90);

	upRotate = RCQuaternion::rotate(RCVector3::normalize(RCMatrix4::getUp((rotateF))), -upAngle);
	mat = RCMatrix4::scale(scaleUp * 0.85f * 0.85f) * rotateF * upRotate * trans;
	//プレイヤーを描画
	Graphic::GetInstance().DrawMesh(MODEL_ID::THREAD_EFFECT_MODEL, &mat, cID, 0, true);
}

void ThreadEffect::OnCollide(Actor& other, CollisionParameter colpara){
}