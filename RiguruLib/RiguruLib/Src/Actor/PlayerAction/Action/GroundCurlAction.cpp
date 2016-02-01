#include "GroundCurlAction.h"
#include "../../Collision.h"
#include "../../../Graphic/Graphic.h"
#include "../../Player.h"
#include "../../Thread.h"
#include "../../../Math/Quaternion.h"

GroundCurlAction::GroundCurlAction(IWorld& world, std::weak_ptr<Player> player_, int padNum_, CAMERA_ID cID_) :
PlayerAction(world, player_, padNum_, cID_, ACTION_ID::GROUND_CURL_ACTION){
	isDead = false;
	nextAction = ACTION_ID::JUMP_ACTION;
}
GroundCurlAction::~GroundCurlAction(){

}
bool GroundCurlAction::Initialize(ACTION_ID beforeId, Vector3 beforeUp){
	beforeAction = beforeId;
	if (beforeAction == ACTION_ID::DANGLE_ACTION || beforeAction == ACTION_ID::REWIND_ACTION)player._Get()->ReturnThread()._Get()->SetIsDead(true);
	//壁擦りベクトル初期化
	curlInertiaVec = player._Get()->GetParameter().inertiaVec;
	curlSpeed = RCVector3::length(curlInertiaVec);
	curlInertiaVec = RCVector3::normalize(curlInertiaVec);

	player._Get()->SetAnimation(
		(ANIM_ID)(ANIM_ID::NEPHILA_ROUND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		(ANIM_ID)(ANIM_ID::NEPHILA_ROUND_ANIM + (player._Get()->GetParameter().id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
		CURLANIMSPEED, true, true, 0, CURLANIMBLEND);
	cameraMove = true;
	playerControlFlag.firstFrameFlag = false;
	wallVec = vector3(0, 0, 0);

	return true;
}
void GroundCurlAction::Update(float frameTime){
	Vector3 nor = player._Get()->ReturnPlayerParameter()->nor;
	//丸まる前の移動量とノーマルから壁擦りベクトルを求める
	Vector3 moveVec = (curlInertiaVec - RCVector3::dot(curlInertiaVec,nor) * nor) * curlSpeed;
	curlSpeed -= (curlSpeed * 1.25f) * frameTime;
	if (curlSpeed < 0) curlSpeed = 0;
	//計算用重力
	Vector3 g = vector3(0, GRABITY, 0);
	//重力と法線から壁擦りベクトルを求め前回のに足して滑らかに加速していく(0.75fは摩擦)
	wallVec += (g - RCVector3::dot(g, nor) * nor) * 0.3f;
	//その壁擦りベクトルから壁擦りベクトルを求め直し地面を突き抜けぬようにする
	wallVec = wallVec - RCVector3::dot(wallVec, nor) * nor;
	//壁擦りベクトルを加算する
	moveVec += wallVec * frameTime;

	//if (!startNorChangeFlag)
	player._Get()->SetMoveVec(moveVec);
	//地面とのあたり判定
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::RAY_MODEL_NATURAL_COLL);
	//壁にめり込まないようにするStep用の処理
	world.SetCollideSelect(player._Get()->shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::RAY_MODEL_STEP_COLL);
}
void GroundCurlAction::OnCollide(Actor& other, CollisionParameter colpara){
	//もし丸が当たっていたら
	if (colpara.id == COL_ID::RAY_MODEL_NATURAL_COLL){
		//当たった場所へ移動
		player._Get()->SetPos(colpara.colPos);

		//上方向をセット
		player._Get()->SetNor(colpara.colNormal);

		if (colpara.colNormal.y < 0.0f){
			ChangeAction(ACTION_ID::AIR_CURL_ACTION);
		}

	}
	//もしStep中レイが壁にめり込んだら
	else if (colpara.id == COL_ID::RAY_MODEL_STEP_COLL){
		//当たった場所へ移動
		player._Get()->SetPos(colpara.colPos);

		//上方向をセット
		player._Get()->SetNor(colpara.colNormal);
	}
}