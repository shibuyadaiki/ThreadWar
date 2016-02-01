#include "FirstAI.h"
#include "../../Audio/Audio.h"
#include "../Thread.h"
#include "../CrystalCenter.h"
#include "../Collision.h"

//視野角(自身のflontからの角度)
static const float viewAngle = 70.0f;
//視野距離
static const float viewLength = 50.0f;
//攻撃範囲角(カメラのflontからの角度)
static const float attackAngle = 10.0f;
//攻撃範囲距離
static const float attackLength = 40.0f;
//巻き取って接近する時の最短距離(これより遠い場合は巻き取って接近)
static const float rewindLength = 30.0f;
//理想距離
static const float bestLength = 15.0f;
//強制的に戦闘状態に移行する距離
static const float dangerLength = 10.0f;

FirstAI::FirstAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_) :
EnemyAI(wo, action, player, cID_, parameter_),
stage(stage_){
	Initialize();
}

void FirstAI::Initialize(){
	nextAI = AI_STATE::FIRST_AI;
	teamID = player._Get()->GetTeam();

	aiTimer = Math::random(1.0f, 5.0f);
	lostTimer = 0.0f;

	for (int i = 0; i < 4; i++){
		targetMats[i] = RCMatrix4::Identity();
	}
	target.mat = RCMatrix4::Identity();
	target.pos = vector3(0, 0, 0);
	target.vec = vector3(0, 0, 0);
	target.isLooking = false;
	target.lastLookingPos = vector3(0, 0, 0);
	target.lastLookingVec = vector3(0, 0, 0);
	target.distance = 0.0f;
	target.isBattle = false;

	flag = false;
	timer = 0.0f;
	counter = 0;

	moveTime = stepTime = threadTime = 0;
	moveTimeMax = Math::random(1.0f, 5.0f);
	stepTimeMax = Math::random(1.0f, 3.0f);
	threadTimeMax = Math::random(1.0f, 2.0f);
	inputVecX = 1.0f;
	inputCameraX = 1.0f;

	myMat = player._Get()->GetParameter().matrix;
	myPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	myPos2D = vector2(myPos.x, myPos.z);
	myParam = player._Get()->ReturnPlayerParameter();

	camera = Device::GetInstance().GetCamera(cID);
	cameraRotMat = RCMatrix4::Identity();
	cameraFront = vector3(0, 0, 0);
	cameraLeft = vector3(0, 0, 0);
	cameraUp = vector3(0, 0, 0);
	inputAngle = vector2(0, 0);
	inputVec = vector3(0, 0, 0);

	centerCrystalDist = iceCrystalDist = caveCrystalDist = nearestCrystalDist = 0.0f;
	nearestCrystalPos = vector3(0, 0, 0);
	targetCrystal = false;

	//サーチ対象
	if (parameter.id == ACTOR_ID::ENEMY_ACTOR) searchActor = ACTOR_ID::PLAYER_ACTOR;
	else searchActor = ACTOR_ID::ENEMY_ACTOR;

	actionFunc.clear();
	actionFunc[AI_ACTION::Curl] = std::bind(&FirstAI::Curl, this, std::placeholders::_1);
	actionFunc[AI_ACTION::RandomMove] = std::bind(&FirstAI::RandomMove, this, std::placeholders::_1);
	actionFunc[AI_ACTION::RandomLook] = std::bind(&FirstAI::RandomLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TurnLook] = std::bind(&FirstAI::TurnLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::JumpShotRewind] = std::bind(&FirstAI::JumpShotRewind, this, std::placeholders::_1);
	actionFunc[AI_ACTION::FrontLook] = std::bind(&FirstAI::FrontLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookH] = std::bind(&FirstAI::TargetPointLookH, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookV] = std::bind(&FirstAI::TargetPointLookV, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLook] = std::bind(&FirstAI::TargetPointLook, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointLookAttack] = std::bind(&FirstAI::TargetPointLookAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointMove] = std::bind(&FirstAI::TargetPointMove, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetPointMoveAttack] = std::bind(&FirstAI::TargetPointMoveAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::CrystalPointMoveAttack] = std::bind(&FirstAI::CrystalPointMoveAttack, this, std::placeholders::_1);
	actionFunc[AI_ACTION::TargetJumpShotRewind] = std::bind(&FirstAI::TargetJumpShotRewind, this, std::placeholders::_1);
	actionFunc[AI_ACTION::GettingThread] = std::bind(&FirstAI::GettingThread, this, std::placeholders::_1);
	actionFunc[AI_ACTION::CreateThreadWeb] = std::bind(&FirstAI::CreateThreadWeb, this, std::placeholders::_1);

	for (int i = 0; i < 3; i++)
	{
		level[i].viewAngle = 35.0f * (i + 1);
		level[i].viewLength = 25.0f * (i + 1);
		level[i].attackAngle = level[i].dangerLength = 5.0f * (i + 1);
		level[i].attackLength = 20.0f * (i + 1);
		level[i].rewindLength = 30.0f;
		level[i].bestLength = 15.0f;
		level[i].penetrationTime = 1.0f * (i + 1);
		level[i].inputXChangeTimeMax = 10.0f / (i + 1);
		level[i].inputXChangeTimeMin = 2.0f / (i + 1);
		level[i].stepTimeMax = 6.0f / (i + 1);
		level[i].stepTimeMin = 2.0f / (i + 1);
		level[i].battleStepTimeMax = 4.0f / (i + 1);
		level[i].battleStepTimeMin = 2.0f / (i + 1);
	}
	levelNum = 1;
}

