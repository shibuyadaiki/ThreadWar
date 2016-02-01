#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"
class CrystalCenter;
static float GameOverCount = 300.0f;
static int pointMax = 200;

class Stage :public Actor{
public:
	Stage(IWorld& world_, std::weak_ptr<CrystalCenter> crystalCenter_, std::weak_ptr<CrystalCenter> crystalPlayerSide_, std::weak_ptr<CrystalCenter> crystalEnemySide_,bool bgmPlay);
	~Stage();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	Matrix4* ReturnMat();
	void CrystalDamage(ACTOR_ID threadID, ACTOR_ID crystalID);
	std::weak_ptr<CrystalCenter> ReturnCrystal(ACTOR_ID crystalID);
	void AddPoint(ACTOR_ID id);//id:死んだプレイヤーのもっているparameter.id
	float ReturnGameTime(){ return GameOverCount - gameTimer; }
	int ReturnTeamPoint(){ return teamPoint; }
	int ReturnPlayerTeamPoint(){ return playerPoint; };
	int ReturnEnemyTeamPoint(){ return enemyPoint; };
	int ReturnTotalPoint(){ return totalPoint; };
	float ReturnStartTime(){ return startTime; }
	VICTORY_ID ReturnWinner();
private:
	void CountDown(float count);
	float angle = 0;
	float angleZ = 0;
	float bvolume = 50;
	float svolume = 50;
	float animCount = 0;
	float yAngle;
	std::weak_ptr<CrystalCenter> crystalCenter, crystalPlayerSide, crystalEnemySide;

	//加算されるポイント
	int addPoint = 0;
	//チームのポイント(0～200、200に近いほどプレイヤーチームが有利、0に近いほどエネミーチームが有利)
	int teamPoint = 100;
	int playerPoint = 0;
	int enemyPoint = 0;
	int totalPoint = 0;
	//クリスタルの状態をリセットするための時間
	float crystalTimer = 0;
	//ゲームの経過時間
	float gameTimer = 0;
	//ゲーム終了かどうか
	bool gameEnd = false;
	////プレイヤーの勝利かどうか（true:プレイヤーチーム勝利、false:エネミーチーム勝利）
	//bool playerTeamWin = true;
	////引き分けかどうか
	//bool draw = false;
	//勝利したチーム。
	VICTORY_ID winner;
	bool gameStart;
	float startTime;
	bool bgmPlay;
};