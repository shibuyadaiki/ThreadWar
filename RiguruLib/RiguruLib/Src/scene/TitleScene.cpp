#include "TitleScene.h"
#include <sstream>
#include <iomanip>
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

#define TITLE_SELECT_NUM 2
#define MOVE_AMOUNT 256
#define SCREEN_CENTER_X 1920/2
#define SCREEN_CENTER_Y 1080/2
#define DEFAULT_X SCREEN_CENTER_X - 256

//�R���X�g���N�^
TitleScene::TitleScene()
{
	/************************************************�e�N�X�`���[*************************************************/
	Graphic::GetInstance().LoadTexture(TEXTURE_ID::TITLE_BACK_TEXTURE, "Res/Texture/test_back.png");

}

//�f�X�g���N�^
TitleScene::~TitleScene()
{
}

//�J�n
void TitleScene::Initialize()
{
	/*�}�b�v�֌W*/
	std::shared_ptr<CrystalCenter> crystalCenter = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_CENTER_ACTOR);
	std::shared_ptr<CrystalCenter> crystalPlayerSide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR);
	std::shared_ptr<CrystalCenter> crystalEnemySide = std::make_shared<CrystalCenter>(wa, ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR);
	wa.Add(ACTOR_ID::CRYSTAL_CENTER_ACTOR, crystalCenter);
	wa.Add(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR, crystalPlayerSide);
	wa.Add(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR, crystalEnemySide);
	std::shared_ptr<Stage> stage = std::make_shared<Stage>(wa, crystalCenter, crystalPlayerSide, crystalEnemySide,false);
	Device::GetInstance().CameraInit(CAMERA_ID::PLAYER_CAMERA_1P, stage);
	wa.Add(ACTOR_ID::STAGE_ACTOR, stage);
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetCamera(vector3(0,0,0.3f), vector3(0, 0, 0), 1.0f / 60.0f);
	/***********/

	Audio::GetInstance().SetSEVolume(80);
	Audio::GetInstance().SetBGMVolume(80);

	mIsEnd = false;
	isSelect = false;
	timer = 0;
	lerpTime = 1;
	selects = SELECT_GAMESTART;
	teamID = TEAM_ID::FIRST_TEAM;

	//3�̑I�����̃p�����[�^���������B
	for (int i = 0; i <= 1; i++){
		ts_scale[i] = 0.6f;
		ts_alpha[i] = 0.5f;
		if (selects == (TITLE_SELECT)i){
			ts_scale[i] = 1.0f;
			ts_alpha[i] = 1.0f;
		}
		ts_nextScale[i] = ts_scale[i];
		ts_prevScale[i] = ts_scale[i];
		ts_nextAlpha[i] = ts_alpha[i];
	}
	pos = vector3(0, 0, 0);
}

void TitleScene::Update(float frameTime)
{
	wa.Update(frameTime);

	//PRESS_START�������ꂽ��A"��l�v���C"�A"��ʕ���"�A"�I�v�V����"��3�̑I�����B
	if (!isSelect){
		TitleSelect();
		timer += 5 * 60 * frameTime;
		timer %= 360;
	}
	//3�̑I�����̈���I�����ꂽ��A�`�[���I���B
	else if (isSelect)
		mIsEnd = true;
		//TeamSelect();

	/*if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_W))
		pos.z += 0.1f;
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_S))
		pos.z -= 0.1f;
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_D))
		pos.x += 0.1f;
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_A))
		pos.x -= 0.1f;
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_E))
		pos.y += 0.1f;
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Q))
		pos.y -= 0.1f;
	Device::GetInstance().GetCamera(CAMERA_ID::PLAYER_CAMERA_1P)->SetCamera(pos, vector3(0, 0, 0), 1.0f / 60.0f);
	Vector3 p = pos;*/
}

