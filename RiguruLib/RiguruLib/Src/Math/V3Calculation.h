//Vector3の計算あれこれ
#pragma once
#include "Vector3Utility.h"

namespace RCV3Calc
{
	//２ベクトルのなす角を取得
	float InnerAngle(const Vector3& v1, const Vector3& v2);
	//p1からp2へ向かう単位ベクトルを取得
	Vector3 Direction(const Vector3& p1, const Vector3& p2);
}