Vector3 FirstAI::UpdateAI(float frameTime){
	//入力状態リセット
	InputReset();

	//パラメータ等、AI実行前に必要な計算
	ParamCalc();

	//AIを実行
	AIRun(frameTime);
	//stateFunc[currentAction](frameTime);
	//GettingThread(frameTime);

	//最終的な入力
	FinalInput();

	return inputVec;
}

void FirstAI::ParamCalc()
{
	//自身のパラメータを取得
	myMat = player._Get()->GetParameter().matrix;
	myPos = RCMatrix4::getPosition(player._Get()->GetParameter().matrix);
	myPos2D = vector2(myPos.x, myPos.z);
	myParam = player._Get()->ReturnPlayerParameter();

	//カメラの取得
	//camera = Device::GetInstance().GetCamera(cID);
	cameraRotMat = camera->GetCameraRotMatrix();
	cameraFront = -RCVector3::normalize(RCMatrix4::getFront(cameraRotMat));
	cameraLeft = RCVector3::normalize(RCMatrix4::getLeft(cameraRotMat));
	cameraUp = RCVector3::normalize(RCMatrix4::getUp(cameraRotMat));

	//各クリスタルまでの距離
	centerCrystalDist = RCVector3::distance_(myPos, centerCrystalPos);
	iceCrystalDist = RCVector3::distance_(myPos, iceCrystalPos);
	caveCrystalDist = RCVector3::distance_(myPos, caveCrystalPos);
	//一番近い攻撃対象クリスタルを特定
	targetCrystal = true;
	if (parameter.id == ACTOR_ID::PLAYER_ACTOR){
		if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsPlayer() &&
			caveCrystalDist < centerCrystalDist && caveCrystalDist < iceCrystalDist){
			nearestCrystalPos = caveCrystalPos;
			nearestCrystalDist = caveCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsPlayer() &&
			centerCrystalDist < iceCrystalDist){
			nearestCrystalPos = centerCrystalPos;
			nearestCrystalDist = centerCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsPlayer()){
			nearestCrystalPos = iceCrystalPos;
			nearestCrystalDist = iceCrystalDist;
		}
		else
			targetCrystal = false;
	}
	else{
		if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsEnemy() &&
			iceCrystalDist < centerCrystalDist && iceCrystalDist < caveCrystalDist){
			nearestCrystalPos = iceCrystalPos;
			nearestCrystalDist = iceCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsEnemy() &&
			centerCrystalDist < caveCrystalDist){
			nearestCrystalPos = centerCrystalPos;
			nearestCrystalDist = centerCrystalDist;
		}
		else if (!stage._Get()->ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsEnemy()){
			nearestCrystalPos = caveCrystalPos;
			nearestCrystalDist = caveCrystalDist;
		}
		else
			targetCrystal = false;
	}

	//入力初期化
	inputAngle = vector2(0, 0);
	inputVec = vector3(0, 0, 0);
}

