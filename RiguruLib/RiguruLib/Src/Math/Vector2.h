#pragma once

struct Vector2{
	float x;
	float y;
};
//Vector2�쐬
Vector2 vector2(float x, float y);

namespace RCVector2{
	//����(dot)�����߂�
	float dot(const Vector2& v1, const Vector2& v2);
	//�O�ς����߂�
	float cross(const Vector2& v1, const Vector2& v2);
	//����(�m�����A�����W)�����߂�
	float length(const Vector2& v);

	//���K��
	Vector2 normalize(const Vector2& v);
}

//�P�����Z�q�I�[�o�[���[�h
Vector2 operator + (const Vector2& v);
Vector2 operator - (const Vector2& v);

//������Z�q�I�[�o�[���[�h
Vector2& operator += (Vector2& v1, const Vector2& v2);
Vector2& operator -= (Vector2& v1, const Vector2& v2);
Vector2& operator *= (Vector2& v, float s);
Vector2& operator /= (Vector2& v, float s);

//�Q�����Z�q�I�[�o�[���[�h
Vector2 operator + (const Vector2& v1, const Vector2& v2);
Vector2 operator - (const Vector2& v1, const Vector2& v2);
Vector2 operator * (const Vector2& v, float s);
Vector2 operator * (float s, const Vector2& v);
Vector2 operator / (const Vector2& v, float s);
