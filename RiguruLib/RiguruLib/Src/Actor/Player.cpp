#include "Player.h"
#include  "../Other/Device.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtility.h"
#include "../Graphic/Graphic.h"
#include "ID.h"
#include "../Audio/Audio.h"
#include "../Math/Converter.h"
#include "Collision.h"
#include "Thread.h"
#include "Stage.h"
#include "Camera\Camera.h"
#include "ThreadWeb.h"
#include "EnemyAI\FirstAI.h"
#include "PlayerAction\Action\RewindAction.h"
#include "PlayerAction\PlayerActionPtr.h"
#include "PlayerAction\PlayerAction.h"
#include "ThreadEffect.h"
#include "EnemyAI\EnemyAI.h"

//プレイヤーの半径
const float RADIUS = 0.5f;
//敵の橋の拘束時間
const float LINE_BIND_TIME = 60;
//敵の巣の拘束時間
const float WEB_BIND_TIME = 60 * 3;
//糸に拘束されていた場合の速度や移動距離の減衰率
const float BINDPOW_LINE = 0.5f;
//巣に拘束されていた場合の速度や移動距離の減衰率
const float BINDPOW_WEB = 0.25f;
Player::Player(IWorld& world_, std::weak_ptr<Stage> stage_, CAMERA_ID cID_, int padNum_, int playerNum_) :Actor(world_), stage(stage_), pAM(), cID(cID_), padNum(padNum_),playerNum(playerNum_){
	animSpeed =new float(5.0f);
	blendSpeed = new float(5.0f);
	firstFlag = true;
}

Player::~Player(){
	SAFE_DELETE(animSpeed);
	SAFE_DELETE(blendSpeed);
}
void Player::Initialize(){
	curlAngle = 0;
	//パラメータセット
	parameter.isDead = false;
	parameter.id = ACTOR_ID::PLAYER_ACTOR;
	parameter.cubeScale = vector3(1.0f, 3.0f, 1.0f);
	parameter.radius = RADIUS;

	threadHit = true;
	playerParam.nor = vector3(0, 1, 0);
	drawMatrix = RCMatrix4::Identity();
	//補間用
	angleF = 0;
	cameraMatrix = RCMatrix4::Identity();
	playerParam.onGroundFlag = false;
	playerParam.vec = vector3(0, 0, 0);
	parameter.moveVec = vector3(0, 0, 0);
	playerParam.angleMat = RCMatrix4::Identity();
	RCMatrix4::setLeft(playerParam.angleMat, vector3(1, 0, 0));
	RCMatrix4::setUp(playerParam.angleMat, vector3(0, 1, 0));
	RCMatrix4::setFront(playerParam.angleMat, vector3(0, 0, 1));

	//テスト描画用のフラグ
	hitFlag = false;
	currentAI = FIRST_AI;
	thisCopy = this;

	playerParam.hp = 2;
	hpHealTime = 60;
	damageCoolTime = 0;

	if (cID == CAMERA_ID::PLAYER_CAMERA_1P){
		parameter.id = ACTOR_ID::PLAYER_ACTOR;
		inputAI = false;
	}
	else if ((int)cID <= CAMERA_ID::PLAYER_CAMERA_4P){
		parameter.id = ACTOR_ID::PLAYER_ACTOR;
		inputAI = true;
	}
	else{
		parameter.id = ACTOR_ID::ENEMY_ACTOR;
		inputAI = true;
	}

	//リスポーン地点
	//pos = vector3(1.2f, 0.83f, 91.46f);
	if (parameter.id == ACTOR_ID::PLAYER_ACTOR){
		pos = respawnPoint = vector3(-3.2f + (1.2f * (playerNum)), 0.80f, -92.5f);
		//angleF = Math::PI;//初期水平方向の向きを反対に
		playerParam.angleMat = RCMatrix4::rotateY(180.0f);
		parameter.matrix = RCMatrix4::scale(vector3(0.1f, 0.1f, 0.1f))* RCMatrix4::rotateY(180.0f) * RCMatrix4::translate(pos);
	}
	else{
		pos = respawnPoint = vector3(-2.4f + (1.2f * playerNum - 4), 0.83f, 91.46f);
		parameter.matrix = RCMatrix4::scale(vector3(0.1f, 0.1f, 0.1f)) * RCMatrix4::translate(pos);
	}
	respawnTimer = 0.0f;
	isRespawn = false;
	stageOut = false;

	nodamageTimer = 0.0f;
	isNodamage = false;

	bindFlag = false;
	bindTime = 0;

	inputCurlOn = false;
	inputCurlOff = false;
	inputJump = false;
	inputStep = false;
	inputShot = false;

	Device::GetInstance().GetCamera(cID)->Initialize();
	//	playerAction.Initialize(shared_from_this());
	if (firstFlag){
		pAM.Initialize(world, shared_from_this(), padNum, cID);
		ai[AI_STATE::FIRST_AI] = std::make_shared<FirstAI>(world, pAM, shared_from_this(), cID, parameter, stage);
		firstFlag = false;
	}
	pAM.ChangeAction(ACTION_ID::HOVER_ACTION);
	//	playerAction.ChangeAction(ACTION_ID::HOVER_ACTION);
	//Graphic::GetInstance().SetAnimation(this->shared_from_this(), ANIM_ID::NEPHILA_WAIT_ANIM, 50.0f,true,);

	playerParam.hp = 2;
	hpHealTime = 60;
	damageCoolTime = 0;

	if (cID == CAMERA_ID::PLAYER_CAMERA_1P){
		parameter.id = ACTOR_ID::PLAYER_ACTOR;
		inputAI = false;
	}
	else if ((int)cID <= CAMERA_ID::PLAYER_CAMERA_4P){
		parameter.id = ACTOR_ID::PLAYER_ACTOR;
		inputAI = true;
	}
	else{
		parameter.id = ACTOR_ID::ENEMY_ACTOR;
		inputAI = true;
	}
	respawnTimer = 0.0f;
	isRespawn = false;

	nodamageTimer = 0.0f;
	isNodamage = false;
}
void Player::Update(float frameTime){
	if (playerNum == 0)
	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 400), vector2(0.20f, 0.25f), 0.5f, "ONGROUNDFLAG:" + std::to_string(RCMatrix4::getPosition(parameter.matrix).y));
