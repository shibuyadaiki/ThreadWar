#pragma once
#include "Vector3.h"

struct Matrix4{
	float m[4][4];
};


namespace RCMatrix4{

	//単位行列
	const Matrix4& Identity();

	//拡大縮小行列
	Matrix4 scale(const Vector3& s);

	//回転行列
	Matrix4 rotateX(float angle);
	Matrix4 rotateY(float angle);
	Matrix4 rotateZ(float angle);

	//平行移動行列
	Matrix4 translate(const Vector3& pos);

	//マトリックスを求める スケール　X軸　Y軸　Z軸　移動量
	Matrix4 matrix(const Vector3& s = vector3(1,1,1), const float& rototeXangle = 0, const float& rotateYangle = 0, const float& rotateZangle = 0, const Vector3& pos = vector3(0,0,0));

	//マトリックスを求める
	Matrix4 matrix(const Matrix4& scale_ = Identity(), const Matrix4& rotateX_ = Identity(), const Matrix4& rotateY_ = Identity(), const Matrix4& rotateZ_ = Identity(), const Matrix4& translate_ = Identity());

	//座標変換(ベクトルと行列の積)
	Vector3 transform(const Vector3& v, const Matrix4& m);

	//座標変換(回転のみ)
	Vector3 transformRotateOnly(const Vector3& v, const Matrix4& m);

	//転置行列
	Matrix4 transpose(const Matrix4& m);
	Matrix4 transposeRotateOnly(const Matrix4& m);

	//逆行列(拡縮除く)
	Matrix4 inverseFast(const Matrix4& m);

	//逆行列
	Matrix4 inverse(const Matrix4& m);
}

//視野変換行列
Matrix4 lookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

//透視変換行列を作成する関数
Matrix4 perspective(float fov, float aspect, float zn, float zf);

//スクリーン座標変換行列を作成する関数
Matrix4 screen(float x, float y, float width, float height);

//代入演算子オーバーロード
Matrix4& operator *= (Matrix4& m1, const Matrix4& m2);

//2項演算子オーバーロード
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2);
Vector3 operator * (const Vector3& v, const Matrix4& m);
