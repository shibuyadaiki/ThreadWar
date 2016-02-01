#include "ThreadBullet.h"
#include "Thread.h"
#include "Player.h"
#include "../Graphic/Graphic.h"
#include "Stage.h"
#include "Collision.h"
ThreadBullet::ThreadBullet(IWorld& world_, std::weak_ptr<Stage> stage_, ACTOR_ID thread_id, Vector3& startPos_, Vector3& currentPos, Vector3 endPos, std::weak_ptr<Thread> rootThread_, CAMERA_ID cID)
:Actor(world_), pos(currentPos), rootThread(rootThread_), startPos(startPos_), stage(stage_)
{
	threadParam.endPosition = endPos;
	threadParam.startPosition = startPos;
	threadParam.currentEndPosition = pos;
	//各種パラメーターに値をセット
	parameter.isDead = false;
	parameter.id = ACTOR_ID::PLAYER_THREADBULLET_ACTOR;
	if (thread_id == ACTOR_ID::ENEMY_THREAD_ACTOR)parameter.id = ACTOR_ID::ENEMY_THREADBULLET_ACTOR;
	parameter.matrix = RCMatrix4::translate(pos);
	parameter.radius = 0.25f;
}

ThreadBullet::~ThreadBullet(){
	hitPlayer.clear();
}

void ThreadBullet::Initialize(){

}

void ThreadBullet::Update(float frameTime){
	threadParam.currentEndPosition = pos;
	if ((!rootThread._Get()->IsShot() || rootThread.lock() == nullptr)){
		parameter.isDead = true;
		Vector3 hitPos = threadParam.endPosition;
		Vector3 start = vector3(-2.4f, -1.3f, -1.5f);
		Vector3 end =vector3(0.5f, 5.5f, 1.5f);      
		if (start.x <= hitPos.x && start.y <= hitPos.y && start.z <= hitPos.z && hitPos.x <= end.x&& hitPos.y <= end.y&& hitPos.z <= end.z){
			if (!rootThread._Get()->ThreadHit())
			stage._Get()->CrystalDamage(parameter.id, ACTOR_ID::CRYSTAL_CENTER_ACTOR);

		}
		start = vector3(-2.4f, -1.3f, 62.0f);
		end = vector3(0.5f, 5.5f, 65.3f);
		if (start.x <= hitPos.x && start.y <= hitPos.y && start.z <= hitPos.z && hitPos.x <= end.x&& hitPos.y <= end.y&& hitPos.z <= end.z){
			if (!rootThread._Get()->ThreadHit())
			stage._Get()->CrystalDamage(parameter.id, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR);

		}
		start = vector3(-2.4f, -1.3f, -67.0f);
		end = vector3(0.5f, 5.5f, -64.0f);
		if (start.x <= hitPos.x && start.y <= hitPos.y && start.z <= hitPos.z && hitPos.x <= end.x&& hitPos.y <= end.y&& hitPos.z <= end.z){
			if (!rootThread._Get()->ThreadHit())
			stage._Get()->CrystalDamage(parameter.id, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR);

		}
	}
	parameter.matrix = RCMatrix4::translate(pos);
	parameter.lastNor = pos;
}

void ThreadBullet::Draw(CAMERA_ID cID) const{
	Graphic::GetInstance().DrawSphere(threadParam.currentEndPosition, 0.25f, cID,parameter.id == ACTOR_ID::PLAYER_THREADBULLET_ACTOR ? vector3(1,0,0) : vector3(0,0,1));
}

void ThreadBullet::OnCollide(Actor& other, CollisionParameter colpara){
	bool beforeHit = false;
	for (auto i : hitPlayer){
		if (i._Get() == &other){
			beforeHit = true;
		}
	}
	if (!beforeHit && !rootThread._Get()->ThreadHit()){
		static_cast<Player*>(&other)->Damage(1.0f);
		static_cast<Player*>(&other)->SetBindTime(0.5f);
		hitPlayer.push_back(static_cast<Player*>(&other)->shared_from_this());
		rootThread._Get()->SetIsDead(true);
		this->SetIsDead(true);
	}
}