//	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_G, true) && playerNum != 0){
//		if (playerNum != 4)
//		parameter.isDead = true;
//		dokusai = true;
//		inputAI = false;
//		Device::GetInstance().GetCamera(cID)->SetDokusai();
//	}
//	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_H, true) && playerNum == 0){
//		inputAI = true;
//		playerAI = true;
//		Device::GetInstance().GetCamera(cID)->SetPlayerAI();
//	}
	Device::GetInstance().GetCamera(cID)->SetIsRespawn(isRespawn);

	//リスポーン中は以降の処理を行わない
	if (isRespawn)
	{
		if (pAM.ReturnThread()._Get() != nullptr){
			pAM.ReturnThread()._Get()->SetIsDead(true);
		}
		bindTime = 10000.0f;
		if (respawnTimer < 0.01f){
			pAM.SetAnimation(
				(ANIM_ID)(ANIM_ID::NEPHILA_DEAD_ANIM + (parameter.id == ACTOR_ID::PLAYER_ACTOR ? 0 : ANIM_ID::CENTER)),
				ANIM_ID::NEPHILA_DEAD_ANIM, 30.0f, true, false, 0, 5.0f);
		}
		respawnTimer += frameTime;
		//とりあえず丸まり
		//3秒経過でリスポーン
		if (respawnTimer >= 3.0f)
		{
			Initialize();
			Graphic::GetInstance().DeleteBackAnimation(shared_from_this());
			respawnTimer = 0.0f;
			//無敵化
			isNodamage = true;			
		}
	}

	if (RCMatrix4::getPosition(parameter.matrix).y < -9.0f || 
		RCMatrix4::getPosition(parameter.matrix).y > 18.0f)stageOut = true;
	if (stageOut){
		Initialize();
		respawnTimer = 0.0f;
		//無敵化
		isNodamage = true;
		stageOut = false;
	}

	//カメラのパラメータを取得
	CAMERA_PARAMETER c = *Device::GetInstance().GetCamera(cID)->CameraParam();

	

	//無敵状態のとき
	if (isNodamage)
	{
		nodamageTimer += 10.0f* frameTime;
		if (nodamageTimer > 40.0f)
		{
			isNodamage = false;
			nodamageTimer = 0.0f;
		}
	}
	//if (dokusai && !playerAI)
	//playerParam.vec = Control(frameTime, c);
	////入力を取得
	//else
	pAM.Control();
	if (stage._Get()->ReturnStartTime() < 0)
	{
		if (padNum == 0)
			playerParam.vec = Control(frameTime, c);
		else
		{
			ai[currentAI]->UpdateAI(frameTime);
			Control(frameTime, c);
		}
	}

	//if (playerAI){
	//	ai[currentAI]->UpdateAI(frameTime);
	//	Control(frameTime, c);
	//}


	//移動ベクトルを初期化
	parameter.inertiaVec = parameter.moveVec;
	parameter.moveVec = vector3(0, 0, 0);
	if (!isRespawn)
	pAM.Update(frameTime);
	//無敵じゃなければ敵の糸と判定
	if (!isNodamage)
	{
		world.SetCollideSelect(shared_from_this(),
			parameter.id == ACTOR_ID::PLAYER_ACTOR ? ACTOR_ID::ENEMY_THREADBULLET_ACTOR : ACTOR_ID::PLAYER_THREADBULLET_ACTOR,
			COL_ID::SPHERE_LINE_COLL);
		world.SetCollideSelect(shared_from_this(),
			parameter.id == ACTOR_ID::PLAYER_ACTOR ? ACTOR_ID::ENEMY_THREADWEB_ACTOR : ACTOR_ID::PLAYER_THREADWEB_ACTOR,
			COL_ID::TRIANGLE_SPHERE_COLL);
		world.SetCollideSelect(shared_from_this(),
			parameter.id == ACTOR_ID::PLAYER_ACTOR ? ACTOR_ID::ENEMY_THREAD_ACTOR : ACTOR_ID::PLAYER_THREAD_ACTOR,
			COL_ID::SPHERE_LINE_COLL);
	}


	//playerAction.UpdateAction(frameTime);

	Matrix4 sizeMat, tranMat;
	float s = 0.1f;
	//サイズセット
	sizeMat = RCMatrix4::scale(vector3(s, s, s));

	//回転のマトリックスセット
	float angle = Math::angle(c.AngleH - angleF);
	//if (playerAction.CameraDrawChangeOK())
	{
		nor = playerParam.nor;
	}
	if (pAM.ReturnPlayerControl().firstFrameFlag){
		paramMat = parameter.matrix;
	}
	else if (pAM.ReturnPlayerControl().secondFrameFlag){
		parameter.matrix = paramMat;
		pAM.SetSecondFrameFlag(false);
	}
	playerParam.angleMat = NorToRotateMat(c, nor, parameter.matrix, angle);

	if (bindFlag){
		if (threadHit)
			parameter.moveVec *= BINDPOW_LINE;
		else
			parameter.moveVec *= BINDPOW_WEB;
		if (pAM.ReturnActionID() == ACTION_ID::AIR_CURL_ACTION || pAM.ReturnActionID() == ACTION_ID::GROUND_CURL_ACTION)
			bindTime -= 4.0f * frameTime;
		else
			bindTime -= 1.0f * frameTime;
		if (bindTime < 0){
			bindFlag = false;
		}
	}

	//移動させる
	pos += parameter.moveVec;
	//トランスフォーム取得
	tranMat = RCMatrix4::translate(pos);
	//マトリックス完成
	parameter.matrix = sizeMat *playerParam.angleMat * tranMat;
	//描画用マトリックス完成
	//描画用に編集

	Matrix4 drawAngleMat = playerParam.angleMat;
	{
		//丸まっていたら
		if (pAM.ReturnActionID() == ACTION_ID::GROUND_CURL_ACTION){
			//進行方向と上からレフトを求め
			Vector3 newLeft = RCVector3::cross(nor, RCVector3::normalize(parameter.moveVec));

			//そのレフトを軸に回転させるmatrixを作り
			drawAngleMat = drawAngleMat * RCQuaternion::rotate(newLeft, curlAngle);
			//長さに応じて回転させる
			curlAngle += RCVector3::length(parameter.moveVec) * 10000.0f * frameTime;

			//ちょっと上にあげる
			tranMat = RCMatrix4::translate(pos + (RCVector3::normalize(RCMatrix4::getUp(parameter.matrix)) * 0.1f));
		}
		else if (pAM.ReturnActionID() == ACTION_ID::THREAD_ACTION){
			//そのレフトを軸に回転させるmatrixを作り
			Vector3 cameraVec = RCVector3::normalize(RCMatrix4::getFront(playerParam.angleMat));
			Vector3 ghostFront = RCVector3::normalize(pAM.ReturnThread()._Get()->GetThreadParameter().startNormal);
			float ang = Math::acos(RCVector3::dot(cameraVec, ghostFront));
			Vector3 newFront = ghostFront;
			for (int i = 1; i < 4; i++){
				if (ang > Math::acos(RCVector3::dot(cameraVec, RCVector3::normalize(ghostFront * RCQuaternion::rotate(nor, 90 * i))))){
					newFront = RCVector3::normalize(ghostFront*RCQuaternion::rotate(nor, 90 * i));
					ang = Math::acos(RCVector3::dot(cameraVec, RCVector3::normalize(ghostFront * RCQuaternion::rotate(nor, 90 * i))));
				}
			}
			Vector3 newLeft = RCVector3::cross(newFront, RCVector3::normalize(nor));
			Matrix4 angleMat;
			//回転行列を求める
			angleMat = RCMatrix4::Identity();
			RCMatrix4::setLeft(angleMat, RCVector3::normalize(-newLeft));
			RCMatrix4::setUp(angleMat, RCVector3::normalize(nor));
			RCMatrix4::setFront(angleMat, RCVector3::normalize(newFront));
			drawAngleMat = angleMat;
		}
		else if (pAM.ReturnActionID() == ACTION_ID::REWIND_ACTION){
			Vector3 rewNor = nor;
			//そのレフトを軸に回転させるmatrixを作り
				Vector3 newFront = RCVector3::normalize(pAM.ReturnThread()._Get()->GetThreadParameter().startNormal);
			if (!pAM.ReturnPlayerControl().isGoingEnd){
				newFront = RCVector3::normalize(pAM.ReturnThread()._Get()->GetThreadParameter().endNormal);
			}
			if (pAM.ReturnBeforeActionID() == ACTION_ID::DANGLE_ACTION && pAM.ReturnThread()._Get()->GetThreadParameter().endNormal.y > 0){
				rewNor = RCVector3::normalize(RCMatrix4::getFront(parameter.matrix));
			}
			Vector3 newLeft = RCVector3::cross(newFront, RCVector3::normalize(rewNor));

			Matrix4 angleMat;
			angleMat = RCMatrix4::Identity();
			RCMatrix4::setUp(angleMat, -RCVector3::cross(RCVector3::normalize(-newLeft), RCVector3::normalize(newFront)));
			RCMatrix4::setLeft(angleMat, RCVector3::normalize(-newLeft));
			//回転行列を求める
			RCMatrix4::setFront(angleMat, RCVector3::normalize(newFront));

			drawAngleMat = angleMat;
		}
		else{
			curlAngle = 0;
		}
	}
	//playerAction.CameraDrawChangeOK() || playerAction.ReturnActionID() == ACTION_ID::JUMP_ACTION || playerAction.ReturnActionID() == ACTION_ID::HOVER_ACTION)
	if (!pAM.ReturnPlayerControl().firstFrameFlag)// || padNum != 0)
	{
		//描画用マトリックス完成
		drawMatrix = sizeMat *drawAngleMat * tranMat;
		cameraMatrix = parameter.matrix;
	}

	//テスト用
	{
		hitFlag = playerParam.onGroundFlag/*ここに条件を書くと表示される*/;
		std::string flagstring = hitFlag == true ? "TRUE" : "FALSE";
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 400), vector2(0.20f, 0.25f), 0.5f, "ONGROUNDFLAG:" + flagstring);
		hitFlag = playerParam.onThreadFlag/*ここに条件を書くと表示される*/;
		flagstring = hitFlag == true ? "TRUE" : "FALSE";
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 385), vector2(0.20f, 0.25f), 0.5f, "ONTHREADFLAG:" + flagstring);
		hitFlag = false;
	}
	if (padNum == 0){
		//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 600), vector2(0.20f, 0.25f), 0.5f, "PlayerHP:" + std::to_string(hp));
	//	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 520), vector2(0.20f, 0.25f), 0.5f, "IsBind:" + std::to_string(bindTime));
	}

	playerParam.onGroundFlag = false;
	playerParam.onThreadFlag = false;
	std::string st;
	st[0] = std::to_string(1).c_str()[0];
	//カメラにマトリックスを渡す
	Device::GetInstance().GetCamera(cID)->SetMatrix(cameraMatrix);

	damageCoolTime = max(damageCoolTime--, 0);
	hpHealTime = max(hpHealTime--, 0);
	//HP自動回復
	/*if (hpHealTime == 0)
	hp = min(hp+0.5f, 2.0f);*/
	playerParam.hp += 0.2f * frameTime;
	playerParam.hp = min(playerParam.hp, 2.0f);
	frameTime_ = frameTime;
}

