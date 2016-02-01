#pragma once

#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include <map>
#include "../Math/Matrix4.h"
#include "../Math/MatrixUtility.h"
#include "../Graphic/Rgr/MeshUser.h"
#include "Actor.h"
#include <list>
#include <memory>
#include "ThreadParamter.h"

class Stage;
class Player;
class ThreadWeb;


class Thread :public Actor, public std::enable_shared_from_this<Thread>{
public:
	Thread(IWorld& world_, std::weak_ptr<Player> player_, std::weak_ptr<Stage> stage_, CAMERA_ID cID_, bool startPosGroundTouch_);
	~Thread();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other,CollisionParameter colpara) override;
	//���̃p�����[�^���擾
	ThreadParameter GetThreadParameter();
	//�����J�n
	void StartRewind();
	//�������
	void ThreadRewind(float frameTime);
	//����蒆���ǂ���
	bool IsRewind();
	//�w偂̑��𐶐�
	void AddThreadWebInit();
	void AddThreadWeb();
	//����蒆�̈ړ������x�N�g�����擾�i�P�ʃx�N�g���j
	Vector3 GetRewindGoingVector();

	//���𔭎˂����A�N�^�[��ACTOR_ID��Ԃ��B
	ACTOR_ID GetRootActor();

	//������
	void ThreadShot(float frameTime);
	//�����˒����B���˂��I�������Rewind�\�B
	bool IsShot();

	void SetDangleFlag(bool dangle_);

	void SetIsDead(bool isDead_);

	void SetStartPos(Vector3 endPos_);
	void SetEndPos(Vector3 endPos_);

	void SetSelectThread(){ selectThread = true; };
	bool ReturnDangleFlag();
	bool ThreadHit(){ return threadHit; }

	void SetIsRewind(bool flag){ isRewind = flag; }
private:
	CAMERA_ID cID;
private:
	std::weak_ptr<Player> player;
	ACTOR_ID rootActor;//�N�����˂��������B
	std::weak_ptr<Stage> stage;
	std::list<std::weak_ptr<Thread>> threads;
	Matrix4 mouseMat;
	float angle = 0;
	float angleZ = 0;
	float bvolume = 50;
	float svolume = 50;
	float animCount = 0;

	//�������p�����[�^
	ThreadParameter threadParam;
	//������蒆���H�A�V���b�g�����B/*�ǉ�*/
	bool isRewind, isShot;
	//�������ہA�i�s����������endPosition���H
	bool isGoingEnd;
	//�ړ������x�N�g��
	Vector3 goingVec;
	//������葬�x
	float rewindSpeed;

	//���̔��˃X�s�[�h
	float shotSpeed;
	//�ŏI�I�Ȏ��̒���(�n�_�ƏI�_�̋���)�B
	float endDis;

	bool dangle;

	bool selectThread;
	bool threadHit;
	Vector3 color;
};