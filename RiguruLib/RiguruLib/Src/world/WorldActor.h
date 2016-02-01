#pragma once
#include "../actor/ActorPtr.h"
#include "../actor/ActorManager.h"
#include <map>
#include <vector>

enum ACTOR_ID;

struct CollideSelect{
	ACTOR_ID otherID;
	COL_ID colID;
};

class WorldActor{
public:
	WorldActor();
	~WorldActor();
	void Update(float frameTime);
	void Draw(CAMERA_ID cID) const;
	void Add(ACTOR_ID id, ActorPtr actor);
	void Clear();
	void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);

	//子オブジェクトを巡回
	void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func);
	//糸の本数制限
	void DeleteOldThread(int limit);

private:
	typedef std::shared_ptr<ActorManager> ActorManagerPtr;
	typedef std::map<ACTOR_ID,ActorManagerPtr> ActorManagerPtrMap;
	typedef std::pair<ACTOR_ID, ActorManagerPtr> ActorManagerPair;
	ActorManagerPtrMap managers;
	std::map<ActorPtr, std::vector<CollideSelect>> colselect;
};