//描画
void Player::Draw(CAMERA_ID cID_) const{
	//無敵時間中は0.1秒間隔で点滅
	if (isNodamage && abs(Math::sin(Math::angle(nodamageTimer))) < 0.5f) return;
	else{
		//プレイヤー用のシェーダーセット
		Graphic::GetInstance().SetShader(SHADER_ID::PLAYER_SHADER);
		//テクニックとパスをセット
		Graphic::GetInstance().SetTechniquePass(SHADER_ID::PLAYER_SHADER, "TShader", "P0");
		//アニメーションをバインド
		//if (cID_ == CAMERA_ID::PLAYER_CAMERA){
		//	Graphic::GetInstance().BindAnimation(thisCopy->shared_from_this(), SHADER_ID::PLAYER_SHADER, frameTime_);
		//	if (parameter.id == ACTOR_ID::PLAYER_ACTOR){
		//		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 585), vector2(0.20f, 0.25f), 0.5f, "PlayerPos.x:" + std::to_string(pos.x) + "f");
		//		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 570), vector2(0.20f, 0.25f), 0.5f, "PlayerPos.y:" + std::to_string(pos.y) + "f");
		//		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 555), vector2(0.20f, 0.25f), 0.5f, "PlayerPos.z:" + std::to_string(pos.z) + "f");
		//	}
		//	else{
		//		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 540), vector2(0.20f, 0.25f), 0.5f, "EnemyPos.x:" + std::to_string(pos.x) + "f");
		//		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 525), vector2(0.20f, 0.25f), 0.5f, "EnemyPos.y:" + std::to_string(pos.y) + "f");
		//		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 510), vector2(0.20f, 0.25f), 0.5f, "EnemyPos.z:" + std::to_string(pos.z) + "f");
		//	}
		//}
		//else
		Graphic::GetInstance().BindAnimation(thisCopy->shared_from_this(), SHADER_ID::PLAYER_SHADER, frameTime_);
		//プレイヤーを描画
		Graphic::GetInstance().DrawMesh(parameter.id == ACTOR_ID::PLAYER_ACTOR ? MODEL_ID::NEPHILA_MODEL : MODEL_ID::TARENTULE_MODEL, &drawMatrix, cID_);
	}
	//テストフォント
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "PlayerHP:" + std::to_string(hp));
	
