#pragma once
#include "ID.h"
#include "../math/Matrix4.h"
#include "../math/Vector3.h"
#include "../math/Vector3Utility.h"
#include "../math/MatrixUtility.h"
#include "../Graphic/Rgr/OctreeUser.h"
#include "../Other/DX11User.h"
#include <functional>
#include "../world/IWorld.h"
//�����蔻��̃p�����[�^
struct CollisionParameter;
//�A�N�^�[�̃p�����[�^
struct ActorParameter
{
	ACTOR_ID id;
	Matrix4 matrix;
	bool isDead = false;
	Vector3 cubeScale;
	float radius;
	OCT_ID octId = OCT_ID::NULL_OCT;
	Vector3 moveVec;
	Vector3 inertiaVec;
	Vector3 lastNor;

};
enum TEAM_ID{
	FIRST_TEAM,
	SECOND_TEAM,
};
class Actor{
public:
	Actor(IWorld& world_);
	~Actor();
	virtual void Initialize() = 0;
	virtual void Update(float frameTime) = 0;
	virtual void Draw(CAMERA_ID cID) const = 0;
	void Collide(COL_ID id, Actor& other);
	virtual bool IsDead() const{ return parameter.isDead; }
	void SetIsDead(bool flag){ parameter.isDead = flag; }
	ActorParameter GetParameter();
protected:
	virtual void OnCollide(Actor& other,CollisionParameter colpara) = 0;
	ActorParameter parameter;
	IWorld& world;

private:
	std::map<COL_ID, std::function<CollisionParameter(const Actor&)>> colFunc;
	//���Ƌ��̂����蔻��
	CollisionParameter SphereSphere(const Actor& other)const;
	//���Ƌ��̂����蔻��
	CollisionParameter ThreadPlayer(const Actor& other)const;
	//���C�ƃ��f���̂����蔻��
	CollisionParameter RayModel(const Actor& other)const;
	//���C�ƃ��f���̂����蔻��(�w偔ŃX�e�b�v)
	CollisionParameter RayModelStep(const Actor& other)const;
	//���C�ƃ��f���̂����蔻��(�w偔�)
	CollisionParameter RayModelNatural(const Actor& other)const;
	CollisionParameter RayModelNaturalTest(Vector3 pos_)const;
	//���ƃ��f���̂����蔻��
	CollisionParameter SphereModel(const Actor& other)const;
	//���ƃ��f���̂����蔻��(�w偔�)
	CollisionParameter SphereModelNatural(const Actor& other)const;
	//���ƃ��f���̂����蔻��(���̏�)
	CollisionParameter SphereModelThreadWeb(const Actor& other)const;
	//�n�a�a�Ƃn�a�a�̂����蔻��
	CollisionParameter OBBOBB(const Actor& other)const;
	//�v���C���[�Ǝ��̂����蔻��
	CollisionParameter SphereLine(const Actor& other)const;
	//�v���C���[�Ǝ��̂����蔻��
	CollisionParameter LineSphere(const Actor& other)const;
	//�v���C���[�Ǝ��̖��[�̂����蔻��
	CollisionParameter SphereLineEnd(const Actor& other)const;
	//�v���C���[�Ǝ��̂����蔻��i�W�����v���j
	CollisionParameter SphereLineJump(const Actor& other)const;
	//�v���C���[�ƒw偂̑��̂����蔻��
	CollisionParameter TriangleLine(const Actor& other)const;
	//�v���C���[�ƒw偂̑��̂����蔻��
	CollisionParameter TriangleSphere(const Actor& other)const;
	//���ƒw偂̑��̂����蔻��
	CollisionParameter TriangleLine_T(const Actor& other)const;
	//�v���C���[�ƒw偂̑��̂����蔻��i�W�����v���j
	CollisionParameter TriangleLineJump(const Actor& other)const;
	//���ƃ��f���̂����蔻��(�v���C���[�p�A�ړ������̐�ɔ��苅��u��)
	CollisionParameter SphereModelMove(const Actor& other)const;
	////���C�ƃ��f���̂����蔻��(�v���C���[�p�A���̏�ɂ���Ƃ�)
	//CollisionParameter RayModelOnThread(const Actor& other)const;

	CollisionParameter CapsuleModel(const Actor& other)const;
	int cubeCount;
};