#include "WorldActor.h"
#include<algorithm>
#include "../actor/ID.h"

WorldActor::WorldActor(){
	for (int i = ACTOR_ID::BEGIN_ACTOR; i <= ACTOR_ID::END_ACTOR; ++i)
		managers.emplace(ACTOR_ID(i), std::make_shared<ActorManager>());
}
WorldActor::~WorldActor(){

}
void WorldActor::Update(float frameTime){
	//�S�L�����A�b�v�f�[�g
	std::for_each(managers.begin(), managers.end(),
		[&](ActorManagerPair pair){pair.second->Update(frameTime); });
	
	//�����蔻��
	for (auto& cols : colselect){
		for (auto& sec : cols.second){
			managers[sec.otherID]->Collide(sec.colID, *cols.first);
		}
	}

	colselect.clear();


	//managers[ACTOR_ID::PLAYER_ACTOR]->Collide(COL_ID::SPHERE_SPHERE_COLL, *managers[ACTOR_ID::ENEMY_ACTOR]);
	//managers[ACTOR_ID::ENEMY_ACTOR]->Collide(COL_ID::SPHERE_SPHERE_COLL, *managers[ACTOR_ID::PLAYER_ACTOR]);
	//�v���C���[�ƓG

	//���̖{���𐧌�����
	DeleteOldThread(40);

	//����ł���̂�����
	std::for_each(managers.begin(), managers.end(),
		[](ActorManagerPair pair){pair.second->Remove(); });
}
void WorldActor::Draw(CAMERA_ID cID) const{
	//�S�L�����`��
	std::for_each(managers.begin(), managers.end(),
		[&](ActorManagerPair pair){pair.second->Draw(cID); });
}
void WorldActor::Add(ACTOR_ID id, ActorPtr actor){
	managers[id]->Add(actor);
}
void WorldActor::Clear(){
	//�S�L�����N���A
	std::for_each(managers.begin(), managers.end(),
		[](ActorManagerPair pair){pair.second->Clear(); });
}

void WorldActor::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID){
	CollideSelect c = { otherID, colID };
	colselect[thisActor].push_back(c);
}

//�q������
void WorldActor::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func)
{
	managers[id]->EachActor(func);
}

void WorldActor::DeleteOldThread(int limit)
{
	managers[ACTOR_ID::PLAYER_THREAD_ACTOR]->DeleteOldThread(limit);
	managers[ACTOR_ID::ENEMY_THREAD_ACTOR]->DeleteOldThread(limit);
}