//	if (bindFlag)
//		Graphic::GetInstance().DrawSphere(RCMatrix4::getPosition(parameter.matrix), 0.5f, cID_);
}

void Player::OnCollide(Actor& other, CollisionParameter colpara){
	pAM.Collide(other, colpara);
	//相手と接触したら
	if ((parameter.id == ACTOR_ID::PLAYER_ACTOR && colpara.id == COL_ID::PLAYER_ENEMY_COLL) ||
		(parameter.id == ACTOR_ID::ENEMY_ACTOR && colpara.id == COL_ID::ENEMY_PLAYER_COLL)){
		if (pAM.ReturnActionID() != ACTION_ID::THREAD_ACTION)
			pos = colpara.colPos;
	}

	if (colpara.id == COL_ID::RAY_MODEL_NATURAL_COLL || colpara.id == COL_ID::SPHERE_MODEL_NATURAL_COLL)
		playerParam.onGroundFlag = true;
	else if (colpara.id == COL_ID::SPHERE_LINE_COLL && other.GetParameter().id == (parameter.id == ACTOR_ID::PLAYER_ACTOR ? ACTOR_ID::PLAYER_THREAD_ACTOR : ACTOR_ID::ENEMY_THREAD_ACTOR))
		playerParam.onThreadFlag = true;
	//playerAction.CollideAction(other, colpara);


	//敵の糸と蜘蛛の巣以外と接触ならonGroundをtrue。
	/*if (!((colpara.id == COL_ID::SPHERE_LINE_COLL || colpara.id == COL_ID::TRIANGLE_LINE_COLL || colpara.id == COL_ID::TRIANGLE_LINE_JUMP_COLL) && colpara.enemyFlag))
	{
	playerParam.onGroundFlag = true;
	}*/
	//当たっているのが敵の糸なら。
	if (colpara.id == COL_ID::SPHERE_LINE_COLL &&
		other.GetParameter().id == (parameter.id == ACTOR_ID::PLAYER_ACTOR ? ACTOR_ID::ENEMY_THREAD_ACTOR : ACTOR_ID::PLAYER_THREAD_ACTOR) &&
		!static_cast<Thread*>(&other)->IsShot()){
		if (!bindFlag){
			bindFlag = true;
			threadHit = true;
			bindTime = 1.0f;
			other.SetIsDead(true);
			world.Add(ACTOR_ID::THREAD_EFFECT_ACTOR, std::make_shared<ThreadEffect>(world,drawMatrix,bindFlag,cID));
		}
		//Thread* otherT = static_cast<Thread*>(const_cast<Actor*>(&other));
		////ショット中の糸ならばダメージ。
		//if (otherT->IsShot() && damageCoolTime == 0){
		//	hp = max(hp--, 0.0f);
		//	damageCoolTime = 30;//とりあえず30フレーム
		//}
	}
	//当たっているのが蜘蛛の巣。
	else if ((colpara.id == COL_ID::TRIANGLE_SPHERE_COLL) && other.GetParameter().id == (parameter.id == ACTOR_ID::PLAYER_ACTOR ? ACTOR_ID::ENEMY_THREADWEB_ACTOR : ACTOR_ID::PLAYER_THREADWEB_ACTOR)){
		//自分の蜘蛛の巣なら。
		if (!bindFlag){
			bindFlag = true;
			threadHit = false;
			bindTime = 4.0f;
			static_cast<ThreadWeb*>(const_cast<Actor*>(&other))->PlusHitCount(3);
			world.Add(ACTOR_ID::THREAD_EFFECT_ACTOR, std::make_shared<ThreadEffect>(world, drawMatrix, bindFlag, cID));
		}
		//if (!colpara.enemyFlag){
		//	//座標をヒットした地点へ移動
		//	//pos = colpara.colPos;
		//	////現在の上向きに近い面に乗っかるように補正
		//	//Vector3 up = RCVector3::normalize(RCMatrix4::getUp(parameter.matrix));
		//	//float a = Math::acos(RCVector3::dot(up, colpara.colNormal));
		//	//float b = Math::acos(RCVector3::dot(up, -colpara.colNormal));
		//	////上方向をセット
		//	//if (a < b)playerParam.nor = colpara.colNormal;
		//	//else playerParam.nor = -colpara.colNormal;
		//}
		////敵の蜘蛛の巣なら
		//else{
		//}
	}
}

