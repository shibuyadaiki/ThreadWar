#include "World.h"
#include "../actor/ID.h"

World::World(){

}

World::~World(){
	Clear();
}

void World::Initialize(){
	Clear();
}
void World::Update(float frameTime){
	actors.Update(frameTime);
}
void World::Draw(CAMERA_ID cID) const{
	actors.Draw(cID);
}
void World::Clear(){
	actors.Clear();
}

void World::Add(ACTOR_ID id, ActorPtr actor){
	if (id == ACTOR_ID::PLAYER_ACTOR)
		player = actor;
	else if (id == ACTOR_ID::ENEMY_ACTOR)
		enemy = actor;

	actors.Add(id, actor);
}

void World::Add(ActorPtr actor){
	actors.Add(ACTOR_ID::PLAYER_ACTOR, actor);
}

bool World::IsEnd()const{
	//とりあえず
	return false;
}

void World::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID){
	actors.SetCollideSelect(thisActor, otherID, colID);
}

//子オブジェクトを巡回
void World::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func)
{
	actors.EachActor(id, func);
}

ActorPtr World::GetPlayer(){
	return player;
}
ActorPtr World::GetEnemy(){
	return enemy;
}