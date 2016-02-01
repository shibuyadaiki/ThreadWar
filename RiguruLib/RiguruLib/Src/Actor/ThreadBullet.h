#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include "Actor.h"
#include <list>
#include <memory>
#include "ThreadParamter.h"

class Thread;
class Player;
class Stage;
class ThreadBullet :public Actor, public std::enable_shared_from_this<ThreadBullet>{
public:
	ThreadBullet(IWorld& world_,std::weak_ptr<Stage> stage_, ACTOR_ID thread_id, Vector3& startPos, Vector3& currentPos,Vector3 endPos, std::weak_ptr<Thread> rootThread_, CAMERA_ID cID);
	~ThreadBullet();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	ThreadParameter GetThreadParameter(){ return threadParam; }
private:
	Vector3& pos,startPos;
	std::weak_ptr<Stage> stage;
	std::list<std::weak_ptr<Player>> hitPlayer;
	std::weak_ptr<Thread> rootThread;
	ThreadParameter threadParam;
};