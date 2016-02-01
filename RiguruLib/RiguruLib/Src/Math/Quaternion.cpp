#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "MathUtility.h"
#include <cmath>

Quaternion quaternion(float x,float y,float z,float w){
	Quaternion result = {x,y,z,w};
	return result;
}
Quaternion quaternion(const Vector3& v,float angle){
	float sin = Math::sin(angle / 2.0f);
	return quaternion(sin * v.x,sin * v.y,sin * v.z,Math::cos(angle / 2.0f));
}
Quaternion quaternion(const Matrix4& m){
	Quaternion result;
	float tr = m.m[0][0] + m.m[1][1] + m.m[2][2] + m.m[3][3];
	if (tr >= 1.0f){
		float fourD = 2.0f * std::sqrt(tr);
		result.x = (m.m[1][2] - m.m[2][1]) / fourD;
		result.y = (m.m[2][0] - m.m[0][2]) / fourD;
		result.z = (m.m[0][1] - m.m[1][0]) / fourD;
		result.w = fourD / 4.0f;
		return result;
	}
	int i = 0;
	if (m.m[0][0] <= m.m[1][1]){
		i = 1;
	}
	if (m.m[2][2] > m.m[i][i]){
		i = 2;
	}
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	tr = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;
	float fourD = 2.0f * std::sqrt(tr);
	float qa[4];
	qa[i] = fourD / 4.0f;
	qa[j] = (m.m[j][i] + m.m[i][j]) / fourD;
	qa[k] = (m.m[k][i] + m.m[i][k]) / fourD;
	qa[3] = (m.m[j][k] - m.m[k][j]) / fourD;
	result.x = qa[0];
	result.y = qa[1];
	result.z = qa[2];
	result.w = qa[3];
	return result;
}
float RCQuaternion::dot(const Quaternion& q1,const Quaternion& q2){
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}
float RCQuaternion::length(const Quaternion& q){
	return (float)std::sqrt(dot(q,q));
}
Quaternion RCQuaternion::normalize(const Quaternion & q)
{
	Quaternion result = q;
	float len = length( result );
	if ( len != 0 ) {
		result /= len;
	}
	return result;
}
Quaternion operator+(const Quaternion& q){
	return q;
}
Quaternion operator-(const Quaternion& q){
	Quaternion result = {-q.x,-q.y,-q.z,-q.w};
	return result;
}
Quaternion& operator += ( Quaternion& q1, const Quaternion& q2 )
{
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	q1.w += q2.w;
	return q1;
}
Quaternion& operator -= ( Quaternion& q1, const Quaternion& q2 )
{
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	q1.w -= q2.w;
	return q1;
}
Quaternion& operator *= ( Quaternion& q1, const Quaternion& q2 )
{
	Quaternion result = {
		 q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
		-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
		 q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
		-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w
	};
	q1 = result;;
	return q1;
}
Quaternion& operator *= (Quaternion& q1,float s){
	q1.x *= s;
	q1.y *= s;
	q1.z *= s;
	q1.w *= s;
	return q1;
}
Quaternion& operator /= (Quaternion& q,float s){
	return q *= 1.0f / s;
}
Quaternion operator + (const Quaternion& q1,const Quaternion& q2){
	Quaternion result = q1;
	return result += q2;
}
Quaternion operator -(const Quaternion& q1,const Quaternion& q2){
	Quaternion result = q1;
	return result -= q2;
}
Quaternion operator * (const Quaternion& q1,const Quaternion& q2){
	Quaternion result = q1;
	return result *= q2;
}
Quaternion operator * ( const Quaternion& q, float s )
{
	Quaternion result = q;
	return result *= s;
}
Quaternion operator * ( float s, const Quaternion& q )
{
	Quaternion result = q;
	return result *= s;
}
Quaternion operator / ( const Quaternion & q, float s )
{
	Quaternion result = q;
	return result /= s;
}
//クォータニオンを球面線形補間する
Quaternion RCQuaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t) {
	float cos = dot(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f) {
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - cos) > 0.001f) {
		float theta = (float)std::acos(cos);
		k0 = (float)(std::sin(theta*k0) / std::sin(theta));
		k1 = (float)(std::sin(theta*k1) / std::sin(theta));
	}
	return q1*k0 + t2*k1;
}
Matrix4 RCQuaternion::rotate(const Quaternion& q) {
	float xx = q.x * q.x * 2.0f;
	float yy = q.y * q.y * 2.0f;
	float zz = q.z * q.z * 2.0f;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;
	Matrix4 result = {
		1.0f-yy-zz, xy+wz, xz-wy, 0.0f,
		xy-wz, 1.0f-xx-zz, yz+wx, 0.0f,
		xz+wy, yz-wx, 1.0f-xx-yy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}
//任意軸まわりの回転行列を求める関数
Matrix4 RCQuaternion::rotate(const Vector3& v, float angle){
	return rotate(quaternion(v,angle));
}