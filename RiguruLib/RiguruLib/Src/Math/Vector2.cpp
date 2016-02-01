#include "Vector2.h"
#include <cmath>

//Vector2�쐬
Vector2 vector2(float x, float y){
	Vector2 result = { x, y };
	return result;
}

//����(dot)�����߂�
float RCVector2::dot(const Vector2& v1, const Vector2& v2){
	return v1.x * v2.x + v1.y * v2.y;
}
//�O�ς����߂�
float RCVector2::cross(const Vector2& v1, const Vector2& v2){
	return v1.x * v2.y - v2.x * v1.y;
}

//����(�m�����A�����W)�����߂�
float RCVector2::length(const Vector2& v){
	return (float)std::sqrt(RCVector2::dot(v, v));
}

//���K��
Vector2 RCVector2::normalize(const Vector2& v){
	Vector2 result = v;
	float len = RCVector2::length(result);
	if (len != 0){
		result /= len;
	}
	return result;
}

//�P�����Z�q�I�[�o�[���[�h
Vector2 operator + (const Vector2& v){
	return v;
}

Vector2 operator - (const Vector2& v){
	Vector2 result = vector2(-v.x, -v.y);
	return result;
}

//������Z�q�I�[�o�[���[�h
Vector2& operator += (Vector2& v1, const Vector2& v2){
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

Vector2& operator -= (Vector2& v1, const Vector2& v2){
	v1.x -= v2.x;
	v1.y -= v2.y;
	return v1;
}

Vector2& operator *= (Vector2& v, float s){
	v.x *= s;
	v.y *= s;
	return v;
}

Vector2& operator /= (Vector2& v, float s){
	v.x /= s;
	v.y /= s;
	return v;
}

//�Q�����Z�q�I�[�o�[���[�h
Vector2 operator + (const Vector2& v1, const Vector2& v2){
	Vector2 result = v1;
	return result += v2;
}

Vector2 operator - (const Vector2& v1, const Vector2& v2){
	Vector2 result = v1;
	return result -= v2;
}

Vector2 operator * (const Vector2& v, float s){
	Vector2 result = v;
	return result *= s;
}

Vector2 operator * (float s, const Vector2& v){
	return v * s;
}

Vector2 operator / (const Vector2& v, float s){
	Vector2 result = v;
	return result /= s;
}