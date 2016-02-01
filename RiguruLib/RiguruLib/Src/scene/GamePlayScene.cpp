#include "GamePlayScene.h"
#include <sstream>
#include <iomanip>
#include "Scene.h"
#include "../actor/Player.h"
#include "../actor/Thread.h"
#include "../actor/UI.h"
#include "../Graphic/Graphic.h"
#include "../Audio/Audio.h"
#include "../Actor/CrystalCenter.h"


//コンストラクタ
GamePlayScene::GamePlayScene(VICTORY_ID& winner_) :
winner(&winner_)
{
	//mIsEnd = false;
	svolume = 80;
	bvolume = 80;

	/************************************************モデル*****************************************************/
	Graphic::GetInstance().LoadMesh(MODEL_ID::SPHERE_MODEL, "Res/Rgr/shape/ball.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::CUBE_MODEL, "Res/Rgr/shape/cube.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::TARENTULE_MODEL, "Res/Rgr/kumo/kumo.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::NEPHILA_MODEL, "Res/Rgr/nephila/nephila.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::THREAD_EFFECT_MODEL, "Res/Rgr/thread/threadEffect.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::STAGE_MODEL, "Res/Rgr/stage/map01/stageDraw.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::CRYSTAL_CENTER_MODEL, "Res/Rgr/stage/crystal1.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::CRYSTAL_ENEMYSIDE_MODEL, "Res/Rgr/stage/crystal2.rgr");
	Graphic::GetInstance().LoadMesh(MODEL_ID::CRYSTAL_PLAYERSIDE_MODEL, "Res/Rgr/stage/crystal3.rgr");

	/***********************************************オクツリー**************************************************/
	Graphic::GetInstance().LoadOctree(OCT_ID::STAGE_OCT, "Res/Rgr/Octree/stageOct.oct");

	/**********************************************アニメーション***********************************************/
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_WAIT_ANIM, "Res/RgrA/kumo/kumoWait.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_WAITLEFT_ANIM, "Res/RgrA/kumo/kumoleft.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_WALKFRONT_ANIM, "Res/RgrA/kumo/kumoFront.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_WALKBACK_ANIM, "Res/RgrA/kumo/kumoBack.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_WALKRIGHT_ANIM, "Res/RgrA/kumo/kumoRight.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_TURNLEFT_ANIM, "Res/RgrA/kumo/kumoTurn.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_STEPLEFT_ANIM, "Res/RgrA/kumo/kumoStepLeft.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_STEPFRONT_ANIM, "Res/RgrA/kumo/kumoStepFront.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_STEPBACK_ANIM, "Res/RgrA/kumo/kumoStepBack.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_STEPRIGHT_ANIM, "Res/RgrA/kumo/kumoStepRight.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_JUMP_ANIM, "Res/RgrA/kumo/kumoJump.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_ROUND_ANIM, "Res/RgrA/kumo/kumoRound.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_THREADFRONT_ANIM, "Res/RgrA/kumo/kumoThreadFront.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_THREADLEFT_ANIM, "Res/RgrA/kumo/kumoThreadLeft.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_REWIND_ANIM, "Res/RgrA/kumo/kumoRewind.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::TARENTULE_DEAD_ANIM, "Res/RgrA/kumo/kumoDead.rgra");

	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_WAIT_ANIM, "Res/RgrA/nephila/nephilaWait.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_WAITLEFT_ANIM, "Res/RgrA/nephila/nephilaleft.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_WALKFRONT_ANIM, "Res/RgrA/nephila/nephilaFront.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_WALKBACK_ANIM, "Res/RgrA/nephila/nephilaBack.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_WALKRIGHT_ANIM, "Res/RgrA/nephila/nephilaRight.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_TURNLEFT_ANIM, "Res/RgrA/nephila/nephilaTurn.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_STEPLEFT_ANIM, "Res/RgrA/nephila/nephilaStepLeft.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_STEPFRONT_ANIM, "Res/RgrA/nephila/nephilaStepFront.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_STEPBACK_ANIM, "Res/RgrA/nephila/nephilaStepBack.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_STEPRIGHT_ANIM, "Res/RgrA/nephila/nephilaStepRight.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_JUMP_ANIM, "Res/RgrA/nephila/nephilaJump.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_ROUND_ANIM, "Res/RgrA/nephila/nephilaRound.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_THREADFRONT_ANIM, "Res/RgrA/nephila/nephilaThreadFront.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_THREADLEFT_ANIM, "Res/RgrA/nephila/nephilaThreadLeft.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_REWIND_ANIM, "Res/RgrA/nephila/nephilaRewind.rgra");
	Graphic::GetInstance().LoadAnimation(ANIM_ID::NEPHILA_DEAD_ANIM, "Res/RgrA/nephila/nephilaDead.rgra");

	/***********************************************テクスチャ**************************************************/
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::FLOOR_TEXTURE, "Res/Texture/map.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::CURSOR_TEXTURE, "Res/Texture/cursor1.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::CURSOR2_TEXTURE, "Res/Texture/cursor2.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::POINT_TEXTURE, "Res/Texture/character.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::ARROW_TEXTURE, "Res/Texture/arrow.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::GAUGE_TEXTURE, "Res/Texture/gauge.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::GAUGE_CENTER_TEXTURE, "Res/Texture/gauge2.png");
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::DAMAGE_TEXTURE, "Res/Texture/damage.png");


	/**********************************************フォントテクスチャ*******************************************/
	Graphic::GetInstance().LoadFont(FONT_ID::TEST_FONT, "Res/Texture/font/font.png");

	/************************************************シェーダー*************************************************/
	Graphic::GetInstance().LoadShader(SHADER_ID::PLAYER_SHADER, "Shader/cso/fbxModelShader.cso");
	Graphic::GetInstance().LoadShader(SHADER_ID::THREAD_EFFECT_SHADER, "Shader/cso/ThreadEffectShader.cso");
	Graphic::GetInstance().LoadShader(SHADER_ID::STAGE_SHADER, "Shader/cso/StageShader.cso");
	Graphic::GetInstance().LoadShader(SHADER_ID::TEXTURE_SHADER, "Shader/cso/TextureShader.cso", false);
	Graphic::GetInstance().LoadShader(SHADER_ID::FONT_SHADER, "Shader/cso/FontShader.cso", false);
	Graphic::GetInstance().LoadShader(SHADER_ID::SPHERE_SHADER, "Shader/cso/SphereShader.cso");
	Graphic::GetInstance().LoadShader(SHADER_ID::CUBE_SHADER, "Shader/cso/CubeShader.cso");
	Graphic::GetInstance().LoadShader(SHADER_ID::LINE_SHADER, "Shader/cso/LineShader.cso");
	Graphic::GetInstance().LoadShader(SHADER_ID::CRYSTAL_SHADER, "Shader/cso/CrystalShader.cso");

	/**************************************************音楽*****************************************************/
	Audio::GetInstance().LoadSE(SE_ID::BOOM_SE, _T("Res/Sound/thread_se.wav"), 10);
	Audio::GetInstance().LoadSE(SE_ID::ENTER_SE, _T("Res/Sound/enter_se.wav"), 10);
	Audio::GetInstance().LoadSE(SE_ID::BACK_SE, _T("Res/Sound/back_se.wav"), 10);
	Audio::GetInstance().LoadSE(SE_ID::SWITCH_SE, _T("Res/Sound/switch_se.wav"), 10);
	Audio::GetInstance().LoadBGM(BGM_ID::TITLE_BGM, _T("Res/Sound/title_bgm.wav"));
	Audio::GetInstance().LoadBGM(BGM_ID::GAME_BGM, _T("Res/Sound/game_bgm.wav"));
}