void FirstAI::AIRun(float frameTime)
{
	//敵の位置を検索
	int num = 0;
	world.EachActor(searchActor, [&](const Actor& other){
		targetMats[num] = static_cast<Player*>(const_cast<Actor*>(&other))->GetParameter().matrix;
		targetDists[num] = RCVector3::distance_(myPos, RCMatrix4::getPosition(targetMats[num]));
		num++;
	});

	//視界内に目標がいるか調べる
	target.isLooking = Search();

	//目標を発見するまではクリスタルを攻撃or索敵
	if (!target.isBattle)
	{
		if (targetCrystal)
		{
			//ターゲットをクリスタルに
			target.lastLookingPos = nearestCrystalPos;
			target.lastLookingVec = RCVector3::normalize(nearestCrystalPos - myPos);
			target.distance = nearestCrystalDist;
			//CrystalPointMoveAttack(frameTime);
			ChangeAction(AI_ACTION::CrystalPointMoveAttack);
		}
		else
		{
			aiTimer += frameTime;
			if (aiTimer < 8.0f)
				ChangeAction(AI_ACTION::RandomMove);
			//RandomMove(frameTime);
			else if (aiTimer < 11.0f)
				ChangeAction(AI_ACTION::CreateThreadWeb);
			//CreateThreadWeb(frameTime);
			else if (aiTimer < 14.0f)
				ChangeAction(AI_ACTION::JumpShotRewind);
			//JumpShotRewind(frameTime);
			else
			{
				ChangeAction(AI_ACTION::RandomMove);
				aiTimer = 0.0f;
				//flag = false;
				//timer = 0.0f;
			}
		}

		//敵を発見した場合
		if (target.isLooking)
		{
			//距離がクリスタルより近い場合
			if (targetDists[targetNum] < nearestCrystalDist)
			{
				//戦闘状態へ移行
				target.isBattle = true;
				aiTimer = 0.0f;
			}
		}
	}

	//戦闘状態の場合
	if (target.isBattle)
	{
		//視界内に捉えているなら
		if (target.isLooking)
		{
			//最後に捉えた地点を更新
			target.lastLookingPos = target.pos;
			//方向も取得
			target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			lostTimer = 0;
		}
		//捉えていないなら
		else
		{
			//見失った時間に加算
			lostTimer += frameTime;
			//2秒以内は貫通して見える
			if (lostTimer <= 2.0f){
				target.lastLookingPos = target.pos;
				target.lastLookingVec = RCVector3::normalize(target.lastLookingPos - myPos);
			}
		}

		if (lostTimer <= 4.0f)
		{
			//距離が近く、接地しているなら
			if (myParam->onGroundFlag && target.distance <= bestLength)
				//糸発射しながら移動
				ChangeAction(AI_ACTION::TargetPointMoveAttack);
			//TargetPointMoveAttack(frameTime);
			//距離が遠いなら
			else
				//巻き取って接近
				ChangeAction(AI_ACTION::TargetJumpShotRewind);
			//TargetJumpShotRewind(frameTime);
		}
	}

	//４秒以上見失ったら
	if (lostTimer > 4.0f)
	{
		//最後に捉えた地点まで移動
		float dist = RCVector2::distance_(myPos2D, vector2(target.lastLookingPos.x, target.lastLookingPos.z));
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 180), vector2(0.20f, 0.25f), 0.5f, "lost:" + std::to_string(lostTimer));
		if (lostTimer  < 12.0f)
			//8秒間移動
			ChangeAction(AI_ACTION::TargetPointMove);
		//TargetPointMove(frameTime);
		else
		{
			//移動し終えたら旋回して索敵
			aiTimer += frameTime;
			if (aiTimer < 6.0f)
				//索敵のために回転
				ChangeAction(AI_ACTION::TurnLook);
			//TurnLook(frameTime);
			else if (aiTimer >= 6.0f)
			{
				//回転しても見つからなかった場合は戦闘状態解除
				target.isBattle = false;
				lostTimer = 0;
				aiTimer = 0;
			}
		}
	}

	actionFunc[currentAction](frameTime);

	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 400), vector2(0.20f, 0.25f), 0.5f, "looking:" + std::to_string(target.isLooking));
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 380), vector2(0.20f, 0.25f), 0.5f, "attack:" + std::to_string(AttackArea()));
}

