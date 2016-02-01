#include "PlayerActionManager.h"
#include "../Collision.h"
#include "Action/NormalAction.h"
#include "Action\HoverAction.h"
#include "Action\JumpAction.h"
#include "Action\StepAction.h"
#include "Action\GroundCurlAction.h"
#include "Action\AirCurlAction.h"
#include "Action\ThreadAction.h"
#include "Action\DangleAction.h"
#include "Action\RewindAction.h"
#include "Action\ThreadWebAction.h"
#include "../Player.h"
#include "../../Graphic/Graphic.h"
#include "../Thread.h"
#include "../../Math/Quaternion.h"
#include "../ThreadWeb.h"
#include "../Collision.h"
PlayerActionManager::PlayerActionManager(){
	animUpdateTime = new float(WALKANIMSPEED);
	beforeAnimBlendLevel = new float(WALKANIMBLEND);
	*animUpdateTime = 0;
	*beforeAnimBlendLevel = 0;
	animTime = 0;
	a_id = ACTION_ID::NORMAL_ACTION;
	before_id = ACTION_ID::NORMAL_ACTION;
	grabity = vector3(0, 0, 0);
}
PlayerActionManager::~PlayerActionManager(){
	SAFE_DELETE(animUpdateTime);
	SAFE_DELETE(beforeAnimBlendLevel);
}

void PlayerActionManager::Initialize(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_){
	playerAction[ACTION_ID::NORMAL_ACTION] = std::make_shared<NormalAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::HOVER_ACTION] = std::make_shared<HoverAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::JUMP_ACTION] = std::make_shared<JumpAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::STEP_ACTION] = std::make_shared<StepAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::AIR_CURL_ACTION] = std::make_shared<AirCurlAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::GROUND_CURL_ACTION] = std::make_shared<GroundCurlAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::THREAD_ACTION] = std::make_shared<ThreadAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::DANGLE_ACTION] = std::make_shared<DangleAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::REWIND_ACTION] = std::make_shared<RewindAction>(world, player_, padNum_, cID_);
	playerAction[ACTION_ID::THREAD_WEB_ACTION] = std::make_shared<ThreadWebAction>(world, player_, padNum_, cID_);

	player = player_;
}
void PlayerActionManager::ChangeAction(ACTION_ID actionID){
	playerAction[a_id]->ChangeAction(actionID);
}

void PlayerActionManager::Control(){
	playerAction[a_id]->Control();
}

void PlayerActionManager::Update(float frameTime){
	if (playerAction[a_id]->IsDead()){
		playerAction[a_id]->SetIsDead(false);
		ACTION_ID nextID = playerAction[a_id]->NextAction();
		playerAction[nextID]->GetControl();
		if (playerAction[nextID]->Initialize(a_id, playerAction[a_id]->ReturnUp())){
			playerAction[a_id]->GrabityReset(&grabity);
			before_id = a_id;
			a_id = nextID;
		}
	}
	playerAction[a_id]->PlayerActionUpdate(frameTime,&grabity);
	playerAction[a_id]->Update(frameTime);
	static_cast<StepAction*>(playerAction[ACTION_ID::STEP_ACTION]._Get())->UpdateStepDelay(frameTime);
	animTime += *animUpdateTime * frameTime;
}
void PlayerActionManager::Collide(Actor& other, CollisionParameter colpara){
	playerAction[a_id]->OnCollide(other, colpara);
}

void PlayerActionManager::SetAnimation(ANIM_ID firstActionId, ANIM_ID secondActionId, float animUpdateTime_, bool animTimeReset, bool roop, float blendLevel, float beforeAnimBlendLevel_){
	*animUpdateTime = animUpdateTime_;
	*beforeAnimBlendLevel = beforeAnimBlendLevel_;
	if (animTimeReset)animTime = 0;
	Graphic::GetInstance().SetAnimation(
		player._Get()->shared_from_this(),
		firstActionId,
		animTime,
		animUpdateTime,
		secondActionId,
		animTime,
		animUpdateTime,
		blendLevel,
		roop,
		beforeAnimBlendLevel);
}

void PlayerActionManager::SetAnimTime(float time){
	*animUpdateTime = time;
}
void PlayerActionManager::SetAnimBlend(float blend){
	*beforeAnimBlendLevel = blend;
}

PlayerControl PlayerActionManager::ReturnPlayerControl(){
	return playerAction[a_id]->ReturnPlayerControl();
}

void PlayerActionManager::SetSecondFrameFlag(bool pc){
	playerAction[a_id]->SetSecondFrameFlag(pc);
}
void PlayerActionManager::SetThread(std::weak_ptr<Thread> thread_, bool beforeThreadKill){
	if (thread._Get() != nullptr && beforeThreadKill)
	thread._Get()->SetIsDead(true);
	thread = thread_;
}
std::weak_ptr<Thread> PlayerActionManager::ReturnThread(){
	return thread;
}
void PlayerActionManager::SetThreadWeb(std::weak_ptr<ThreadWeb> threadWeb_){
	threadWeb = threadWeb_;
}
std::weak_ptr<ThreadWeb> PlayerActionManager::ReturnThreadWeb(){
	return threadWeb;
}

