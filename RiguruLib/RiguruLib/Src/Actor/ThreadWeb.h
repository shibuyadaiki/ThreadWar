#pragma once
#include "Actor.h"
#include <vector>
class Thread;
class IWorld;

//�w偂̑�3���_
struct ThreadWebParameter
{
	Vector3 a, b, c;
};

class ThreadWeb : public Actor, public std::enable_shared_from_this<ThreadWeb>
{
public:
	ThreadWeb(IWorld& world, std::shared_ptr<Thread> parent1, std::shared_ptr<Thread> parent2);
	~ThreadWeb();
	virtual void Initialize() override;
	virtual void Update(float frameTime) override;
	virtual void Draw(CAMERA_ID cID) const override;
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	void PlusHitCount(int hitPoint);
	ThreadWebParameter GetThreadWebParameter();

	//�w偂̑��̊Ԃɕ~����鎅���v�Z
	//mWebParamater.a���N�_�Ƃ��Čv�Z
	void WebThreadsCalc();

	ACTOR_ID GetRootActor();
	void SetSelectThreadWeb(){ selectThreadWeb = true; };

private:
	//�����̌��ɂȂ�����
	std::weak_ptr<Thread> mParent1;
	std::weak_ptr<Thread> mParent2;

	ThreadWebParameter mWebParamater;
	//�w偂̑��̊Ԃɕ~����鎅���i�[
	std::vector<Vector3> mWebThreads;
	//�w偂̑��̊Ԃɕ~����鎅�̕�����
	int mDivide;

	int hitCount;
	Vector3 color;

	bool selectThreadWeb;
};