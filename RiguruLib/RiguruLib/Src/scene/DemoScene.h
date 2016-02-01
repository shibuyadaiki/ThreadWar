#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Actor/Actor.h"
#include "../Math/Vector2.h"

class  DemoScene :public IScene{
public:
	//コンストラクタ 
	DemoScene();
	//デストラクタ
	~DemoScene();

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
	bool mIsEnd;
	bool effectEnd, pressStart;
	World wa;
	//演出のブランクタイムに使用。
	int timer;
	//演出のアルファ値用lerp。
	float backLerp, logoLerp, pressLerp;
};