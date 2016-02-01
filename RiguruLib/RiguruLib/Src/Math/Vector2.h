#pragma once

struct Vector2{
	float x;
	float y;
};
//Vector2作成
Vector2 vector2(float x, float y);

namespace RCVector2{
	//内積(dot)を求める
	float dot(const Vector2& v1, const Vector2& v2);
	//外積を求める
	float cross(const Vector2& v1, const Vector2& v2);
	//長さ(ノルム、レンジ)を求める
	float length(const Vector2& v);

	//正規化
	Vector2 normalize(const Vector2& v);
}

//単項演算子オーバーロード
Vector2 operator + (const Vector2& v);
Vector2 operator - (const Vector2& v);

//代入演算子オーバーロード
Vector2& operator += (Vector2& v1, const Vector2& v2);
Vector2& operator -= (Vector2& v1, const Vector2& v2);
Vector2& operator *= (Vector2& v, float s);
Vector2& operator /= (Vector2& v, float s);

//２項演算子オーバーロード
Vector2 operator + (const Vector2& v1, const Vector2& v2);
Vector2 operator - (const Vector2& v1, const Vector2& v2);
Vector2 operator * (const Vector2& v, float s);
Vector2 operator * (float s, const Vector2& v);
Vector2 operator / (const Vector2& v, float s);
