#pragma once

#include"ActorPtr.h"
#include<list>//�V�[�P���X�R���e�ilist����������
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
	//�q������
	void EachActor(std::function<void(Actor&)> func);
	//���̖{������(�����F�����{��)
	void DeleteOldThread(int limit);
private:
	std::list<ActorPtr> actorPtr;
};