//デストラクタ
GamePlayScene::~GamePlayScene()
{
}

//開始
void GamePlayScene::Initialize()
{
	mIsEnd = false;
	fadeIn = true; 
	fadeOut = false;
	*winner = VICTORY_ID::PLAYER_WIN;
	fadeTime = 1;

	wa.Initialize();

	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa,ACTOR_ID::CRYSTAL_CENTER_ACTOR);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide,true);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_1P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_2P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_3P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_4P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_5P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_6P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_7P, stage);
	Device::GetInstance().CameraInit(CAMERA_ID::ENEMY_CAMERA_8P, stage);
	//Device::GetInstance().CameraInit(CAMERA_ID::GOD_CAMERA, stage);

	wa.Add(ACTOR_ID::UI_ACTOR, std::make_shared<UI>(wa,stage));
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);

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
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_5P, 1, 4));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_6P, 5, 5));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_7P, 6, 6));
	wa.Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Player>(wa, stage, CAMERA_ID::ENEMY_CAMERA_8P, 7, 7));

	//Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA)->SetPadNum(0);
	//Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA)->SetPadNum(1);

	Audio::GetInstance().SetSEVolume(svolume);
	Audio::GetInstance().SetBGMVolume(bvolume);
}

void GamePlayScene::Update(float frameTime)
{
	/*if (fadeIn){
		fadeTime = max((fadeTime + 1.0f / 60.0f) * 60.0f * frameTime, 0.0f);
		if (fadeTime == 0.0f)
			fadeIn = false;
		return;
	}
	if (fadeOut){
		fadeTime = min((fadeTime - 1.0f / 60.0f) * 60.0f * frameTime, 1.0f);
		if (fadeTime == 1.0f)
			mIsEnd = true;
		return;
	}*/
	if (stage.get()->ReturnGameTime() <= 0){
		mIsEnd = true;
		*winner = stage.get()->ReturnWinner();
	}
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_1, true))mIsEnd = true;
	//カメラの設定
	wa.Update(frameTime);
	
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_1, true)){
	//	svolume = svolume - 10 < 0 ? 0 : svolume - 10;
	//	Audio::GetInstance().SetSEVolume(svolume);
	//}
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_2, true)){
	//	svolume = svolume + 10 > 100 ? 100 : svolume + 10;
	//	Audio::GetInstance().SetSEVolume(svolume);
	//}
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_3, true)){
	//	bvolume = bvolume - 10 < 0 ? 0 : bvolume - 10;
	//	Audio::GetInstance().SetBGMVolume(bvolume);
	//}
	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_4, true)){
	//	bvolume = bvolume + 10 > 100 ? 100 : bvolume + 10;
	//	Audio::GetInstance().SetBGMVolume(bvolume);
	//}
	if (Device::GetInstance().GetInput()->MouseButtonDown(INPUTMOUSEBUTTON::LEFT_BUTTON, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0,GAMEPADKEY::BUTTON_R1,true))
	Audio::GetInstance().PlaySE(SE_ID::BOOM_SE);

	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_2P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_3P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_4P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_5P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_6P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_7P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA_8P)->SetCamera(vector3(0, 0.0f, 0.0f), vector3(0, 0, 0), frameTime);
	//Device::GetInstance().GetCamera(CAMERA_ID::GOD_CAMERA)->GotCamera(frameTime);
	//Device::GetInstance().GetCamera(CAMERA_ID::ENEMY_CAMERA)->SetCamera(vector3(0, 0.0f, -3.0f), vector3(0, 0, 0), frameTime);

	//if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_C, true))
	//{
	//	drawNum++;
	//	if (drawNum >= 8 || drawNum <= -1) drawNum = 0;
	//	if (drawNum == 0)drawCamera = CAMERA_ID::PLAYER_CAMERA_1P;
	//	if (drawNum == 1)drawCamera = CAMERA_ID::PLAYER_CAMERA_2P;
	//	if (drawNum == 2)drawCamera = CAMERA_ID::PLAYER_CAMERA_3P;
	//	if (drawNum == 3)drawCamera = CAMERA_ID::PLAYER_CAMERA_4P;
	//	if (drawNum == 4)drawCamera = CAMERA_ID::ENEMY_CAMERA_5P;
	//	if (drawNum == 5)drawCamera = CAMERA_ID::ENEMY_CAMERA_6P;
	//	if (drawNum == 6)drawCamera = CAMERA_ID::ENEMY_CAMERA_7P;
	//	if (drawNum == 7)drawCamera = CAMERA_ID::ENEMY_CAMERA_8P;
	//	//if (drawNum == 8)drawCamera = CAMERA_ID::GOD_CAMERA;
	//}
}