//入力
Vector3 Player::Control(float frameTime, CAMERA_PARAMETER c){
	//移動量の初期化
	Vector3 vec = vector3(0, 0, 0);
	//パッドの入力があるか
	bool padFlag = false;

	//斜め時のアニメーションを初期化
	ANIM_ID DiagonallyAnimID = ANIM_ID::NULL_ANIM;

	//丸まりボタンが離されたら
	if ((!inputAI && (Device::GetInstance().GetInput()->KeyUp(INPUTKEY::KEY_LCTRL)
		|| Device::GetInstance().GetInput()->GamePadButtonUp(padNum, GAMEPADKEY::BUTTON_CURCLE)
		|| Device::GetInstance().GetInput()->GamePadButtonUp(padNum, GAMEPADKEY::BUTTON_L2))
		) || inputCurlOff){
		if (playerParam.onGroundFlag)
			pAM.ChangeAction(ACTION_ID::NORMAL_ACTION);
		else
			pAM.ChangeAction(ACTION_ID::HOVER_ACTION);
	}
	vec = Device::GetInstance().GetInput()->LeftStick(padNum) / 10000.0f;
	if (!(vec == vector3(0, 0, 0)))padFlag = true;
	//もしAが押されたら
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_A)){
		//ベクトルをセット
		vec.x = -1;
	}
	//上の右版
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_D)){
		vec.x = 1;
	}

	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_W)){
		vec.z = -1;
	}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_S)){
		vec.z = 1;
	}

	//丸まりの処理はこちら とりあえずゆっくり動くようにはできた
	if (((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LCTRL, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_L2,true)) 
		&& !inputAI)
		|| inputCurlOn){
		pAM.ChangeAction(ACTION_ID::CURL_ACTION);
	}
	//スペースが押されたかつジャンプ中じゃなかったら
	if ((((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R3, true) &&
		!inputAI)
		|| inputJump) &&
		!pAM.ReturnPlayerActionPtr()->ReturnRide())
		&& pAM.ReturnActionID() != ACTION_ID::JUMP_ACTION &&
		!pAM.ReturnPlayerActionPtr()->ReturnHitGround()){

		pAM.ChangeAction(ACTION_ID::JUMP_ACTION);
	}

	//ステップの処理はこちらへ
	if ((((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LSHIFT, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_L3, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_SELECT, true)) && !inputAI)
		&& !(vec == vector3(0, 0, 0)))
		|| inputStep){
		if (pAM.ReturnActionID() != ACTION_ID::THREAD_ACTION && pAM.ReturnActionID() != ACTION_ID::THREAD_WEB_ACTION)
			pAM.ChangeAction(ACTION_ID::STEP_ACTION);
		else{
			//pAM.ChangeAction(ACTION_ID::THREAD_STEP_ACTION);
		}
	}

	//左クリックされたら糸発射
	if (((!inputAI && (Device::GetInstance().GetInput()->MouseButtonDown(INPUTMOUSEBUTTON::LEFT_BUTTON, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(padNum, GAMEPADKEY::BUTTON_R1, true))) || inputShot) &&
		pAM.ReturnActionID() != ACTION_ID::GROUND_CURL_ACTION &&
		pAM.ReturnActionID() != ACTION_ID::AIR_CURL_ACTION){
		std::shared_ptr<Thread> thread = std::make_shared<Thread>(world, shared_from_this(), stage, cID, (pAM.ReturnActionID() == ACTION_ID::THREAD_ACTION || pAM.ReturnActionID() == ACTION_ID::THREAD_WEB_ACTION) ? false : true);
		//それぞれのチームのIDの糸を生成。
		ACTOR_ID threadID = ACTOR_ID::PLAYER_THREAD_ACTOR;
		if (parameter.id != ACTOR_ID::PLAYER_ACTOR)
			threadID = ACTOR_ID::ENEMY_THREAD_ACTOR;

		world.Add(threadID, thread);

		if (((!playerParam.onGroundFlag && !playerParam.onThreadFlag) || pAM.ReturnActionID() == ACTION_ID::DANGLE_ACTION) && pAM.ReturnActionID() != ACTION_ID::THREAD_WEB_ACTION){
			if (!thread->IsDead()){
				if (pAM.ReturnActionID() == ACTION_ID::DANGLE_ACTION)
					pAM.SetThread(thread, true);
				else
					pAM.SetThread(thread);
				pAM.ChangeAction(ACTION_ID::DANGLE_ACTION);
				thread->SetDangleFlag(true);
			}
		}
	}

	//本来の目的を半分忘れかけたが入力を返す
	if (padFlag)
		return vec;
	else
		return RCVector3::normalize(vec);
}

