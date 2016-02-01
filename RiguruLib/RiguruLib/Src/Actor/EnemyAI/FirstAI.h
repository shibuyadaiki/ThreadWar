#pragma once
#include <array>
#include "EnemyAI.h"
#include "../Base.h"
#include "../../math/Vector3Utility.h"
#include "../../math/Vector2Utility.h"
#include "../../Graphic/Graphic.h"
#include "../Player.h"
#include "../../Math/Quaternion.h"
#include "../../Math/V3Calculation.h"
#include "../../Actor/Stage.h"
#include "../ID.h"
#include "../PlayerAction/PlayerActionManager.h"

//行動
enum AI_ACTION{
	Curl,
	RandomMove,
	RandomLook,
	TurnLook,
	JumpShotRewind,
	FrontLook,
	TargetPointLookH,
	TargetPointLookV,
	TargetPointLook,
	TargetPointLookAttack,
	TargetPointMove,
	TargetPointMoveAttack,
	CrystalPointMoveAttack,
	TargetJumpShotRewind,
	GettingThread,
	CreateThreadWeb,
};

struct AILevel{
	//視野角(自身のflontからの角度)
	float viewAngle;
	//視野距離
	float viewLength;
	//攻撃範囲角(カメラのflontからの角度)
	float attackAngle;
	//攻撃範囲距離
	float attackLength;
	//巻き取って接近する時の最短距離(これより遠い場合は巻き取って接近)
	float rewindLength;
	//理想距離
	float bestLength;
	//強制的に戦闘状態に移行する距離
	float dangerLength;
	//壁が貫通して見える秒数
	float penetrationTime;
	//左右移動を切り替える秒数間隔
	float inputXChangeTimeMax, inputXChangeTimeMin;
	//通常時のステップする秒数間隔
	float stepTimeMax, stepTimeMin;
	//戦闘時のステップする秒数間隔
	float battleStepTimeMax, battleStepTimeMin;
};

//目標の捕捉に関するパラメーター
struct TargetParameter{
	//目標のマトリックス
	Matrix4 mat;
	//目標地点
	Vector3 pos;
	//自分から目標地点への方向
	Vector3 vec;
	//視界内に目標がいるか？
	bool isLooking;
	//最後に視界内に捉えた目標地点
	Vector3 lastLookingPos;
	//最後に視界内に捉えた目標地点への方向
	Vector3 lastLookingVec;
	//自身と目標地点の距離
	float distance;
	//戦闘状態か？(視界内に目標を捉える～見失ってから2秒経過　の間)
	bool isBattle;
	//攻撃範囲内にいるか？
	bool isAttack;

};

class FirstAI : public EnemyAI{
public:
	FirstAI(IWorld& wo, PlayerActionManager& action, std::weak_ptr<Player> player, CAMERA_ID& cID_, ActorParameter& parameter_, std::weak_ptr<Stage> stage_);
	virtual void Initialize() override;
	virtual Vector3 UpdateAI(float frameTime) override;
	virtual void DrawAI() const override{}

private:
	//パラメータ等、AI実行前に必要な計算
	void ParamCalc();
	//AI実行
	void AIRun(float frameTime);
	//入力状態をリセット
	void InputReset();

	//ステートを変える
	void ChangeAction(AI_ACTION s);
	//自身が天井にいるか？
	bool IsCeiling();

	/*********=行動=*********/
	//３秒間丸まる
	void Curl(float frameTime);
	//索敵目的のランダムな移動(斜め前方にランダム移動)
	void RandomMove(float frameTime);
	//索敵目的のランダムな向き変更
	void RandomLook(float frameTime);
	//索敵目的の１方向の向き変更
	void TurnLook(float frameTime);
	//索敵目的の巻き取り高速移動
	void JumpShotRewind(float frameTime);
	//斜め上を向く→ジャンプ→発射→巻取り　天井への移動手段
	void SlantingShotRewind(float frameTime);
	//向きたい縦方向を指定すると向いてくれる
	void LookV(float frameTime, Vector3 v);
	//正面を向く
	void FrontLook(float frameTime);
	//横方向のみ目標地点を向く
	void TargetPointLookH(float frameTime);
	//縦方向のみ目標地点を向く
	void TargetPointLookV(float frameTime);
	//目標地点を向く
	void TargetPointLook(float frameTime);
	//目標地点へ攻撃する
	void TargetPointLookAttack(float frameTime);
	//目標地点へ移動する
	void TargetPointMove(float frameTime);
	//目標地点へ移動しながら攻撃する
	void TargetPointMoveAttack(float frameTime);
	void CrystalPointMoveAttack(float frameTime);
	//目標地点を向きながらジャンプ→発射→巻取り　目標地点への急速接接近手段
	void TargetJumpShotRewind(float frameTime);
	//糸に乗る
	void GettingThread(float frameTime);
	//蜘蛛の巣を作る
	void CreateThreadWeb(float frameTime);

