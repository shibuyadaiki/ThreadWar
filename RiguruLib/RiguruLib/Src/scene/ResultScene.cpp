#include "ResultScene.h"
#include "Scene.h"
#include "../Other/Device.h"
#include "../Graphic/Graphic.h"
#include "../Math/MathUtility.h"

//�R���X�g���N�^
ResultScene::ResultScene(VICTORY_ID& winner_):
winner(&winner_)
{
}

//�f�X�g���N�^
ResultScene::~ResultScene()
{
}

//�J�n
void ResultScene::Initialize()
{
	mIsEnd = false;
	timer = 0;
}

void ResultScene::Update(float frameTime)
{
	timer = min((timer + 1.0f / 60.0f) * 60 * frameTime, 1.0f);
	
	if ((Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_Z, true) ||
		Device::GetInstance().GetInput()->KeyDown(INPUTKEY::KEY_SPACE, true) ||
		Device::GetInstance().GetInput()->GamePadButtonDown(0, GAMEPADKEY::BUTTON_CURCLE, true)) &&
		timer == 1.0f){
		mIsEnd = true;
	}
}

//�`��
void ResultScene::Draw() const
{
	//���݂̃X�N���[���T�C�Y�Ɋg��B
	Vector2 screenPow = vector2(1920.0f / 1280.0f, 1080.0f / 720.0f);

	Graphic::GetInstance().DrawTexture(TEXTURE_ID::DAMAGE_TEXTURE, vector2(0, 0), screenPow, D3DXCOLOR(1, 1, 1, 1), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
	if (*winner == VICTORY_ID::PLAYER_WIN)
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "PLAYER TEAM WINS", vector3(1, 0, 0), 1.0f, true);
	else if (*winner == VICTORY_ID::ENEMY_WIN)
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "ENEMY TEAM WINS", vector3(0, 0, 1), 1.0f, true);
	else if (*winner == VICTORY_ID::DRAW_WIN)
		Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(1920 / 2.0f, 1080 / 2.0f), vector2(0.6f, 0.6f), 0.5f, "DRAW", vector3(0, 1, 0), 1.0f, true);

	float backAlpha = Math::lerp3(1.0f, 0.0f, timer);
	Graphic::GetInstance().DrawTexture(TEXTURE_ID::BLACK_TEXTURE, vector2(0, 0), vector2(1920, 1080), D3DXCOLOR(1, 1, 1, backAlpha), vector2(0.0f, 0.0f), 0, 0, 1.0f, 1.0f, 0);
}

//�I�����Ă��邩�H
bool ResultScene::IsEnd() const
{
	return mIsEnd;
}

//���̃V�[����Ԃ�
Scene ResultScene::Next() const
{
	return Scene::Demo;
}


void ResultScene::End(){
}