//�`��
void TitleScene::Draw() const
{
	wa.Draw(CAMERA_ID::PLAYER_CAMERA_1P);
	if (!isSelect){
		//����
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y - MOVE_AMOUNT / 2), vector2(1.0f, 1.0f)*ts_scale[0], 0.5f, "START GAME", vector3(1, 1, 1), ts_alpha[0], true);
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(SCREEN_CENTER_X, SCREEN_CENTER_Y + MOVE_AMOUNT / 2), vector2(1.0f, 1.0f)*ts_scale[1], 0.5f, "OPTIONS", vector3(1, 1, 1), ts_alpha[1], true);
		//�f�o�b�O�p�B
		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 500), vector2(0.20f, 0.25f), 0.5f, "gamestartts_scale:" + std::to_string(ts_scale[0]));
		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 475), vector2(0.20f, 0.25f), 0.5f, "select:" + std::to_string(selects));
	}
	else{
		Vector2 pos = vector2(128, 1080 - 64);
		//�w�i��
		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, pos, vector2(0.3f, 0.3f), 0.5f, teamID == TEAM_ID::FIRST_TEAM ? "TEAM_ONE" : "TEAM_TWO", vector3(0, 0, 0), 1.0f);
		//����
		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, pos, vector2(0.29f, 0.29f), 0.5f, teamID == TEAM_ID::FIRST_TEAM ? "TEAM_ONE" : "TEAM_TWO", vector3(1, 1, 1), 1.0f);

		pos = vector2(1920 - (pos.x * 2), pos.y);
		//�w�i��
		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, pos, vector2(0.3f, 0.3f), 0.5f, teamID == TEAM_ID::SECOND_TEAM ? "TEAM_ONE" : "TEAM_TWO", vector3(0, 0, 0), 1.0f);
		//����
		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, pos, vector2(0.29f, 0.29f), 0.5f, teamID == TEAM_ID::SECOND_TEAM ? "TEAM_ONE" : "TEAM_TWO", vector3(1, 1, 1), 1.0f);

		//�f�o�b�O�p
		Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(0, 475), vector2(0.20f, 0.25f), 0.5f, "team:" + std::to_string(teamID));
	}
}

//�I�����Ă��邩�H
bool TitleScene::IsEnd() const
{
	return mIsEnd;
}

//���̃V�[����Ԃ�
Scene TitleScene::Next() const
{
	Audio::GetInstance().StopBGM(BGM_ID::TITLE_BGM);
	if (selects == TITLE_SELECT::SELECT_RETURN)
		return Scene::Demo;
	else
		return Scene::GamePlay;
	
	/*switch (selects)
	{
	case SELECT_GAMESTART_SINGLE:
	return Scene::GamePlay;
	break;
	case SELECT_GAMESTART_TWOPLAYER:
	return Scene::GamePlay;
	break;
	case SELECT_OPTION:
	return Scene::OPTION;
	break;
	default:
	return Scene::GamePlay;
	break;
	}*/
}

void TitleScene::TitleSelect(){
	if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_UP) ||
		Device::GetInstance().GetInput()->LeftStick(0).z >= 0.5f) &&
		lerpTime == 1.0f){
		selects = (TITLE_SELECT)(selects - 1);
		selects = (TITLE_SELECT)(selects < 0 ? 1 : selects);
		UMove();
	}
	if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_DOWN) ||
		Device::GetInstance().GetInput()->LeftStick(0).z <= -0.5f) &&
		lerpTime == 1.0f){
		selects = (TITLE_SELECT)(selects + 1);
		selects = (TITLE_SELECT)(selects % TITLE_SELECT_NUM);
		DMove();
	}
	if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)){
		isSelect = true;
	}
	else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CROSS, true)){
		mIsEnd = true;
		selects = TITLE_SELECT::SELECT_RETURN;
	}

	for (int i = 0; i <= TITLE_SELECT_NUM - 1; i++){
		ts_scale[i] = Math::lerp3(ts_prevScale[i], ts_nextScale[i], lerpTime);
		ts_alpha[i] = Math::lerp3(ts_alpha[i], ts_nextAlpha[i], lerpTime);
	}

	lerpTime = min(lerpTime + 1.0f / 15.0f, 1.0f);
}

void TitleScene::TeamSelect(){
	switch (selects)
	{
	case SELECT_OPTION:
		mIsEnd = true;
		break;
	default:
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_RIGHT)){
			teamID = TEAM_ID::FIRST_TEAM;
		}
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_LEFT)){
			teamID = TEAM_ID::SECOND_TEAM;
		}
		if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
			Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true)){
			mIsEnd = true;
		}
		else if (Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_X, true))
			isSelect = false;
		break;
	}
}

void TitleScene::DMove(){
	lerpTime = 0;

	for (int i = 0; i <= TITLE_SELECT_NUM - 1; i++){
		ts_prevScale[i] = ts_scale[i];

		ts_nextScale[i] = 0.6f;
		ts_nextAlpha[i] = 0.5f;
		if (selects == (TITLE_SELECT)i){
			ts_nextScale[i] = 1.0f;
			ts_nextAlpha[i] = 1.0f;
		}
	}
}

void TitleScene::UMove(){
	lerpTime = 0;

	for (int i = 0; i <= TITLE_SELECT_NUM - 1; i++){
		ts_prevScale[i] = ts_scale[i];

		ts_nextScale[i] = 0.6f;
		ts_nextAlpha[i] = 0.5f;
		if (selects == (TITLE_SELECT)i){
			ts_nextScale[i] = 1.0f;
			ts_nextAlpha[i] = 1.0f;
		}
	}
}

void TitleScene::End(){
	wa.Clear();
}