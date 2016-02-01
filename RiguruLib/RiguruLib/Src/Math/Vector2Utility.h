#pragma once
#include "Vector2.h"
#include "MathUtility.h"
#include <cmath>
namespace RCVector2{
	float distance_(const Vector2& my, const Vector2& target);//２点間の距離を求める
	Vector2 velocity(const Vector2& my, const Vector2& target, float speed = 1.0f);//ターゲット方向への移動量を求める
	Vector2 velocity(float angle, float speed = 1.0f);//指定方向への移動量を求める
	float angle(const Vector2& v);//ベクトルの向いている角度を求める
	float angle(const Vector2& my, const Vector2& target);//ターゲット方向への角度を求める
	float innerAngle(const Vector2& a, const Vector2& b);//２つのベクトルのなす角度を求める
	Vector2 lerp(const Vector2& start, const Vector2& end, float t);//ベクトルの線形補間
	Vector2 lerp2(const Vector2& start, const Vector2& end, float t);//ベクトルの線形補間
	Vector2 lerp3(const Vector2& start, const Vector2& end, float t);//ベクトルの線形補間
	Vector2 lerp4(const Vector2& start, const Vector2& end, float t);//ベクトルの線形補間
	Vector2 lerp5(const Vector2& start, const Vector2& end, float t);//ベクトルの線形補間
	float normalize(float angle);//角度の正規化
	float subtractAngle(float my, float target);//角度差を求める
	float subtractAngle(const Vector2& my, float dir, const Vector2& target);//ターゲット方向への角度差を求める
}