	/*********=索敵=*********/
	//視野内に敵がいるか
	bool Search();
	//視野を表示 
	void SearchLineDraw();
	//攻撃範囲内に敵がいるか
	bool AttackArea();
	//攻撃範囲を表示 
	void AttackAreaLineDraw();

	/*********=入力・操作=*********/
	//丸まる
	void InputCurlOn();
	//丸まりを終了
	void InputCurlOff();
	//ジャンプ
	void InputJump();
	//ステップ
	void InputStep();
	//糸の発射
	void InputShotThread();
	//糸を巻き取る
	void InputRewind();
	//ステップ丸まり(ステップの慣性を受け継いだ丸まり)
	void InputStepCurl();

	//最終的な入力処理
	void FinalInput();

private:
	//プレイヤークラスからもらってくる
	std::weak_ptr<Stage> stage;

private:
	/*****=AI実行=******/
	//タイマー
	float aiTimer;
	//戦闘状態時、視界内に目標を捉えずに経過した時間
	float lostTimer;

	/*****=行動=******/

	//1チームの人数
	const int teamMemberCount = 4;
	//敵4体のマトリックス
	std::array<Matrix4, 4> targetMats;
	//敵4体までの距離
	std::array<float, 4> targetDists;
	//敵
	int targetNum;
	//サーチされる対象(敵のアクターID)
	ACTOR_ID searchActor;

	//目標
	TargetParameter target;

	//クリスタルの位置　中央
	const Vector3 centerCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, -1.5f), vector3(0.5f, 5.5f, 1.5f), 0.5);
	//氷側(PlayerSide)
	const Vector3 iceCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, 62.0f), vector3(0.5f, 5.5f, 65.3f), 0.5);
	//洞窟側(EnemySide)
	const Vector3 caveCrystalPos = RCVector3::lerp(vector3(-2.4f, -1.3f, -67.0f), vector3(0.5f, 5.5f, -64.0f), 0.5);
	//各クリスタルまでの距離
	float centerCrystalDist, iceCrystalDist, caveCrystalDist;
	//一番近い攻撃対象クリスタル
	Vector3 nearestCrystalPos;//位置
	float nearestCrystalDist;//距離
	//クリスタルを狙うか？
	bool targetCrystal;

	//汎用フラグ
	bool flag;
	//汎用タイマー
	float timer;
	//汎用カウンター
	int counter;
	//行動が終わったか？
	bool stateEnd;

	//左右移動切り替え時間、ステップ発生タイマ、糸放出タイマ、ジャンプタイマ
	float moveTime, stepTime, threadTime, jumpTime;
	float moveTimeMax, stepTimeMax, threadTimeMax;
	//左右移動入力値
	float inputVecX;
	float inputCameraX;

	//自身のパラメーター
	Matrix4 myMat;
	Vector3 myPos;
	Vector2 myPos2D;
	PlayerParameter* myParam;

	//カメラ
	Camera* camera;
	//カメラの回転行列
	Matrix4 cameraRotMat;
	Vector3 cameraFront;
	Vector3 cameraLeft;
	Vector3 cameraUp;

	//現在の行動
	AI_ACTION currentAction;
	std::map<AI_ACTION, std::function<void(float)>> actionFunc;

	/*****=入力=******/
	//自身の前後左右移動の入力
	Vector3 inputVec = vector3(0, 0, 0);
	//カメラの方向入力
	Vector2 inputAngle = vector2(0.0f, 0.0f);

	/*****=難易度・AI警戒レベル=******/
	std::array<AILevel, 3> level;
	int levelNum;
};