//マトリックスを返す
Matrix4* Player::ReturnMat(){
	return &parameter.matrix;
}
//ポジションをセット
void Player::SetPos(Vector3 position){
	pos = position;
}

//移動ベクトルをセット
void Player::SetMoveVec(Vector3 moveVec){
	parameter.moveVec += moveVec;
}

//ノーマルからRotateを計算
Matrix4 Player::NorToRotateMat(CAMERA_PARAMETER c, Vector3 normal, Matrix4 matrix, float angle){
	Matrix4  angleMat;

	//まずUpを軸にyawの回転をさせる
	auto fm = RCQuaternion::rotate(normal, angle);
	//差分用（player.hみて）
	angleF = c.AngleH;
	//今のフロントにさっきの回転を掛け高さを無視した前を求める
	auto dfv = RCMatrix4::getFront(matrix) * fm;

	//上と高さを無視した前から外積を用いて左を求める 前の方向が高さ無視しても上と垂直な直線って変わらないんです、理解してね
	auto lv = RCVector3::cross(RCVector3::normalize(dfv), normal);
	//正しい左と上から正しい前を求める
	auto fv = RCVector3::cross(normal, lv);

	//回転行列を求める
	angleMat = RCMatrix4::Identity();
	RCMatrix4::setLeft(angleMat, RCVector3::normalize(-lv));
	RCMatrix4::setUp(angleMat, RCVector3::normalize(normal));
	RCMatrix4::setFront(angleMat, RCVector3::normalize(fv));

	return angleMat;
}


