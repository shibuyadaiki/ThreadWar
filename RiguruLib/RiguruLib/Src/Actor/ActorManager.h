#pragma once

#include"ActorPtr.h"
#include<list>//シーケンスコンテナlistを実装する
#include <functional>
enum CAMERA_ID;
enum COL_ID;
class ActorManager
{
public:
	ActorManager();
	~ActorManager();
	void Update(float frameTime);
	void Draw(CAMERA_ID cID) const;
	void Add(ActorPtr actor);
	void Clear();
	void Remove();
	void Collide(COL_ID id, Actor& other);
	void Collide(COL_ID id, ActorManager& other);
	//子を巡回
	void EachActor(std::function<void(Actor&)> func);
	//糸の本数制限(引数：制限本数)
	void DeleteOldThread(int limit);
private:
	std::list<ActorPtr> actorPtr;
};