#include "Base.h"
#include  "../Other/Device.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtility.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Graphic.h"
#include "ID.h"
#include <string>
#include "Collision.h"

Base::Base(IWorld& world_, Vector3 pos, TEAM_ID teamID) :Actor(world_){
	parameter.isDead = false;
	parameter.id = ACTOR_ID::BASE_ACTOR;
	parameter.cubeScale = vector3(10.0f, 3.0f, 10.0f);
	parameter.radius = 1.0f;
	yAngle = 0;
	//parameter.octId = OCT_ID::STAGE_OCT;
	float size2 = 3.0f;
	parameter.matrix = RCMatrix4::matrix(
		vector3(size2, size2, size2),
		0.0f,
		yAngle,
		0.0f,
		pos);
	this->teamID = teamID;
}

Base::~Base(){

}
void Base::Initialize(){

}
void Base::Update(float frameTime){

}

void Base::Draw(CAMERA_ID cID) const{
	Graphic::GetInstance().DrawSphere(RCMatrix4::getPosition(parameter.matrix), 4, cID);
}

void Base::OnCollide(Actor& other, CollisionParameter colpara){

}

Matrix4* Base::ReturnMat(){
	return &parameter.matrix;
}

TEAM_ID Base::GetTeamID(){
	return teamID;
}
