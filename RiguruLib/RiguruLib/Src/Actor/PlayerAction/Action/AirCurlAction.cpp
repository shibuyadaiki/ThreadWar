#include "AirCurlAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../../Math/Quaternion.h"

AirCurlAction::AirCurlAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :
PlayerAction(world, player_, padNum_, cID_, ACTION_ID::AIR_CURL_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
}
AirCurlAction::~AirCurlAction(){

}
bool AirCurlAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	if (beforeAction == ACTION_ID::DANGLE_ACTION || beforeAction == ACTION_ID::REWIND_ACTION)player._Get()->ReturnThread()._Get()->SetIsDead(true);
	//壁擦りベクトル初期化
	curlInertiaVec = player._Get()->GetParameter().inertiaVec;
	curlSpeed = RCVector3::length(curlInertiaVec);
	curlInertiaVec = RCVector3::normalize(curlInertiaVec);

	player._Get()->SetAnimation(
		(ANIM_ID)(ANIM_ID::NEPHILA_ROUND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(ANIM_ID::NEPHILA_ROUND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		CURLANIMSPEED,
		true, true, 0, CURLANIMBLEND);
	cameraMove = true;
	playerControlFlag.firstFrameFlag = false;
	wallVec = vector3(0, 0, 0);

	return true;
}
void AirCurlAction::Update(float frameTime){
	Vector3 nor = player._Get()->ReturnPlayerParameter()->nor;
	Vector3 moveVec = curlInertiaVec * curlSpeed;
	curlSpeed -= (curlSpeed * 1.25f) * frameTime;
	if (curlSpeed < 0) curlSpeed = 0;
	player._Get()->SetMoveVec(moveVec);
	player._Get()->SetMoveVec(nor * 0.01f);
	//地面とのあたり判定
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::SPHERE_MODEL_NATURAL_COLL);
}
void AirCurlAction::OnCollide(Actor& other, CollisionParameter colpara){
	//もし丸が当たっていたら
	if (colpara.id == COL_ID::SPHERE_MODEL_NATURAL_COLL){
		//当たった場所へ移動
		player._Get()->SetPos(colpara.colPos);
		//Upをセット
		player._Get()->SetNor(colpara.colNormal);
		if (colpara.colNormal.y >= 0)
		ChangeAction(ACTION_ID::GROUND_CURL_ACTION);
		curlInertiaVec += curlInertiaVec - 2 * RCVector3::dot(curlInertiaVec, colpara.colNormal) * colpara.colNormal;
		curlInertiaVec = RCVector3::normalize(curlInertiaVec);
		//curlSpeed /= 4.0f;
	}
}