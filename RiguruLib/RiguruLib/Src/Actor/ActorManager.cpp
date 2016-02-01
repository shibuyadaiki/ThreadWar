#include "ActorManager.h"
#include "Actor.h"
#include "ID.h"
#include<algorithm>
#include "Thread.h"

ActorManager::ActorManager(){
}

ActorManager::~ActorManager(){

}
void ActorManager::Update(float frameTime){
	std::for_each(actorPtr.begin(), actorPtr.end(),
		[&](ActorPtr actor){ actor->Update(frameTime); });
}

void ActorManager::Draw(CAMERA_ID cID) const{
	std::for_each(actorPtr.begin(), actorPtr.end(),
		[&](ActorPtr actor){ actor->Draw(cID); });
}

void ActorManager::Add(ActorPtr actor){
	actorPtr.push_back(actor);
	actor->Initialize();
}

void ActorManager::Clear(){
	actorPtr.clear();
}
void ActorManager::Remove(){
	actorPtr.remove_if([](ActorPtr actor){return actor->IsDead(); });
}
void ActorManager::Collide(COL_ID id, Actor& other){
	std::for_each(actorPtr.begin(), actorPtr.end(),
		[&](ActorPtr actor){other.Collide(id, *actor); });
}
void ActorManager::Collide(COL_ID id, ActorManager& other){
	std::for_each(actorPtr.begin(), actorPtr.end(),
		[&](ActorPtr actor){other.Collide(id, *actor); });
}

//éqÇèÑâÒ
void ActorManager::EachActor(std::function<void(Actor&)> func)
{
	std::for_each(actorPtr.begin(), actorPtr.end(),
		[&](const ActorPtr& actor) {func(*actor); });
}

//éÖÇÃñ{êîêßå¿
void ActorManager::DeleteOldThread(int limit)
{
	int size = actorPtr.size();
	int count = 0;
	if (size > limit)
	{
		for (auto i : actorPtr){
			if (!static_cast<Thread*>(i.get())->IsRewind()){
				i->SetIsDead(true);
				count++;
			}
			if (count >= size - limit){
				break;
			}
		}
	}
}