//プレイヤーパラメータを返す
PlayerParameter* Player::ReturnPlayerParameter(){
	return &playerParam;
}

//上方向をセット
void Player::SetNor(Vector3 nor_){
	playerParam.nor = nor_;
}

void Player::ShotThread(){
	std::shared_ptr<Thread> thread = std::make_shared<Thread>(world, shared_from_this(), stage, cID,pAM.ReturnActionID() == ACTION_ID::THREAD_ACTION ? false : true);
	//それぞれのチームのIDの糸を生成。
	ACTOR_ID threadID = ACTOR_ID::PLAYER_THREAD_ACTOR;
	if (teamID == TEAM_ID::SECOND_TEAM)
		threadID = ACTOR_ID::ENEMY_THREAD_ACTOR;
	
	world.Add(threadID, thread);
	Audio::GetInstance().PlaySE(SE_ID::BOOM_SE);
}

TEAM_ID Player::GetTeam()
{
	return teamID;
}

bool Player::IsBind(){
	return true;// bindTime != 0;
}

float Player::BindPow(float pow){
	if (IsBind())
		return pow;
	return 1.0f;
}

float Player::BindPow(bool rec){
	float pow = 1.0f;
	//if (hitEnemyThreadID == ACTOR_ID::ENEMY_THREAD_ACTOR)
	//	pow = BINDPOW_LINE;
	//if (hitEnemyThreadID == ACTOR_ID::ENEMY_THREADWEB_ACTOR)
	//	pow = BINDPOW_WEB;
	//
	////recがtrueなら逆数にする。
	//if (rec)
	//	pow = 1.0f / pow;

	return pow;
}

