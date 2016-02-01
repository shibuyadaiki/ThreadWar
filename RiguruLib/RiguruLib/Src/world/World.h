#pragma once
#include "IWorld.h"
#include "WorldActor.h"

class World :public IWorld{
public:
	World();
	~World();

	void Initialize();
	void Update(float frameTime);
	void Draw(CAMERA_ID cID) const;
	void Clear();
	virtual void Add(ACTOR_ID id, ActorPtr actor);
	virtual void Add(ActorPtr actor);
	virtual bool IsEnd() const;
	virtual void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);

	//�q�I�u�W�F�N�g������
	virtual void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) override;

	//�v���C���[�ƃG�l�~�[�̃|�C���^��Ԃ��B
	virtual ActorPtr GetPlayer();
	virtual ActorPtr GetEnemy();
private:
	WorldActor actors;
	ActorPtr player, enemy;
};