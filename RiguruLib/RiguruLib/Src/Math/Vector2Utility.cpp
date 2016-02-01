#include "Vector2Utility.h"

float RCVector2::distance_(const Vector2& my, const Vector2& target){
	return length(target - my);
}
Vector2 RCVector2::velocity(const Vector2& my, const Vector2& target, float speed){
	return normalize(target - my) * speed;
}
Vector2 RCVector2::velocity(float angle, float speed){
	return vector2(Math::cos(angle), Math::sin(angle))* speed;
}
float RCVector2::angle(const Vector2& v){
	return Math::atan(v.y, v.x);
}
float RCVector2::angle(const Vector2& my, const Vector2& target){
	return angle(target - my);
}
float RCVector2::innerAngle(const Vector2& a, const Vector2& b){
	return Math::acos(dot(normalize(a), normalize(b)));
}
Vector2 RCVector2::lerp(const Vector2& start, const Vector2& end, float t){
	if (t < 0.0f){
		t = 0.0f;
	}
	else if (t > 1.0f){
		t = 1.0f;
	}
	return start * (1.0f - t) + end * t;
}
Vector2 RCVector2::lerp2(const Vector2& start, const Vector2& end, float t){
	if (t < 0.0f){
		t = 0.0f;
	}
	else if (t > 1.0f){
		t = 1.0f;
	}
	t = sinf(Math::radian(t*90.0f));
	return start * (1.0f - t) + end * t;
}
Vector2 RCVector2::lerp3(const Vector2& start, const Vector2& end, float t){
	if (t < 0.0f){
		t = 0.0f;
	}
	else if (t > 1.0f){
		t = 1.0f;
	}
	t = (2.0f - t) * t;
	return start * (1.0f - t) + end * t;
}
Vector2 RCVector2::lerp4(const Vector2& start, const Vector2& end, float t){
	t = t * t;
	return start * (1.0f - t) + end * t;
}

Vector2 RCVector2::lerp5(const Vector2& start, const Vector2& end, float t){
	if (t < 0.0f){
		t = 0.0f;
	}
	else if (t > 1.0f){
		t = 1.0f;
	}
	t = -std::pow(2.0f, -10.0f * t) + 1.0f;
	return start * (1.0f - t) + end * t;
}
float RCVector2::normalize(float angle){
	return std::fmod(angle, 360.0f);
}
float RCVector2::subtractAngle(float my, float target){
	float d = normalize(target - my);
	if (d > 180.0f){
		d -= 360.0f;
	}
	else if (d < -180.0f) {
		d += 360.0f;
	}
	return d;
}
float RCVector2::subtractAngle(const Vector2& my, float dir, const Vector2& target){
	return subtractAngle(dir, angle(my, target));
}