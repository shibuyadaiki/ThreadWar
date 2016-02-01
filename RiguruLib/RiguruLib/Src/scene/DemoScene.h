#pragma once

#include "IScene.h"
#include "ISceneMediator.h"
#include "../world/World.h"
#include "../Actor/Actor.h"
#include "../Math/Vector2.h"

class  DemoScene :public IScene{
public:
	//�R���X�g���N�^ 
	DemoScene();
	//�f�X�g���N�^
	~DemoScene();

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
	bool mIsEnd;
	bool effectEnd, pressStart;
	World wa;
	//���o�̃u�����N�^�C���Ɏg�p�B
	int timer;
	//���o�̃A���t�@�l�plerp�B
	float backLerp, logoLerp, pressLerp;
};