void FirstAI::InputReset()
{
	player._Get()->SetInputCurlOn(false);
	player._Get()->SetInputCurlOff(false);
	player._Get()->SetInputJump(false);
	player._Get()->SetInputStep(false);
	player._Get()->SetInputShot(false);
}

void FirstAI::ChangeAction(AI_ACTION s)
{
	if (currentAction != s)
	{
		currentAction = s;
		flag = false;
		timer = 0.0f;
		counter = 0;
	}
}
bool FirstAI::IsCeiling()
{
	return RCV3Calc::InnerAngle(RCVector3::normalize(RCMatrix4::getUp(cameraRotMat)), vector3(0, 1, 0)) > 90;
}
void FirstAI::Curl(float frameTime)
{
	if (!flag)
	{
		InputStep();
		InputCurlOn();
		flag = true;
	}

	timer += frameTime;
	if (timer >= 3.0f)
	{
		InputCurlOff();
	}
}
void FirstAI::RandomMove(float frameTime)
{
	//タイマ増加
	moveTime += frameTime;
	stepTime += frameTime;

	//前進入力
	inputVec.z = -1;
	//左右入力
	inputVec.x = inputVecX;
	inputVec = RCVector3::normalize(inputVec);
	//左右入力切替
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}

	//ステップ入力
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}
}
void FirstAI::RandomLook(float frameTime)
{
	if (!flag)
	{
		moveTimeMax = Math::random(1.0f, 3.0f);	//入力の長さ
		inputCameraX = Math::random(3.0f, 9.0f);	//入力の強さ
		if (Math::random(-1.0f, 1.0f) < 0) inputVecX *= -1;	//1/2の確率で入力方向反転
		flag = true;
	}

	//タイマ増加
	timer += frameTime;
	//カメラ入力
	inputAngle.x = inputCameraX;

	if (timer > moveTimeMax)
	{
		//もう一回
		flag = false;
		timer = 0.0f;
	}
}
void FirstAI::TurnLook(float frameTime)
{
	if (!flag)
	{
		inputCameraX = 3.0f;	//入力の強さ
		flag = true;
	}

	//カメラ入力
	inputAngle.x = inputCameraX;
}
void FirstAI::JumpShotRewind(float frameTime)
{
	timer += frameTime;
	inputVec.z = -1.0f;
	FrontLook(frameTime);
	if (timer < 1.0f) return;

	//一秒超えたらジャンプ→発射→巻取り
	switch (counter)
	{
	case 0:
		InputJump();
		counter++;
		break;

	case 1:
		if (timer > 1.3f)
		{
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.4f)
		{
			InputRewind();
			counter++;
		}
		break;
	case 3:
		//巻き取りの成功/失敗にかかわらず、接地していたら
		if (myParam->onGroundFlag)
		{
			//初期化
			counter = 0;
			timer = 0;
		}
	default:
		break;
	}

}
void FirstAI::SlantingShotRewind(float frameTime)
{
	if (counter >= 1)
		timer += frameTime;

	float angle = Math::acos(RCVector3::dot(cameraFront, RCVector3::normalize(RCMatrix4::getUp(myMat))));
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 120), vector2(0.20f, 0.25f), 0.5f, "angle:" + std::to_string(angle));

	//斜め上を向いたら巻き取って移動
	switch (counter)
	{
	case 0:
		//自身の上方向にカメラを向ける
		if (angle > 30)
		{
			inputAngle.y += 6.0f;
			break;
		}
		counter++;
		break;

	case 1:
		//最初にジャンプ
		InputJump();
		counter++;
		break;

	case 2:
		if (timer > 0.3f)
		{
			//発射
			InputShotThread();
			counter++;
		}
		break;

	case 3:
		if (timer > 0.5f)
		{
			//巻き取り
			InputRewind();
			counter++;
		}
		break;

	default:
		break;
	}
}
void FirstAI::LookV(float frameTime, Vector3 v)
{
	//向きたい方向
	Vector3 vec = v;

	if (vec.z > 0)
		vec = -vec;//+の値に制限して計算

	//上下判定
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.y, cameraFront.z)),
		RCVector2::normalize(vector2(vec.y, vec.z))));

	//なす角
	float acos = Math::acos(RCVector3::dot(
		RCVector3::normalize(vector3(0, cameraFront.y, cameraFront.z)),
		RCVector3::normalize(vector3(0, vec.y, target.vec.z))));

	//なす角を基に入力量を計算
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//入力により向きを変える
	if (asin < 0) inputAngle.y = -add;
	else inputAngle.y = add;

	//天井にいる場合は入力反転
	if (IsCeiling()) inputAngle.y *= -1;
}
void FirstAI::FrontLook(float frameTime)
{
	LookV(frameTime, RCMatrix4::getFront(myMat));
}
void FirstAI::TargetPointLookH(float frameTime)
{
	//左右判定
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.x, cameraFront.z)),
		RCVector2::normalize(vector2(target.lastLookingVec.x, target.lastLookingVec.z))));

	//なす角
	float acos = RCV3Calc::InnerAngle(
		vector3(cameraFront.x, 0, cameraFront.z),
		vector3(target.lastLookingVec.x, 0, target.lastLookingVec.z));

	//なす角を基に入力量を計算
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//入力により向きを変える
	if (asin > 0) inputAngle.x = -add;
	else inputAngle.x = add;

	//天井にいる場合は入力反転
	if (IsCeiling()) inputAngle.x *= -1;
}
void FirstAI::TargetPointLookV(float frameTime)
{
	//向きたい方向
	Vector3 vec = target.lastLookingVec;

	if (vec.z > 0)
		vec = -vec;//+の値に制限して計算

	//上下判定
	float asin = Math::asin(RCVector2::cross(
		RCVector2::normalize(vector2(cameraFront.y, cameraFront.z)),
		RCVector2::normalize(vector2(vec.y, vec.z))));

	//なす角
	float acos = RCV3Calc::InnerAngle(
		vector3(0, cameraFront.y, cameraFront.z),
		vector3(0, vec.y, target.vec.z));

	//なす角を基に入力量を計算
	float add = fabsf(acos) / 2;
	add = Math::clamp(add, 1.0f, 10.0f);

	//入力により向きを変える
	if (asin < 0) inputAngle.y = -add;
	else inputAngle.y = add;

	//天井にいる場合は入力反転
	if (IsCeiling()) inputAngle.y *= -1;
}
void FirstAI::TargetPointLook(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
}
void FirstAI::TargetPointLookAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);

	timer += frameTime;
	if (timer >= 0.5f)
	{
		InputShotThread();
		timer = 0;
	}
}
void FirstAI::TargetPointMove(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//前進
	inputVec.z = -1;

	//タイマ増加
	stepTime += frameTime;
	//ステップ入力
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}
}
void FirstAI::TargetPointMoveAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//タイマ増加
	moveTime += frameTime;
	stepTime += frameTime;

	//距離によって前後の移動方向を変える
	if (target.distance > bestLength - 5)
		//前進
		inputVec.z = -1;
	else
		//後退
		inputVec.z = 1;

	//左右入力切替
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}
	//左右入力
	inputVec.x = inputVecX;
	//正規化
	inputVec = RCVector3::normalize(inputVec);


	//ステップ入力
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}

	//糸の発射
	timer += frameTime;

	if (timer > 0.7f)
	{
		InputShotThread();
		timer = 0;
	}
}
void FirstAI::CrystalPointMoveAttack(float frameTime)
{
	TargetPointLookH(frameTime);
	TargetPointLookV(frameTime);
	//タイマ増加
	moveTime += frameTime;
	stepTime += frameTime;


	//距離によって前後の移動方向を変える
	if (target.distance > bestLength)
		//前進
		inputVec.z = -1;
	else
		//後退
		inputVec.z = 1;

	//左右入力切替
	if (moveTime > moveTimeMax){
		inputVecX = -inputVecX;
		moveTime = 0;
		moveTimeMax = Math::random(1.0f, 5.0f);
	}
	//左右入力
	inputVec.x = inputVecX;
	//正規化
	inputVec = RCVector3::normalize(inputVec);


	//ステップ入力
	if (stepTime > stepTimeMax){
		InputStep();
		stepTime = 0;
		stepTimeMax = Math::random(1.0f, 3.0f);
	}

	//糸の発射
	timer += frameTime;

	if (timer > 0.2f)
	{
		Vector3 start = myPos + RCMatrix4::getUp(myMat) * 3.0f;
		Vector3 end = nearestCrystalPos;
		end = end - RCVector3::normalize(end - start) * 2.0f;
		//Graphic::GetInstance().DrawLine(start, end, CAMERA_ID::PLAYER_CAMERA_1P);
		CollisionParameter stageCol = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, start, end);
		if (!stageCol.colFlag)
			InputShotThread();
		timer = 0;
	}
}
void FirstAI::TargetJumpShotRewind(float frameTime)
{
	timer += frameTime;
	//目標を向きながら前進
	TargetPointLook(frameTime);
	inputVec.z = -1.0f;

	if (timer < 1.0f) return;

	//一秒超えたらジャンプ→発射→巻取り
	switch (counter)
	{
	case 0:
		InputJump();
		counter++;
		break;

	case 1:
		if (timer > 1.3f)
		{
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.4f)
		{
			InputRewind();
			counter++;
		}
		break;
	case 3:
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 150), vector2(0.20f, 0.25f), 0.5f, "jump:" + std::to_string(target.distance < bestLength));
		if (target.distance < bestLength)
		{
			InputJump();
			counter++;
		}
		if (timer > 5.0f)
		{
			timer = 0;
			counter = 0;
		}
		break;
	default:
		counter = 0;
		timer = 0;
		break;
	}
}
void FirstAI::GettingThread(float frameTime)
{
	if (counter >= 1)
		timer += frameTime;

	float angle = RCV3Calc::InnerAngle(cameraFront, RCMatrix4::getUp(myMat));

	//斜め上を向いたら糸発射→糸に乗る
	switch (counter)
	{
	case 0:
		//自身の上方向にカメラを向ける
		if (angle > 30)
		{
			inputAngle.y += 6.0f;
			break;
		}
		counter++;
		break;

	case 1:
		if (timer > 0.6f)
		{
			//発射
			InputShotThread();
			counter++;
		}
		break;

	case 2:
		if (timer > 1.2f)
		{
			////巻き取り
			//InputRewind();
			counter++;
		}
		break;

	default:
		break;
	}
}
void FirstAI::CreateThreadWeb(float frameTime)
{
	//カメラ入力による回転
	inputAngle.x = 10.0f;
	//照準を正面より少し上に向ける
	FrontLook(frameTime);

	//糸発射
	threadTime += frameTime;
	if (threadTime >= 0.2f)
	{
		InputShotThread();
		threadTime = 0;
	}
}

