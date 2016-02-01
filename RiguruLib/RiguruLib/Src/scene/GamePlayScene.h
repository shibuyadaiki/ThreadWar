#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Math/Matrix4.h"
#include "../Actor/ID.h"
#include "../Actor/Stage.h"

class  GamePlayScene :public IScene{
public:
	//�R���X�g���N�^ 
	GamePlayScene(VICTORY_ID& winner_);
	//�f�X�g���N�^
	~GamePlayScene();

	//�X�V�O������
	virtual void Initialize() override;
	//�X�V����
	virtual void Update(float frameTime)override;
	//�`�揈��
	virtual void Draw() const override;
	//�I�����Ă��邩�H
	virtual bool IsEnd() const override;
	//���̃V�[����Ԃ�
	virtual Scene Next() const override;
	//�I��������
	virtual void End()override;

private:
	std::shared_ptr<Stage> stage;
	bool mIsEnd;
	World wa;
	int bvolume;
	int svolume;
	Matrix4 playerMat, enemyMat;

	//�P�`�[���̐l��
	const float teamMemberCount = 4;
	CAMERA_ID drawCamera = CAMERA_ID::PLAYER_CAMERA_1P;
	float drawNum = 0;
	VICTORY_ID* winner;
	bool fadeIn, fadeOut;
	float fadeTime;
};