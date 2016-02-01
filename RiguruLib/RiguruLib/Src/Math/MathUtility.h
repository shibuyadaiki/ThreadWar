#pragma once

class Math{
public:
	static const float PI;

	static float sin(float angle);
	static float cos(float angle);
	static float tan(float angle);

	static float asin(float x);
	static float acos(float x);
	static float atan(float y, float x);

	static float radian(float angle);
	static float angle(float radian);

	static float clamp(float target, float mn, float mx);
	//ノーマルLerp。
	static float lerp(float s, float e, float t);
	//sinLerp。
	static float lerp2(float s, float e, float t);
	//最初速い最後ゆっくりLerp。
	static float lerp3(float s, float e, float t);
	//最初ゆっくり最後速いLerp。
	static float lerp4(float s, float e, float t);
	//バウンドLerp。
	static float lerp5(float s, float e, float t);
	static float random(float _min, float _max);
};