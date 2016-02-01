#include "Stage.h"
#include  "../Other/Device.h"
#include "../Math/Vector2.h"
#include "../Math/MathUtility.h"
#include "../Graphic/Graphic.h"
#include "ID.h"
#include <string>
#include "Collision.h"
#include "CrystalCenter.h"
#include "../Audio/Audio.h"

Stage::Stage(IWorld& world_, std::weak_ptr<CrystalCenter> crystalCenter_, std::weak_ptr<CrystalCenter> crystalPlayerSide_, std::weak_ptr<CrystalCenter> crystalEnemySide_, bool bgmPlay_)
:Actor(world_), crystalCenter(crystalCenter_), crystalPlayerSide(crystalPlayerSide_), crystalEnemySide(crystalEnemySide_),bgmPlay(bgmPlay_){
	parameter.isDead = false;
	parameter.id = ACTOR_ID::STAGE_ACTOR;
	parameter.cubeScale = vector3(10.0f, 3.0f, 10.0f);
	parameter.radius = 1.0f;
	yAngle = 0;
	parameter.octId = OCT_ID::STAGE_OCT;
	float size2 = 3.0f;
	parameter.matrix = RCMatrix4::matrix(
		vector3(size2, size2, size2),
		0.0f,
		yAngle,
		0.0f,
		vector3(-1, -2.0f, 0));
	startTime = 3.0f;
}

Stage::~Stage(){

}
void Stage::Initialize(){
	//���Z�����|�C���g
	addPoint = 0;
	//�`�[���̃|�C���g(0�`200�A200�ɋ߂��قǃv���C���[�`�[�����L���A0�ɋ߂��قǃG�l�~�[�`�[�����L��)
	playerPoint = 0;
	enemyPoint = 0;
	totalPoint = 0;
	teamPoint = pointMax / 2;
	//�N���X�^���̏�Ԃ����Z�b�g���邽�߂̎���
	crystalTimer = 0;
	//�Q�[���̌o�ߎ���
	gameTimer = 0;
	//�Q�[���I�����ǂ���
	gameEnd = false;
	//�v���C���[�̏������ǂ����itrue:�v���C���[�`�[�������Afalse:�G�l�~�[�`�[�������j
	//playerTeamWin = true;
	////�����������ǂ���
	//draw = false;
	gameStart = false;
}
void Stage::Update(float frameTime){
	//����
	if (gameTimer > GameOverCount)
	{
		////�|�C���g�ɂ���ď��s������
		//if (teamPoint == pointMax / 2) draw = true;
		//else if (teamPoint < pointMax / 2) playerTeamWin = false;
		//�I��
		gameEnd = true;
	}
	if (playerPoint == enemyPoint) winner = VICTORY_ID::DRAW_WIN;
	else { winner = playerPoint > enemyPoint ? VICTORY_ID::PLAYER_WIN : VICTORY_ID::ENEMY_WIN; }


	//�I����̓|�C���g�v�Z���Ȃ�
	if (gameEnd) return;

	if (startTime < 0){
		gameTimer += frameTime;
		crystalTimer += frameTime;
		if (startTime > -1.0f)startTime -= 0.6f * frameTime;
		if (!gameStart){
			if (bgmPlay)
			Audio::GetInstance().PlayBGM(BGM_ID::GAME_BGM, true);
			gameStart = true;
		}
	}
	else{
		startTime -= frameTime;
	}

	//�N���X�^���̏�Ԃ����Z�b�g
	if (crystalTimer >= 90.0f)
	{
		crystalTimer = 0;
		ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->Reset();
		ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->Reset();
		ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->Reset();
	}
}

void Stage::Draw(CAMERA_ID cID) const{
	Graphic::GetInstance().SetShader(SHADER_ID::STAGE_SHADER);
	Graphic::GetInstance().DrawMesh(MODEL_ID::STAGE_MODEL, &parameter.matrix, cID, NULL, true);

	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 400), vector2(0.20f, 0.25f), 0.5f, "point:" + std::to_string(teamPoint));
	//Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 380), vector2(0.20f, 0.25f), 0.5f, "timer:" + std::to_string(gameTimer));
	if (gameEnd)
	{
		///	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 340), vector2(0.20f, 0.25f), 0.5f, "playerWin:" + std::to_string(playerTeamWin));
		//	Graphic::GetInstance().DrawFont(FONT_ID::TEST_FONT, vector2(800, 320), vector2(0.20f, 0.25f), 0.5f, "draw:" + std::to_string(draw));
	}
}

void Stage::OnCollide(Actor& other, CollisionParameter colpara){

}

Matrix4* Stage::ReturnMat(){
	return &parameter.matrix;
}

void Stage::CrystalDamage(ACTOR_ID threadID, ACTOR_ID crystalID){
	if (crystalID == ACTOR_ID::CRYSTAL_CENTER_ACTOR){
		crystalCenter._Get()->Damage(threadID == ACTOR_ID::PLAYER_THREADBULLET_ACTOR ? true : false);
	}
	else if (crystalID == ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR){
		crystalPlayerSide._Get()->Damage(threadID == ACTOR_ID::PLAYER_THREADBULLET_ACTOR ? true : false);
	}
	else if (crystalID == ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR){
		crystalEnemySide._Get()->Damage(threadID == ACTOR_ID::PLAYER_THREADBULLET_ACTOR ? true : false);
	}
}

std::weak_ptr<CrystalCenter> Stage::ReturnCrystal(ACTOR_ID crystalID)
{
	if (crystalID == ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR){
		return crystalPlayerSide;
	}
	else if (crystalID == ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR){
		return crystalEnemySide;
	}
	else{
		return crystalCenter;
	}
}

void Stage::AddPoint(ACTOR_ID id)
{
	addPoint = 0;
	//���񂾂̂��v���C���[�Ȃ�
	if (id == ACTOR_ID::PLAYER_ACTOR)
	{
		if (ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsEnemy())
			addPoint++;
		if (ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsEnemy())
			addPoint++;
		if (ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsEnemy())
			addPoint++;
		addPoint = 1 + addPoint;
		enemyPoint += addPoint;
	}
	//���񂾂̂��G�l�~�[�Ȃ�
	else
	{
		if (ReturnCrystal(ACTOR_ID::CRYSTAL_PLAYERSIDE_ACTOR)._Get()->IsPlayer())
			addPoint++;
		if (ReturnCrystal(ACTOR_ID::CRYSTAL_CENTER_ACTOR)._Get()->IsPlayer())
			addPoint++;
		if (ReturnCrystal(ACTOR_ID::CRYSTAL_ENEMYSIDE_ACTOR)._Get()->IsPlayer())
			addPoint++;
		addPoint = 1 + addPoint;
		playerPoint += addPoint;
	}

	totalPoint = playerPoint + enemyPoint;

	////���Z
	//teamPoint += addPoint;
	////�N�����v
	//teamPoint = min(pointMax, teamPoint);
	//teamPoint = max(0, teamPoint);

	////�R�[���h����
	//if (teamPoint >= pointMax)
	//{
	//	//�I��
	//	gameEnd = true;
	//	playerTeamWin = true;
	//}
	//else if (teamPoint <= 0)
	//{
	//	//�I��
	//	gameEnd = true;
	//	playerTeamWin = false;
	//}
}

VICTORY_ID Stage::ReturnWinner(){
	return winner;
}