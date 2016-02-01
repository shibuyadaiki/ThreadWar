#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Actor/Actor.h"
#include "../Math/Vector2.h"
#define TITLE_SELECT_NUM 2

enum TITLE_SELECT{
	SELECT_GAMESTART,
	SELECT_OPTION,
	SELECT_RETURN,
};

class  TitleScene :public IScene{
public:
	//コンストラクタ 
	TitleScene();
	//デストラクタ
	~TitleScene();

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

	//タイトルセレクト。
	void TitleSelect();
	//チームセレクト。
	void TeamSelect();

	/*タイトルセレクトの2つの選択肢移動。*/
	void DMove();
	void UMove();
	/*******************/

private:
	bool mIsEnd;
	//2つの選択肢のうちの1つが選択されたか。
	bool isSelect;
	World wa;
	//演出の点滅やブランクタイムなどに使用。
	int timer;
	//演出のlerp用。
	float lerpTime;
	//2つの選択肢で選択されたものを格納。
	TITLE_SELECT selects;
	//2つのチームで選択されたものを格納。
	TEAM_ID teamID;

	//選択肢のパラメータ。
	float ts_scale[TITLE_SELECT_NUM];
	float ts_nextScale[TITLE_SELECT_NUM], ts_prevScale[TITLE_SELECT_NUM];
	float ts_alpha[TITLE_SELECT_NUM];
	float ts_nextAlpha[TITLE_SELECT_NUM];

	Vector3 pos;
};