bool FirstAI::Search()
{
	for (int i = 0; i < teamMemberCount; i++)
	{
		//目標のマトリックスから計算
		Matrix4 mat = targetMats[i];
		Vector3 pos = RCMatrix4::getPosition(mat);
		Vector3 vec = RCVector3::normalize(pos - myPos);
		float dist = targetDists[i];
		//強制的に戦闘状態に入る距離に敵がいるか
		if (dist < dangerLength)
		{
			//targetが確定
			target.mat = mat;
			target.pos = pos;
			target.vec = vec;
			target.distance = dist;
			targetNum = i;
			return true;
		}

		//視野距離以内にいるか
		//視野角内にいるか
		Vector3 front = RCVector3::normalize(-RCMatrix4::getFront(myMat));
		float acos = Math::acos(RCVector3::dot(front, vec));
		//敵から自身の間の線分にステージのポリゴンがあるか
		Vector3 start = myPos + RCMatrix4::getUp(myMat) * 3.0f;
		Vector3 end = pos + RCMatrix4::getUp(mat) * 3.0f;
		CollisionParameter stageCol = ModelRay(*stage._Get()->ReturnMat(), OCT_ID::STAGE_OCT, start, end);
		if (dist < level[levelNum].viewLength && acos < level[levelNum].viewAngle && !stageCol.colFlag)
		{
			//targetが確定
			target.mat = mat;
			target.pos = pos;
			target.vec = vec;
			target.distance = dist;
			targetNum = i;
			return true;
		}
	}
	return false;
}

