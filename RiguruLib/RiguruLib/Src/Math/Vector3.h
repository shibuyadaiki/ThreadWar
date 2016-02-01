#pragma once

struct Vector3{
	float x;
	float y;
	float z;
};
//Vector3�쐬
Vector3 vector3(float x, float y,float z);

namespace RCVector3{

	//����(dot)�����߂�
	float dot(const Vector3& v1, const Vector3& v2);

	// �O��(cross)�����߂�
	Vector3 cross(const Vector3& v1, const Vector3& v2);

	//����(�m�����A�����W)�����߂�
	float length(const Vector3& v);

	//���K��
	Vector3 normalize(const Vector3& v);

	Vector3 CreateNormal(Vector3 v, Vector3 v1, Vector3 v2);
}
//�P�����Z�q�I�[�o�[���[�h
Vector3 operator + (const Vector3& v);
Vector3 operator - (const Vector3& v);

//������Z�q�I�[�o�[���[�h
Vector3& operator += (Vector3& v1, const Vector3& v2);
Vector3& operator -= (Vector3& v1, const Vector3& v2);
bool operator == (const Vector3& v1,const Vector3& v2);
Vector3& operator *= (Vector3& v, float s);
Vector3& operator /= (Vector3& v, float s);

//�Q�����Z�q�I�[�o�[���[�h
Vector3 operator + (const Vector3& v1, const Vector3& v2);
Vector3 operator - (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Vector3& v, float s);
Vector3 operator * (float s, const Vector3& v);
Vector3 operator / (const Vector3& v, float s);
