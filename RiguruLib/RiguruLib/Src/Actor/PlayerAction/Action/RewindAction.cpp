#include "RewindAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../../Math/Quaternion.h"

RewindAction::RewindAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :
PlayerAction(world, player_, padNum_, cID_, ACTION_ID::REWIND_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
}
RewindAction::~RewindAction(){

}
bool RewindAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	ThreadParameter threadParam = player._Get()->ReturnThread()._Get()->GetThreadParameter();
	player._Get()->ReturnThread()._Get()->SetIsRewind(true);
	//プレイヤーの向きが、始点と終点のどちらにより向いているかを比較
	Vector3 pFrontNor = RCVector3::normalize(-RCMatrix4::getFront(player._Get()->GetParameter().matrix));
	float deg_startNor = Math::acos(RCVector3::dot(pFrontNor, threadParam.startNormal));
	float deg_endNor = Math::acos(RCVector3::dot(pFrontNor, threadParam.endNormal));

	if (deg_endNor < deg_startNor) playerControlFlag.isGoingEnd = true;
	else playerControlFlag.isGoingEnd = false;
	if (beforeAction == ACTION_ID::DANGLE_ACTION){
		playerControlFlag.isGoingEnd = true;
	}
	//プレイヤーの位置からスタート
	if (playerControlFlag.isGoingEnd){
		goingVec = RCVector3::normalize(threadParam.endNormal);
		threadParam.startPosition = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	}
	else{
		goingVec = RCVector3::normalize(threadParam.startNormal);
		threadParam.endPosition = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);

	}

	player._Get()->SetAnimation(
		(ANIM_ID)(ANIM_ID::NEPHILA_REWIND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(ANIM_ID::NEPHILA_REWIND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		WALKANIMSPEED * 1.0f, true, true, 0, WALKANIMBLEND);
	return true;
}

void RewindAction::Update(float frameTime){
	player._Get()->SetMoveVec(goingVec * REWINDSPEED * frameTime);
	if (playerControlFlag.isGoingEnd)
		player._Get()->ReturnThread()._Get()->SetStartPos(RCMatrix4::getPosition(player._Get()->GetParameter().matrix) + goingVec * REWINDSPEED * frameTime);
	else{
		if (player._Get()->ReturnThread().lock() == nullptr && !player._Get()->ReturnThread()._Get()->GetThreadParameter().startPosIsGroundTouch){
			ChangeAction(ACTION_ID::HOVER_ACTION);
		}
		player._Get()->ReturnThread()._Get()->SetEndPos(RCMatrix4::getPosition(player._Get()->GetParameter().matrix) + goingVec * REWINDSPEED * frameTime);
	}
	//あたり判定を丸に
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::SPHERE_MODEL_NATURAL_COLL);
}

void RewindAction::OnCollide(Actor& other, CollisionParameter colpara){
	if (colpara.id == COL_ID::SPHERE_MODEL_NATURAL_COLL){
		ChangeAction(ACTION_ID::NORMAL_ACTION);
		//当たった場所へ移動
		player._Get()->SetPos(colpara.colPos);
		//Upをセット
		player._Get()->SetNor(colpara.colNormal);

		player._Get()->ReturnThread()._Get()->SetIsDead(true);
	}
}