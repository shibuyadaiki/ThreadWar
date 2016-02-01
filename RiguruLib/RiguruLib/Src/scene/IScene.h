#pragma once

enum class Scene;

//シーンインターフェース
class IScene{
public:
	//仮想デストラクタ
	virtual ~IScene(){}
	//更新前処理
	virtual void Initialize() = 0;
	//更新処理
	virtual void Update(float frameTime) = 0;
	//描画処理
	virtual void Draw() const = 0;
	//終了しているか？
	virtual bool IsEnd() const = 0;
	//移行先のシーンを返す
	virtual Scene Next() const = 0;
	//終了時処理
	virtual void End() = 0;
};