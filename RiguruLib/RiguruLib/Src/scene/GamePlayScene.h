#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Math/Matrix4.h"
#include "../Actor/ID.h"
#include "../Actor/Stage.h"

class  GamePlayScene :public IScene{
public:
	//コンストラクタ 
	GamePlayScene(VICTORY_ID& winner_);
	//デストラクタ
	~GamePlayScene();

	//更新前初期化
	virtual void Initialize() override;
	//更新処理
	virtual void Update(float frameTime)override;
	//描画処理
	virtual void Draw() const override;
	//終了しているか？
	virtual bool IsEnd() const override;
	//次のシーンを返す
	virtual Scene Next() const override;
	//終了時処理
	virtual void End()override;

private:
	std::shared_ptr<Stage> stage;
	bool mIsEnd;
	World wa;
	int bvolume;
	int svolume;
	Matrix4 playerMat, enemyMat;

	//１チームの人数
	const float teamMemberCount = 4;
	CAMERA_ID drawCamera = CAMERA_ID::PLAYER_CAMERA_1P;
	float drawNum = 0;
	VICTORY_ID* winner;
	bool fadeIn, fadeOut;
	float fadeTime;
};