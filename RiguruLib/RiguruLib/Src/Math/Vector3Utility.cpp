#include "Vector3Utility.h"

float RCVector3::distance_(const Vector3& my, const Vector3& target){
	return length(target - my);
}

Vector3 RCVector3::direction(const Vector3& my, const Vector3& target){
	return normalize(target - my);
}

Vector3 RCVector3::velocity(const Vector3& my, const Vector3& target, float speed){
	return normalize(target - my) * speed;	
}

Vector3 RCVector3::lerp(const Vector3& start, const Vector3& end, float t){
	if (t < 0.0f){
		t = 0.0f;
	}
	else if (t > 1.0f){
		t = 1.0f;
	}
	return start * (1.0f - t) + end * t;
}

Vector3 RCVector3::interpolatePower(const Vector3& start, const Vector3& end, float t, float power){
	return lerp(start, end, std::pow(t, power));
}
Vector3 RCVector3::interpolateSinPower(const Vector3& start, const Vector3& end, float t, float power){
	return lerp(start, end, std::pow(Math::sin(90.0f * t), power));
}
Vector3 RCVector3::fromPitchYaw(float pitch, float yaw){
	Vector3 result = {
		Math::cos(pitch) * Math::sin(yaw),
		-Math::sin(pitch),
		Math::cos(pitch) * Math::cos(yaw)
	};

	return result;
}
float RCVector3::pitch(const Vector3& v){
	float l;
	if ((l = length(v)) == 0){
		return 0.0f;
	}
	float y = v.y / l;
	return Math::asin(-y);
}
float RCVector3::yaw(const Vector3& v){
	if (length(v) == 0){
		return 0.0f;
	}

	return Math::atan(v.x, v.z);
}

Vector3 RCVector3::abs(const Vector3& v){
	Vector3 v1 = v;
	if (v1.x < 0)v1.x = v1.x * -1;
	if (v1.y < 0)v1.y = v1.y * -1;
	if (v1.z < 0)v1.z = v1.z * -1;
	return v1;
}