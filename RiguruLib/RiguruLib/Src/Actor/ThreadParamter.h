#pragma once
#include "../Math/Vector3.h"
//糸のもつパラメーター
struct ThreadParameter
{
	//糸始点（発生点）
	Vector3 startPosition;
	//糸終点
	Vector3 endPosition;
	//現在の糸の終点。
	Vector3 currentEndPosition;
	//糸の終点から始点へ向かう単位ベクトル
	Vector3 startNormal;
	//糸の始点から終点へ向かう単位ベクトル
	Vector3 endNormal;

	bool startPosIsGroundTouch = true;
};