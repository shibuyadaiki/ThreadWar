#pragma once
#include "Matrix4.h"
#include "Vector3Utility.h"

namespace RCMatrix4{
	Vector3 getPosition(const Matrix4& m);
	Vector3 getLeft(const Matrix4& m);
	Vector3 getUp(const Matrix4& m);
	Vector3 getFront(const Matrix4& m);
	Vector3 getScale(const Matrix4& m);
	Matrix4& setPosition(Matrix4& m, const Vector3& position);
	Matrix4& setLeft(Matrix4& m, const Vector3& left);
	Matrix4& setUp(Matrix4& m, const Vector3& up);
	Matrix4& setFront(Matrix4& m, const Vector3& front);
	Matrix4 slerp(const Matrix4& start, const Matrix4& end, float t);
	Matrix4 lerp(const Matrix4& start, const Matrix4& end, float t);
	float pitch(const Matrix4& m);
	float yaw(const Matrix4& m);
	float roll(const Matrix4& m);

	Vector3 project(const Vector3& position,
		const Matrix4& screen,
		const Matrix4& projection,
		const Matrix4& view,
		const Matrix4& world = Identity());
	Vector3 unProject(const Vector3& position,
		const Matrix4& screen,
		const Matrix4& projection,
		const Matrix4& view,
		const Matrix4& world = Identity());


	//âÒì]çsóÒÇÃê≥ãKâª
	Matrix4 normalize(const Matrix4& m);
}