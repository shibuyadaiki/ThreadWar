#pragma once
struct Quaternion {
	float x; // x����
	float y; // y����
	float z; // z����
	float w; // w����
};
struct Vector3;
struct Matrix4;
// �N�I�[�^�j�I�����쐬
Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
Quaternion quaternion(const Vector3& v, float angle);
//��]�s�񂩂�N�H�[�^�j�I�������߂�
Quaternion quaternion(const Matrix4& m);
namespace RCQuaternion{
	// ���ς����߂�
	float dot(const Quaternion& q1, const Quaternion& q2);
	// �m���������߂�
	float length(const Quaternion& q);
	// ���K������
	Quaternion normalize(const Quaternion& q);

	//�֗��֐�
	//�N�H�[�^�j�I�������ʐ��`���
	Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
	//�N�H�[�^�j�I�������]�s���
	Matrix4 rotate(const Quaternion& q);
	//�N�H�[�^�j�I�������]�s���
	Matrix4 rotate2(const Quaternion& q);
	//�C�ӎ�����̉�]�s������߂�֐�(glRotatef�Ɠ��l)
	Matrix4 rotate(const Vector3& v, float angle);
}

// �P�����Z�q�I�[�o�[���[�h
Quaternion operator + (const Quaternion& q);
Quaternion operator - (const Quaternion& q);
// ������Z�q�I�[�o�[���[�h
Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q, float s);
Quaternion& operator /= (Quaternion& q, float s);
Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);
// �Q�����Z�q�I�[�o�[���[�h
Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q, float s);
Quaternion operator * (float s, const Quaternion& q);
Quaternion operator / (const Quaternion& q, float s);