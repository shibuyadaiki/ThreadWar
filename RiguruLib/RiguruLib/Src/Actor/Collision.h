#pragma once
#include "ID.h"
#include "../math/Matrix4.h"
#include "../math/Vector3.h"
#include "../math/Vector3Utility.h"
#include "../math/MatrixUtility.h"
#include "../Graphic/Rgr/OctreeUser.h"
#include "../Other/DX11User.h"
#include <memory>

//�����蔻��̃p�����[�^
struct CollisionParameter{
	COL_ID id;
	bool colFlag;		//���������ǂ����̃t���O
	Vector3 colPos;		//�������|�W�V����
	Vector3 colNormal;	//�������ʂ̖@��
	bool enemyFlag = false;		//�G����̍U���i㩁j���H
};

//�n�a�a
struct OBB{
	D3DXVECTOR3 m_Pos;							// �ʒu
	std::vector<D3DXVECTOR3> m_NormaDirect;		// �����x�N�g��
	std::vector<FLOAT> m_fLength;				// �e�������̒���
};

//�J�v�Z��
struct Capsule{
	Vector3 startPos;	//�J�v�Z���̃X�^�[�g�|�W�V����
	Vector3 endPos;		//�J�v�Z���̃G���h�|�W�V����
	float radius;		//�J�v�Z���̔��a
};

//���f���̎O�p�`���ꃂ�f�����S�����ꂽ����
typedef std::map<int, std::map<int, std::vector<ModelTriangle>>> ModelTriangleVec;
//���f���̎O�p�`���ꃂ�f�����S������镨�����f�����Ƃɂ܂Ƃ߂�����
typedef std::map<int, std::vector<ModelTriangle>> ModelTriangleVecNonMap;

//���Ƃ`�`�a�a�̂����蔻��
bool ColSphereBox(CubeParameter& aabb, Vector3& spherePos, Matrix4 mat, float& radius);
//���C�Ƃ`�`�a�a�̂����蔻��
bool ColRayBox(Vector3* pos, Vector3* dir_w, CubeParameter* aabb, Matrix4* mat);
//�J�v�Z���Ƃ`�`�a�a�̂����蔻��
bool ColCapsuleBox(Vector3* pos_, Vector3* dir_w_, float radius, CubeParameter* aabb, Matrix4* mat_);

//���f�����Ƃɂ܂Ƃ߂��O�p�`�ƃ��C������Ƃ����蔻���Ԃ����́B(ContextPrecision����ԋ߂��𔻒肷�邩�ǂ���)
CollisionParameter ModelRay(ModelTriangleVecNonMap* modelTri, Matrix4 mat, Vector3 pos, Vector3 down, bool ContextPrecision = false);
//�O�p�`���ƃ��C������Ƃ����蔻���Ԃ����́B
CollisionParameter ModelRay(ModelTriangle* modelTri, Matrix4 modelMat, Vector3 pos, Vector3 down);
//�I�N�c���[�h�c�Ƃ��̃}�g���b�N�X�ƃ��C������Ƃ����蔻���Ԃ����́B
CollisionParameter ModelRay(const Matrix4& othermat_, OCT_ID id, Vector3& startPos, Vector3& endPos);
//���f�����Ƃɂ܂Ƃ߂��O�p�`�Ƌ�������Ƃ����蔻���Ԃ����́B(ContextPrecision����ԋ߂��𔻒肷�邩�ǂ���)
CollisionParameter ModelSphere(ModelTriangleVecNonMap* modelTri, Matrix4 mat, Vector3 spherePos, float radius, bool ContextPrecision = false);
//�O�p�`�Ƌ�������Ƃ����蔻���Ԃ����́B(ContextPrecision����ԋ߂��𔻒肷�邩�ǂ���)
CollisionParameter ModelSphere(ModelTriangle* modelTri, Matrix4 mat, Vector3 spherePos, float radius, bool ContextPrecision = false);
//�I�N�c���[�h�c�Ƃ��̃}�g���b�N�X�Ƌ�������Ƃ����蔻���Ԃ����́B
CollisionParameter ModelSphere(const Matrix4& othermat_, OCT_ID id, Vector3& spherePos, float radius);
//�I�N�c���[�h�c�Ƃ��̃}�g���b�N�X�ƃJ�v�Z��������Ƃ����蔻���Ԃ����́B
CollisionParameter ModelCapsule(const Matrix4& othermat_, OCT_ID id, Capsule c);

//�I�N�c���[�Ɛ����̂����蔻��@octNumber��Ԃ�
std::shared_ptr<std::vector<int>> OctRay(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 startPos, Vector3 endPos);
//�I�N�c���[�ƃJ�v�Z���̂����蔻��@octNumber��Ԃ�
std::shared_ptr<std::vector<int>> OctCapsule(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 startPos, Vector3 endPos, float radius);
//�I�N�c���[�Ƌ��̂����蔻��@octNumber��Ԃ�
std::shared_ptr<std::vector<int>> OctSphere(std::map<int, std::vector<CubeParameter>>* cubeVec, Matrix4 octMat, Vector3 spherePos,float radius);

//�n�a�a�Ƃn�a�a�̂����蔻��
bool OBBvsOBB(OBB &obb1,OBB &obb2);
//3�̓��ς̐�Βl�̘a�œ��e���������v�Z�������
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0);
//�n�a�a�쐬
OBB CreateOBB(const Matrix4& mat, const Vector3& scale);

//�J�v�Z���ƃJ�v�Z���̂����蔻��
CollisionParameter CapsulevsCapsule(Capsule c1, Capsule c2);
//�J�v�Z���쐬
Capsule CreateCapsule(Vector3 startPos, Vector3 endPos, float radius);

//���ƃ��C�̂����蔻��
CollisionParameter IsCollideLineToSphere(Vector3 posS, Vector3 posE, Vector3 posSphere, float sphereRadius,Vector3 nor = vector3(0,1,0));

//���Ƌ��̂����蔻��
CollisionParameter IsCollideSphereToSphere(Vector3 posS, Vector3 posE, float radiusS, float radiusE);

//���ƃJ�v�Z���̂����蔻��
CollisionParameter IsCollideCapsuleToSphere(Vector3 posS, Vector3 posE, Vector3 posSphere, float sphereRadius, Vector3 nor = vector3(0, 1, 0));

//�O�p�`�Ɛ����̂����蔻��
CollisionParameter IsCollideTriangleLine(Vector3 a, Vector3 b, Vector3 c, Vector3 pos, Vector3 down);

CollisionParameter IsCollideTriangleSphere(Vector3 a, Vector3 b, Vector3 c, Vector3 pos, float radiuw);