//描画
void GamePlayScene::Draw() const
{
	//Device::GetInstance().Getd3d11User()->ChangeViewport(0, 1, 0, 0.5f);
	//Device::GetInstance().Getd3d11User()->ChangeViewport(0, 0, 0, 0);
	//wa.Draw(CAMERA_ID::PLAYER_CAMERA_4P);
	//wa.Draw(CAMERA_ID::PLAYER_CAMERA_3P);
	//wa.Draw(CAMERA_ID::PLAYER_CAMERA_2P);

	//Device::GetInstance().Getd3d11User()->ChangeViewport(0, 1, 0.5f, 0.5f);
	//wa.Draw(CAMERA_ID::ENEMY_CAMERA);
	Device::GetInstance().Getd3d11User()->ChangeViewport(0, 1, 0, 1);
	wa.Draw(drawCamera);
	//Graphic::GetInstance().DrawSphere(RCVector3::lerp(vector3(-2.4f, -1.3f, -1.5f), vector3(0.5f, 5.5f, 1.5f), 0.5), 4.0f, drawCamera);//中央
	//Graphic::GetInstance().DrawSphere(RCVector3::lerp(vector3(-2.4f, -1.3f, 62.0f), vector3(0.5f, 5.5f, 65.3f), 0.5), 4.0f, drawCamera);//氷
	//Graphic::GetInstance().DrawSphere(RCVector3::lerp(vector3(-2.4f, -1.3f, -67.0f), vector3(0.5f, 5.5f, -64.0f), 0.5), 4.0f, drawCamera);//洞窟
	//Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, 1), vector2(0, 0));

	//wa.Draw(CAMERA_ID::PLAYER_CAMERA_1P);
}

//終了しているか？
bool GamePlayScene::IsEnd() const
{
	return mIsEnd;
}

//次のシーンを返す
Scene GamePlayScene::Next() const
{
	Audio::GetInstance().StopBGM(BGM_ID::GAME_BGM);
	return Scene::Ending;
}

void GamePlayScene::End(){
	wa.Clear();
}
