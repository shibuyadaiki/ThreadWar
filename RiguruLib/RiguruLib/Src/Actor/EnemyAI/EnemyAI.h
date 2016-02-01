#pragma once
#include "../../world/IWorld.h"
#include "../Actor.h"
#include "../ID.h"
class Player;
class PlayerActionManager;


enum AI_STATE{
	FIRST_AI,
	SECOND_AI,
	THERD_AI,
};

class EnemyAI{
public:
	EnemyAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_) :
		world(wo), action(&action), player(player), cID(cID_), parameter(parameter_){}
	virtual ~EnemyAI(){}
	virtual void Initialize() = 0;
	virtual Vector3 UpdateAI(float frameTime) = 0;
	virtual void DrawAI() const = 0;
	virtual AI_STATE NextAI(){ return nextAI; };

protected:
	IWorld& world;
	PlayerActionManager* action;
	std::weak_ptr<Player> player;
	CAMERA_ID& cID;
	ActorParameter& parameter;
	AI_STATE nextAI;
	TEAM_ID teamID;
};
