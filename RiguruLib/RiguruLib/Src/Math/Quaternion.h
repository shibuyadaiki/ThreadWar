#pragma once
struct Quaternion {
	float x; // x成分
	float y; // y成分
	float z; // z成分
	float w; // w成分
};
struct Vector3;
struct Matrix4;
// クオータニオンを作成
Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
Quaternion quaternion(const Vector3& v, float angle);
//回転行列からクォータニオンを求める
Quaternion quaternion(const Matrix4& m);
namespace RCQuaternion{
	// 内積を求める
	float dot(const Quaternion& q1, const Quaternion& q2);
	// ノルムを求める
	float length(const Quaternion& q);
	// 正規化する
	Quaternion normalize(const Quaternion& q);

	//便利関数
	//クォータニオンを球面線形補間
	Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
	//クォータニオンから回転行列へ
	Matrix4 rotate(const Quaternion& q);
	//クォータニオンから回転行列へ
	Matrix4 rotate2(const Quaternion& q);
	//任意軸周りの回転行列を求める関数(glRotatefと同様)
	Matrix4 rotate(const Vector3& v, float angle);
}

// 単項演算子オーバーロード
Quaternion operator + (const Quaternion& q);
Quaternion operator - (const Quaternion& q);
// 代入演算子オーバーロード
Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q, float s);
Quaternion& operator /= (Quaternion& q, float s);
Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);
// ２項演算子オーバーロード
Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
Quaternion operator * (const Quaternion& q, float s);
Quaternion operator * (float s, const Quaternion& q);
Quaternion operator / (const Quaternion& q, float s);