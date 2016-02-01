#pragma once
#include "Actor.h"
#include <vector>
class Thread;
class IWorld;

//’wå‚Ì‘ƒ3’¸“_
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

	//’wå‚Ì‘ƒ‚ÌŠÔ‚É•~‚©‚ê‚é…‚ğŒvZ
	//mWebParamater.a‚ğ‹N“_‚Æ‚µ‚ÄŒvZ
	void WebThreadsCalc();

	ACTOR_ID GetRootActor();
	void SetSelectThreadWeb(){ selectThreadWeb = true; };

private:
	//¶¬‚ÌŒ³‚É‚È‚Á‚½…
	std::weak_ptr<Thread> mParent1;
	std::weak_ptr<Thread> mParent2;

	ThreadWebParameter mWebParamater;
	//’wå‚Ì‘ƒ‚ÌŠÔ‚É•~‚©‚ê‚é…‚ğŠi”[
	std::vector<Vector3> mWebThreads;
	//’wå‚Ì‘ƒ‚ÌŠÔ‚É•~‚©‚ê‚é…‚Ì•ªŠ„”
	int mDivide;

	int hitCount;
	Vector3 color;

	bool selectThreadWeb;
};