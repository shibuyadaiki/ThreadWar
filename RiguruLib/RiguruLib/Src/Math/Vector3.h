#pragma once

struct Vector3{
	float x;
	float y;
	float z;
};
//Vector3作成
Vector3 vector3(float x, float y,float z);

namespace RCVector3{

	//内積(dot)を求める
	float dot(const Vector3& v1, const Vector3& v2);

	// 外積(cross)を求める
	Vector3 cross(const Vector3& v1, const Vector3& v2);

	//長さ(ノルム、レンジ)を求める
	float length(const Vector3& v);

	//正規化
	Vector3 normalize(const Vector3& v);

	Vector3 CreateNormal(Vector3 v, Vector3 v1, Vector3 v2);
}
//単項演算子オーバーロード
Vector3 operator + (const Vector3& v);
Vector3 operator - (const Vector3& v);

//代入演算子オーバーロード
Vector3& operator += (Vector3& v1, const Vector3& v2);
Vector3& operator -= (Vector3& v1, const Vector3& v2);
bool operator == (const Vector3& v1,const Vector3& v2);
Vector3& operator *= (Vector3& v, float s);
Vector3& operator /= (Vector3& v, float s);

//２項演算子オーバーロード
Vector3 operator + (const Vector3& v1, const Vector3& v2);
Vector3 operator - (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Vector3& v, float s);
Vector3 operator * (float s, const Vector3& v);
Vector3 operator / (const Vector3& v, float s);
