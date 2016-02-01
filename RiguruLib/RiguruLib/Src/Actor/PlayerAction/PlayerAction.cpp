#include "PlayerAction.h"
#include "../Player.h"
#include "../../Other/Device.h"
#include "../Thread.h"
#include "../ThreadWeb.h"

PlayerAction::PlayerAction(IWorld& world_, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_,ACTION_ID id) :
world(world_),
player(player_),
padNum(padNum_),
cID(cID_),
thisAction(id){
	isDead = false;
	cameraMove = true;
	norUp = true;
	nextAction = ACTION_ID::NORMAL_ACTION;

	if (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR)
	{
		friendThreadID = ACTOR_ID::PLAYER_THREAD_ACTOR;
		friendThreadWebID = ACTOR_ID::PLAYER_THREADWEB_ACTOR;
		enemyThreadID = ACTOR_ID::ENEMY_THREAD_ACTOR;
		enemyThreadWebID = ACTOR_ID::ENEMY_THREADWEB_ACTOR;
	}
	else
	{
		friendThreadID = ACTOR_ID::ENEMY_THREAD_ACTOR;
		friendThreadWebID = ACTOR_ID::ENEMY_THREADWEB_ACTOR;
		enemyThreadID = ACTOR_ID::PLAYER_THREAD_ACTOR;
		enemyThreadWebID = ACTOR_ID::PLAYER_THREADWEB_ACTOR;
	}

	moveVecUp = vector3(0, 1, 0);
	beforeUp = vector3(0, 1, 0);
	ride = false;
	hitGround = false;
}

PlayerAction::~PlayerAction(){

}
void PlayerAction::Control(){
	ride = false;
	if (player._Get()->ReturnPlayerNum() == 0 && (thisAction == ACTION_ID::NORMAL_ACTION || (thisAction == ACTION_ID::THREAD_ACTION && !hitGround) || (thisAction == ACTION_ID::THREAD_WEB_ACTION && !hitGround))){
		Vector3 playerPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
		Vector3 cameraVec = RCVector3::normalize(Device::GetInstance().GetCamera(cID)->CameraParam()->Target - Device::GetInstance().GetCamera(cID)->CameraParam()->Eye);
		std::weak_ptr<Thread> threadWeak;
		std::weak_ptr<ThreadWeb> threadWebWeak;
		float lowAngle = 360;
		float webLengh = 10000;
		for (auto i : threadWebVec){
			threadWebWeak = i;
		}
		if (threadWebWeak._Get() != nullptr){
			threadWebWeak._Get()->SetSelectThreadWeb();

			if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) || Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R3, true)){
				if ((threadWebWeak._Get()->GetParameter().id == ACTOR_ID::PLAYER_THREADWEB_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR) ||
					(threadWebWeak._Get()->GetParameter().id == ACTOR_ID::ENEMY_THREADWEB_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::ENEMY_ACTOR)){
					ChangeAction(ACTION_ID::THREAD_WEB_ACTION);
					player._Get()->SetThreadWeb(threadWebWeak._Get()->shared_from_this());
					ride = true;
					change = true;
				}
			}
		}

		if (threadWebWeak._Get() == nullptr){
			for (auto i : threadVec){
				ThreadParameter threadParam = i._Get()->GetThreadParameter();
				Vector3 threadVec = threadParam.startNormal;
				if (RCVector3::length(threadParam.startPosition - playerPos) <
					RCVector3::length(threadParam.currentEndPosition - playerPos)){
					threadVec = threadParam.endNormal;
				}
				threadVec = RCVector3::normalize(threadVec);
				if (lowAngle > Math::acos(RCVector3::dot(threadVec, cameraVec))){
					lowAngle = Math::acos(RCVector3::dot(threadVec, cameraVec));
					threadWeak = i;
				}
			}
			if (threadWeak._Get() != nullptr){
				threadWeak._Get()->SetSelectThread();
				//ボタン入力で巻取りを開始
				if ((Device::GetInstance().GetInput()->MouseButtonDown(INPUTMOUSEBUTTON::RIGHT_BUTTON, true) ||
					Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R2, true)) &&
					thisAction != ACTION_ID::THREAD_ACTION){
					if ((threadWeak._Get()->GetParameter().id == ACTOR_ID::PLAYER_THREAD_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR) ||
						(threadWeak._Get()->GetParameter().id == ACTOR_ID::ENEMY_THREAD_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::ENEMY_ACTOR)){
						player._Get()->SetThread(threadWeak);
						ChangeAction(ACTION_ID::REWIND_ACTION);
					}
				}

				if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) || Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R3, true)){
					if ((threadWeak._Get()->GetParameter().id == ACTOR_ID::PLAYER_THREAD_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR) ||
						(threadWeak._Get()->GetParameter().id == ACTOR_ID::ENEMY_THREAD_ACTOR && player._Get()->GetParameter().id == ACTOR_ID::ENEMY_ACTOR)){
						ChangeAction(ACTION_ID::THREAD_ACTION);
						player._Get()->SetThread(threadWeak._Get()->shared_from_this());
						change = true;
						ride = true;
					}
				}
			}
		}
	}
}
void PlayerAction::PlayerActionUpdate(float frameTime, Vector3* grabity){
	
	GetControl();
	Device::GetInstance().GetCamera(cID)->SetCameraMove(cameraMove);
	if ((!player._Get()->ReturnPlayerParameter()->onGroundFlag && 
		thisAction != ACTION_ID::DANGLE_ACTION && 
		thisAction != ACTION_ID::THREAD_ACTION && 
		thisAction != ACTION_ID::REWIND_ACTION &&
		thisAction != ACTION_ID::THREAD_WEB_ACTION) ||
		!norUp){
		//計算用重力
		Vector3 g = vector3(0, GRABITY, 0);
		*grabity += g * frameTime;
		player._Get()->SetMoveVec(*grabity);
	}
	else{
		*grabity = vector3(0, 0, 0);
	}

	//if (!player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR) return;
	
	
}


