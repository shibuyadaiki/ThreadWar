#include "DemoScene.h"
#include "Scene.h"
#include "../actor/Player.h"
#include "../Other/Device.h"
#include "../Graphic/Graphic.h"
#include "../Math/MathUtility.h"
#include "../Math/Vector2Utility.h"
#include "../Actor/Stage.h"
#include "../Actor/CrystalCenter.h"
#include "../Math/Converter.h"
#include "../Audio/Audio.h"

#define SCREEN_CENTER_X 1920/2
#define SCREEN_CENTER_Y 1080/2

//各演出にかかる時間
#define BACK_ALPHA_TIME 20
#define BACK_BLANK_TIME 30
#define LOGO_ALPHA_TIME 60*4
#define START_ALPHA_TIME 20
#define CHANGE_SCENE_TIME 20

//コンストラクタ
DemoScene::DemoScene()
{
	/************************************************テクスチャー*************************************************/
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TITLE_LOGO_TEXTURE, "Res/Texture/title.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::BLACK_TEXTURE, "Res/Texture/black.png");
}

//デストラクタ
DemoScene::~DemoScene()
{
}

//開始
void DemoScene::Initialize()
{
	/*マップ関係*/
	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_CENTER_ACTOR);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	std::shared_ptr<Stage> stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide,false);
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);

	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_1P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_2P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_3P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_4P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_5P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_6P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_7P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_8P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::GOD_CAMERA, stage);

	//プレイヤーチームを追加
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetPadNum(0);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_2P)->SetPadNum(1);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_3P)->SetPadNum(2);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_4P)->SetPadNum(3);
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_1P, 0, 0));
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_2P, 1, 1));
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_3P, 2, 2));
	wa.Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::PLAYER_CAMERA_4P, 3, 3));
	//敵チームを追加
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_5P)->SetPadNum(4);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_6P)->SetPadNum(5);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_7P)->SetPadNum(6);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_8P)->SetPadNum(7);
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_5P, 4, 4));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_6P, 5, 5));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_7P, 6, 6));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_8P, 7, 7));

	Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->SetCamera(vector3(0, 2.0f, -5.0f), vector3(0, 0, 0), 1.0f / 60.0f);
	/***********/
	Audio::GetInstance().SetBGMVolume(0);
	Audio::GetInstance().SetSEVolume(80);
	Audio::GetInstance().SetBGMVolume(80);
	Audio::GetInstance().PlayBGM(BGM_ID::TITLE_BGM, true);

	mIsEnd = false;
	pressStart = false;
	effectEnd = false;
	timer = 0;
	backLerp = 1;
	logoLerp = 0;
	pressLerp = 0;
}

void DemoScene::Update(float frameTime)
{
	wa.Update(frameTime);
	//Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetCamera(vector3(-10, 0.0f, -10.0f), vector3(0, 0, 0), 1.0f / 60.0f);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_2P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_3P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_4P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_5P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_6P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_7P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_8P)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);
	//Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(frameTime);

	//最初のPRESS_STARTが押されていなければ、PRESS_START文字を明滅。
	if (!effectEnd){
		backLerp = max(backLerp - (1.0f / 20.0f), 0.0f);
		if (timer >= BACK_ALPHA_TIME + BACK_BLANK_TIME){
			logoLerp = min(logoLerp + (1.0f / (60.0f*4.0f)), 1.0f);
		}
		if (timer >= BACK_ALPHA_TIME + BACK_BLANK_TIME + LOGO_ALPHA_TIME){
			pressLerp = min(pressLerp + (1.0f / 20.0f), 1.0f);
		}
		if (timer >= BACK_ALPHA_TIME + BACK_BLANK_TIME + LOGO_ALPHA_TIME + START_ALPHA_TIME){
			effectEnd = true;
			timer = 0;
		}
		timer += 60 * frameTime;

		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true)||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)){
			pressStart = true;
			effectEnd = true;
			timer = 0;
			backLerp = 0.0f;
			logoLerp = 1.0f;
			pressLerp = 1.0f;
		}
	}
	else if (effectEnd && !pressStart){
		timer += 3 * 60 * frameTime;
		timer %= 360;
		pressLerp = Math::sin(timer) + 1.0f;

		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)){
			pressStart = true;
			effectEnd = true;
			timer = 0;
			backLerp = 0.0f;
			logoLerp = 1.0f;
			pressLerp = 1.0f;
		}
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true) ||
			Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CROSS, true)){
			Initialize();
		}
	}
	else if (pressStart){
		backLerp = min(backLerp + 1.0f / CHANGE_SCENE_TIME, 1.0f);
		timer+= 60.0f * frameTime;
		if (timer > CHANGE_SCENE_TIME){
			mIsEnd = true;
		}
	}
}

//描画
void DemoScene::Draw() const
{
	wa.Draw(CAMERA_ID::GOD_CAMERA);
	//現在のスクリーンサイズに拡大。
	Vector2 screenPow = vector2(1920.0f / 1280.0f, 1080.0f / 768.0f);

	float logoAlpha = Math::lerp3(0.0f, 1.0f, logoLerp);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::TITLE_LOGO_TEXTURE, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y), screenPow, D3DXCOLOR(1, 1, 1, logoLerp), vector2(0.5f, 0.5f), 0, 0, 1.0f, 1.0f, 0);

	float startAlpha = Math::lerp3(0.0f, 1.0f, pressLerp);
	Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y + 256), vector2(0.5f, 0.5f), 0.5f, "press SPACE or Circle button", vector3(1, 1, 1), pressLerp, true);

	float backAlpha = Math::lerp3(0.0f, 1.0f, backLerp);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, backLerp), vector2(0.5f, 0.5f), 0, 0, 1.0f, 1.0f, 0);

	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 256), vector2(0.3f, 0.3f), 0.5f, "logo:"+(int)(logoLerp*100), vector3(0, 0, 0), 1.0f);
}

//終了しているか？
bool DemoScene::IsEnd() const
{
	return mIsEnd;
}

//次のシーンを返す
Scene DemoScene::Next() const
{
	return Scene::Title;
}


void DemoScene::End(){
	wa.Clear();
}