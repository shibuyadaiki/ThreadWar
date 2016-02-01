#pragma once

#include "Vector3.h"
#include "MathUtility.h"
#include <cmath>
namespace RCVector3{
	//xutilと競合するので_を入れる
	float distance_(const Vector3& my, const Vector3& target);//２点間の距離
	Vector3 direction(const Vector3& my, const Vector3& target);//方向を求める
	Vector3 velocity(const Vector3& my, const Vector3& target, float speed = 1.0f);//ターゲット方向への移動量を求める
	Vector3 lerp(const Vector3& start, const Vector3& end, float t);//ベクトルの線形補間
	Vector3 interpolatePower(const Vector3& start, const Vector3& end, float t, float power);//べき乗関数を使用した補間関数
	Vector3 interpolateSinPower(const Vector3& start, const Vector3& end, float t, float power);	//べき乗関数(pow 関数)と三角関数（sin 関数）使用した補間関数
	Vector3 fromPitchYaw(float pitch, float yaw);//ピッチとヨーからベクトルを求める
	float pitch(const Vector3& v);//ピッチを求める
	float yaw(const Vector3& v);//ヨーを求める
	Vector3 abs(const Vector3& v);
}