void PlayerAction::ChangeAction(ACTION_ID id){
	isDead = true; 
	beforeUp = player._Get()->ReturnPlayerParameter()->nor;
	if (id != ACTION_ID::CURL_ACTION){
		nextAction = id;
	}
	else{
		//if (a_Id == ACTION_ID::REWIND_ACTION)thread._Get()->SetIsDead(true);
		if (!player._Get()->ReturnPlayerParameter()->onGroundFlag){
			if (thisAction != ACTION_ID::AIR_CURL_ACTION){
				//if (thread._Get() != NULL && a_Id == ACTION_ID::DANGLE_ACTION)thread._Get()->SetIsDead(true);
				nextAction = ACTION_ID::AIR_CURL_ACTION;
			}
		}
		else{
			if (player._Get()->ReturnPlayerParameter()->nor.y < 0){
				if (thisAction != ACTION_ID::AIR_CURL_ACTION){
				//	if (thread._Get() != NULL && a_Id == ACTION_ID::DANGLE_ACTION)thread._Get()->SetIsDead(true);
					nextAction = ACTION_ID::AIR_CURL_ACTION;
				}
			}
			else{
				if (thisAction != ACTION_ID::GROUND_CURL_ACTION){
				//	if (thread._Get() != NULL && a_Id == ACTION_ID::DANGLE_ACTION)thread._Get()->SetIsDead(true);
					nextAction = ACTION_ID::GROUND_CURL_ACTION;
				}
			}
		}
	}
}

void PlayerAction::GetControl(){
	controlVec = player._Get()->ReturnPlayerParameter()->vec;
	controlVecNor = RCVector3::normalize(controlVec);
}

void PlayerAction::GrabityReset(Vector3* grabity){
	if (player._Get()->ReturnPlayerParameter()->onGroundFlag)
		*grabity = vector3(0, 0, 0);
}