void Player::SetAnimTime(float time){
	pAM.SetAnimTime(time);
}
void Player::SetAnimBlend(float blend){
	pAM.SetAnimBlend(blend);
}
void Player::SetAnimation(ANIM_ID firstActionId, ANIM_ID secondActionId, float animUpdateTime, bool animTimeReset, bool roop, float blendLevel, float beforeAnimBlendLevel){
	pAM.SetAnimation(firstActionId, secondActionId, animUpdateTime, animTimeReset, roop, blendLevel, beforeAnimBlendLevel);
}

void Player::SetThread(std::weak_ptr<Thread> thread_){
	pAM.SetThread(thread_);
}
std::weak_ptr<Thread> Player::ReturnThread(){
	return pAM.ReturnThread();
}

void Player::SetThreadWeb(std::weak_ptr<ThreadWeb> threadWeb_){
	pAM.SetThreadWeb(threadWeb_);
}
std::weak_ptr<ThreadWeb> Player::ReturnThreadWeb(){
	return pAM.ReturnThreadWeb();
}

void Player::Damage(float damagePoint){
	playerParam.hp -= damagePoint;

	if (playerParam.hp <= 0 && !isRespawn)
	{
		stage._Get()->AddPoint(parameter.id);
		isRespawn = true;
	}
}
void Player::SetBindTime(float bindTime_){ 
	if (!bindFlag){ 
		bindFlag = true; bindTime = bindTime_; 
		threadHit = true;
		world.Add(ACTOR_ID::THREAD_EFFECT_ACTOR, std::make_shared<ThreadEffect>(world, drawMatrix, bindFlag, cID));
	} 
	else bindTime += bindTime_; 
}
void Player::SetInputCurlOn(bool set){
	inputCurlOn = set;
}
void Player::SetInputCurlOff(bool set){
	inputCurlOff = set;
}
void Player::SetInputJump(bool set){
	inputJump = set;
}
void Player::SetInputStep(bool set){
	inputStep = set;
}
void Player::SetInputShot(bool set){
	inputShot = set;
}