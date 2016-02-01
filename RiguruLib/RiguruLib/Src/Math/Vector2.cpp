#include "Vector2.h"
#include <cmath>

//Vector2作成
Vector2 vector2(float x, float y){
	Vector2 result = { x, y };
	return result;
}

//内積(dot)を求める
float RCVector2::dot(const Vector2& v1, const Vector2& v2){
	return v1.x * v2.x + v1.y * v2.y;
}
//外積を求める
float RCVector2::cross(const Vector2& v1, const Vector2& v2){
	return v1.x * v2.y - v2.x * v1.y;
}

//長さ(ノルム、レンジ)を求める
float RCVector2::length(const Vector2& v){
	return (float)std::sqrt(RCVector2::dot(v, v));
}

//正規化
Vector2 RCVector2::normalize(const Vector2& v){
	Vector2 result = v;
	float len = RCVector2::length(result);
	if (len != 0){
		result /= len;
	}
	return result;
}

//単項演算子オーバーロード
Vector2 operator + (const Vector2& v){
	return v;
}

Vector2 operator - (const Vector2& v){
	Vector2 result = vector2(-v.x, -v.y);
	return result;
}

//代入演算子オーバーロード
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

//２項演算子オーバーロード
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