void FirstAI::SearchLineDraw()
{
	//Vector3 playerFront = -RCVector3::normalize(RCMatrix4::getFront(myMat));
	//Vector3 playerUp = RCVector3::normalize(RCMatrix4::getUp(myMat));
	//Vector3 playerLeft = RCVector3::normalize(RCMatrix4::getLeft(myMat));
	////上下(x軸回転)
	//Matrix4 rollmat = RCQuaternion::rotate(playerLeft, viewAngle);
	//Vector3 frontRise = RCVector3::normalize(playerFront * rollmat);
	//rollmat = RCQuaternion::rotate(playerLeft, -viewAngle);
	//Vector3 frontFall = RCVector3::normalize(playerFront * rollmat);
	////左右(y軸回転)
	//Matrix4 pitchmat = RCQuaternion::rotate(playerUp, viewAngle);
	//Vector3 frontTurnRight = RCVector3::normalize(playerFront * pitchmat);
	//pitchmat = RCQuaternion::rotate(playerUp, -viewAngle);
	//Vector3 frontTurnLeft = RCVector3::normalize(playerFront * pitchmat);

	//Graphic::GetInstance().DrawLine(myPos, myPos + frontRise * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontFall * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnRight * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnLeft * viewLength, CAMERA_ID::ENEMY_CAMERA, vector3(0, 1, 0));
}

