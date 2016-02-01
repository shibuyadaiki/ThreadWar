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
	//�R���X�g���N�^ 
	TitleScene();
	//�f�X�g���N�^
	~TitleScene();

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

	//�^�C�g���Z���N�g�B
	void TitleSelect();
	//�`�[���Z���N�g�B
	void TeamSelect();

	/*�^�C�g���Z���N�g��2�̑I�����ړ��B*/
	void DMove();
	void UMove();
	/*******************/

private:
	bool mIsEnd;
	//2�̑I�����̂�����1���I�����ꂽ���B
	bool isSelect;
	World wa;
	//���o�̓_�ł�u�����N�^�C���ȂǂɎg�p�B
	int timer;
	//���o��lerp�p�B
	float lerpTime;
	//2�̑I�����őI�����ꂽ���̂��i�[�B
	TITLE_SELECT selects;
	//2�̃`�[���őI�����ꂽ���̂��i�[�B
	TEAM_ID teamID;

	//�I�����̃p�����[�^�B
	float ts_scale[TITLE_SELECT_NUM];
	float ts_nextScale[TITLE_SELECT_NUM], ts_prevScale[TITLE_SELECT_NUM];
	float ts_alpha[TITLE_SELECT_NUM];
	float ts_nextAlpha[TITLE_SELECT_NUM];

	Vector3 pos;
};