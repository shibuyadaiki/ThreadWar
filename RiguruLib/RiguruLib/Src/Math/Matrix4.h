#pragma once
#include "Vector3.h"

struct Matrix4{
	float m[4][4];
};


namespace RCMatrix4{

	//�P�ʍs��
	const Matrix4& Identity();

	//�g��k���s��
	Matrix4 scale(const Vector3& s);

	//��]�s��
	Matrix4 rotateX(float angle);
	Matrix4 rotateY(float angle);
	Matrix4 rotateZ(float angle);

	//���s�ړ��s��
	Matrix4 translate(const Vector3& pos);

	//�}�g���b�N�X�����߂� �X�P�[���@X���@Y���@Z���@�ړ���
	Matrix4 matrix(const Vector3& s = vector3(1,1,1), const float& rototeXangle = 0, const float& rotateYangle = 0, const float& rotateZangle = 0, const Vector3& pos = vector3(0,0,0));

	//�}�g���b�N�X�����߂�
	Matrix4 matrix(const Matrix4& scale_ = Identity(), const Matrix4& rotateX_ = Identity(), const Matrix4& rotateY_ = Identity(), const Matrix4& rotateZ_ = Identity(), const Matrix4& translate_ = Identity());

	//���W�ϊ�(�x�N�g���ƍs��̐�)
	Vector3 transform(const Vector3& v, const Matrix4& m);

	//���W�ϊ�(��]�̂�)
	Vector3 transformRotateOnly(const Vector3& v, const Matrix4& m);

	//�]�u�s��
	Matrix4 transpose(const Matrix4& m);
	Matrix4 transposeRotateOnly(const Matrix4& m);

	//�t�s��(�g�k����)
	Matrix4 inverseFast(const Matrix4& m);

	//�t�s��
	Matrix4 inverse(const Matrix4& m);
}

//����ϊ��s��
Matrix4 lookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

//�����ϊ��s����쐬����֐�
Matrix4 perspective(float fov, float aspect, float zn, float zf);

//�X�N���[�����W�ϊ��s����쐬����֐�
Matrix4 screen(float x, float y, float width, float height);

//������Z�q�I�[�o�[���[�h
Matrix4& operator *= (Matrix4& m1, const Matrix4& m2);

//2�����Z�q�I�[�o�[���[�h
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2);
Vector3 operator * (const Vector3& v, const Matrix4& m);
