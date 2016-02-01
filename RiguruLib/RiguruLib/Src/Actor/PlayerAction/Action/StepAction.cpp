#include "StepAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../../Math/Quaternion.h"

StepAction::StepAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :PlayerAction(world, player_, padNum_, cID_,ACTION_ID::STEP_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
	stepDelay = 0.0f;
}
StepAction::~StepAction(){

}
bool StepAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	if (beforeAction != ACTION_ID::GROUND_CURL_ACTION &&
		beforeAction != ACTION_ID::AIR_CURL_ACTION &&
		beforeAction != ACTION_ID::DANGLE_ACTION &&
		 beforeAction != ACTION_ID::REWIND_ACTION){
		if (stepDelay >= STEPDELAY && !(controlVec == vector3(0, 0, 0))){
			if (beforeAction == ACTION_ID::JUMP_ACTION)jumpStepFlag = true;
			else jumpStepFlag = false;
			playerControlFlag.firstFrameFlag = false;
			
			cameraMove = true;
			stepVec = RCVector3::normalize(player._Get()->ReturnPlayerParameter()->vec);
			stepSpeed = STEPSPEED;
			if (controlVec.x >= 0){
				leftRightID = ANIM_ID::NEPHILA_STEPRIGHT_ANIM;
			}
			else{
				leftRightID = ANIM_ID::NEPHILA_STEPLEFT_ANIM;
			}

			if (controlVec.z >= 0){
				frontBackID = ANIM_ID::NEPHILA_STEPBACK_ANIM;
			}
			else {
				frontBackID = ANIM_ID::NEPHILA_STEPFRONT_ANIM;
			}
			float upAniTime = STEPRANIMSPEED;
			float blendAnim = STEPRANIMBLEND;
			player._Get()->SetAnimation(
				(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
				(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
				WALKANIMSPEED, true, false, abs(controlVecNor.z), WALKANIMBLEND  * 2);
			hover = false;
			stepDelay = 0.0f;
			return true;
		}
	}

	return false;
}
void StepAction::Update(float frameTime){
	if (!jumpStepFlag){
		//あたり判定を４本のレイに
		world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::RAY_MODEL_NATURAL_COLL);
		//糸との判定
		world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadID, COL_ID::SPHERE_LINE_COLL);
		//蜘蛛の巣との判定
		world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadWebID, COL_ID::TRIANGLE_LINE_COLL);
		//ステップ中の判定
		world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::RAY_MODEL_STEP_COLL);
	}
	else{
		//ジャンプ中の糸との判定
		world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadID, COL_ID::SPHERE_LINE_COLL);
		//ジャンプ中の蜘蛛の巣との判定
		world.SetCollideSelect(player._Get()->shared_from_this(), friendThreadWebID, COL_ID::TRIANGLE_LINE_JUMP_COLL);
		//あたり判定を丸に
		world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::SPHERE_MODEL_NATURAL_COLL);
	}

	//疑似加速度表現
	stepSpeed -= 65.0f * frameTime * player._Get()->BindPow(true);
	if (stepSpeed <= 5.0f){
		if (!hover){
			hover = true;
			float upAniTime = HOVERANIMSPEED;
			float blendAnim = HOVERANIMBLEND;
			player._Get()->SetAnimation(
				(ANIM_ID)(leftRightID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
				(ANIM_ID)(frontBackID + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
				WALKANIMSPEED, true, true, abs(controlVecNor.z), WALKANIMBLEND);
		}
	}
	//止まったら戻す
	if (stepSpeed < 0){
		if (player._Get()->ReturnPlayerParameter()->onGroundFlag)
			ChangeAction(ACTION_ID::NORMAL_ACTION);
		else{
			ChangeAction(ACTION_ID::HOVER_ACTION);
		}
	}

	Matrix4 angleMat = player._Get()->ReturnPlayerParameter()->angleMat;
	Vector3 moveVecFront = RCVector3::normalize(RCMatrix4::getFront(angleMat));
	Vector3 moveVecLeft = RCVector3::normalize(-RCMatrix4::getLeft(angleMat));
	Vector3 moveVec = (moveVecFront * stepVec.z + moveVecLeft * stepVec.x) * stepSpeed * frameTime;

	player._Get()->SetMoveVec(moveVec);
}
void StepAction::OnCollide(Actor& other, CollisionParameter colpara){
	if (colpara.id == COL_ID::RAY_MODEL_NATURAL_COLL ||
		colpara.id == COL_ID::RAY_MODEL_STEP_COLL ||
		colpara.id == COL_ID::SPHERE_MODEL_NATURAL_COLL){
		if (colpara.id == COL_ID::SPHERE_MODEL_NATURAL_COLL){
			ChangeAction(ACTION_ID::NORMAL_ACTION);
		}
		//当たった場所へ移動
		player._Get()->SetPos(colpara.colPos);
		//Upをセット
		player._Get()->SetNor(colpara.colNormal);
		//	startNor = colpara.colNormal;

	}
	else if (colpara.id == COL_ID::SPHERE_LINE_COLL){
		//ChangeAction(ACTION_ID::THREAD_ACTION);
	}
}

void StepAction::UpdateStepDelay(float frameTime){
	stepDelay += 1.0f * frameTime;
}