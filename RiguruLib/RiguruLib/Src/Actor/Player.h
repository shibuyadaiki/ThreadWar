#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "ID.h"
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"
#include "PlayerAction\PlayerActionManager.h"
struct CAMERA_PARAMETER;
class EnemyAI;
class Stage;
class ThreadWeb;
enum AI_STATE;
class Thread;

struct PlayerParameter{
	Vector3 vec;
	Matrix4 angleMat;
	//設置してるかどうか
	bool onGroundFlag;
	bool onThreadFlag;
	Vector3 nor;
	float hp;
};

class Player :public Actor, public std::enable_shared_from_this<Player>{
public:
	Player(IWorld& world_, std::weak_ptr<Stage> stage_,CAMERA_ID cID_,int padNum_,int playerNum_);
	~Player();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//マトリックスを返す
	Matrix4* ReturnMat();
	//ポジションをセット
	void SetPos(Vector3 position);
	//移動ベクトルをセット
	void SetMoveVec(Vector3 moveVec);
	//プレイヤーパラメータを返す
	PlayerParameter* ReturnPlayerParameter();
	//上方向をセット
	void SetNor(Vector3 nor_);
	//プレイヤのチームIDを取得。
	TEAM_ID GetTeam();
	//糸の発射。
	void ShotThread();
	//拘束時間内か。
	bool IsBind();
	//現在拘束されているかを判定し、拘束されていたら引数を返す。
	float BindPow(float pow);
	//現在どのIDによって拘束されているか判定し、IDごとに設定された行動の減衰値を返す。
	//拘束されていなければ1.0が返り、減衰しない。recは逆数かどうか。
	float BindPow(bool rec = false);

	float* animSpeed;
	float* blendSpeed;
	void SetAnimTime(float time);
	void SetAnimBlend(float blend);
	void SetAnimation(ANIM_ID firstActionId, ANIM_ID secondActionId, float animUpdateTime, bool animTimeReset, bool roop, float blendLevel, float beforeAnimBlendLevel);
	void SetThread(std::weak_ptr<Thread> thread_);
	std::weak_ptr<Thread> ReturnThread();
	void SetThreadWeb(std::weak_ptr<ThreadWeb> threadWeb_);
	std::weak_ptr<ThreadWeb> ReturnThreadWeb();
	void SetBindFlag(bool flag){ bindFlag = flag; }
	void SetBindTime(float bindTime_);
	bool ReturnBindFlag(){ return bindFlag; }
	float ReturnBindTime(){ return bindTime; }
	int ReturnPlayerNum(){ return playerNum; }

	void Damage(float damagePoint);
private:
	PlayerParameter playerParam;
	PlayerActionManager pAM;
	//入力
	Vector3 Control(float frameTime, CAMERA_PARAMETER c);
	//ノーマルからRotateを計算
	Matrix4 NorToRotateMat(CAMERA_PARAMETER c, Vector3 normal, Matrix4 matrix, float angle);

	Player* thisCopy;

	//bool dokusai,playerAI;

private:
	//PlayerAction playerAction;
	/********基本パラメータ********/
	//ステージのポインタ、Threadに送る用
	std::weak_ptr<Stage> stage;
	//テスト表示用のフラグ
	bool hitFlag;
	//プレイヤーのポジション
	Vector3 pos;
	//前の回転に足していくので差分を求めるもの
	float angleF;
	//描画用マトリックス
	Matrix4 drawMatrix;
	//カメラ用のマトリックス
	Matrix4 cameraMatrix;

	float curlAngle;

	Vector3 nor;

	CAMERA_ID cID;

	int padNum;

	Matrix4 paramMat;

	int hpHealTime;//HPの自然回復タイマ。
	int damageCoolTime;//ダメージのクールタイム。
	int playerNum;//プレイヤー番号 4vs4 0～7まで

	Vector3 respawnPoint;//リスポーンする座標
	float respawnTimer;//リスポーンするまでの時間計測
	bool isRespawn;//リスポーンするか？
	bool stageOut;

	float nodamageTimer;//リスポーン直後の無敵時間
	bool isNodamage;//無敵時間か？


	/**************糸****************/
	bool bindFlag;
	float bindTime;
	bool threadHit;
	float frameTime_;

	Vector3 startNor;
	Vector3 dangleStartFront;
	float dangleRotate;
	/**************AI****************/
	AI_STATE currentAI;
	std::map<AI_STATE, std::shared_ptr<EnemyAI>> ai;
	TEAM_ID teamID;
	Vector3 targetPos;

	/*************入力***************/
public:
	void SetInputCurlOn(bool set);
	void SetInputCurlOff(bool set);
	void SetInputJump(bool set);
	void SetInputStep(bool set);
	void SetInputShot(bool set);
private:
	//AIで使用
	bool inputAI;
	bool inputCurlOn;
	bool inputCurlOff;
	bool inputJump;
	bool inputStep;
	bool inputShot;

	bool firstFlag;
};