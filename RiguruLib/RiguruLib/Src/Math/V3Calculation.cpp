#include "V3Calculation.h"

float RCV3Calc::InnerAngle(const Vector3& v1, const Vector3& v2)
{
	return Math::acos(RCVector3::dot(RCVector3::normalize(v1), RCVector3::normalize(v2)));
}

Vector3 RCV3Calc::Direction(const Vector3& p1, const Vector3& p2)
{
	return RCVector3::normalize(p2 - p1);
}