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
	//糸のパラメータを取得
	ThreadParameter GetThreadParameter();
	//巻取り開始
	void StartRewind();
	//巻き取り
	void ThreadRewind(float frameTime);
	//巻取り中かどうか
	bool IsRewind();
	//蜘蛛の巣を生成
	void AddThreadWebInit();
	void AddThreadWeb();
	//巻取り中の移動方向ベクトルを取得（単位ベクトル）
	Vector3 GetRewindGoingVector();

	//糸を発射したアクターのACTOR_IDを返す。
	ACTOR_ID GetRootActor();

	//糸発射
	void ThreadShot(float frameTime);
	//糸発射中か。発射が終了するとRewind可能。
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
	ACTOR_ID rootActor;//誰が発射した糸か。
	std::weak_ptr<Stage> stage;
	std::list<std::weak_ptr<Thread>> threads;
	Matrix4 mouseMat;
	float angle = 0;
	float angleZ = 0;
	float bvolume = 50;
	float svolume = 50;
	float animCount = 0;

	//糸が持つパラメータ
	ThreadParameter threadParam;
	//巻き取り中か？、ショット中か。/*追加*/
	bool isRewind, isShot;
	//巻き取る際、進行方向が糸のendPositionか？
	bool isGoingEnd;
	//移動方向ベクトル
	Vector3 goingVec;
	//巻き取り速度
	float rewindSpeed;

	//糸の発射スピード
	float shotSpeed;
	//最終的な糸の長さ(始点と終点の距離)。
	float endDis;

	bool dangle;

	bool selectThread;
	bool threadHit;
	Vector3 color;
};