bool FirstAI::AttackArea()
{
	//①攻撃距離以内にいるか
	if (target.distance > attackLength) return false;

	//②視野角内にいるか
	//AttackAreaLineDraw();
	float acos = Math::acos(RCVector3::dot(cameraFront, target.vec));
	if (acos > attackAngle) return false;

	return true;
}

void FirstAI::AttackAreaLineDraw()
{
	//Vector3 cameraPos = camera->CameraParam()->Eye;
	////上下(x軸回転)
	//Matrix4 rollmat = RCQuaternion::rotate(cameraLeft, attackAngle);
	//Vector3 frontRise = RCVector3::normalize(cameraFront * rollmat);
	//rollmat = RCQuaternion::rotate(cameraLeft, -attackAngle);
	//Vector3 frontFall = RCVector3::normalize(cameraFront * rollmat);
	////左右(y軸回転)
	//Matrix4 pitchmat = RCQuaternion::rotate(cameraUp, attackAngle);
	//Vector3 frontTurnRight = RCVector3::normalize(cameraFront * pitchmat);
	//pitchmat = RCQuaternion::rotate(cameraUp, -attackAngle);
	//Vector3 frontTurnLeft = RCVector3::normalize(cameraFront * pitchmat);

	//Graphic::GetInstance().DrawLine(myPos, myPos + frontRise * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontFall * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnRight * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
	//Graphic::GetInstance().DrawLine(myPos, myPos + frontTurnLeft * attackLength, CAMERA_ID::ENEMY_CAMERA, vector3(1, 0, 0));
}

void FirstAI::InputCurlOn()
{
	player._Get()->SetInputCurlOn(true);
}

void FirstAI::InputCurlOff()
{
	player._Get()->SetInputCurlOff(true);
}

void FirstAI::InputJump()
{
	player._Get()->SetInputJump(true);
}

void FirstAI::InputStep()
{
	player._Get()->SetInputStep(true);
}

void FirstAI::InputShotThread()
{
	player._Get()->SetInputShot(true);
}

void FirstAI::InputRewind()
{
	if (!player._Get()->ReturnThread().expired())//thread(weak_ptr)がemptyでなければ巻取り実行
		action->ChangeAction(ACTION_ID::REWIND_ACTION);
}

void FirstAI::InputStepCurl()
{
	InputStep();
	InputCurlOn();
}

void FirstAI::FinalInput()
{
	//最終的な入力結果(自身の前後左右)を渡す。移動の計算はその後プレイヤークラスに任せる。
	myParam->vec = inputVec;
	//最終的な入力結果(カメラ)を渡す。カメラの方向計算はその後カメラクラスに任せる。
	camera->AddInputAngleHV(inputAngle.x, inputAngle.y);
}