#include "MatrixUtility.h"
#include "Quaternion.h"

Vector3 RCMatrix4::getPosition(const Matrix4& m){
	Vector3 result = { m.m[3][0], m.m[3][1], m.m[3][2] };
	return result;
}
Vector3 RCMatrix4::getLeft(const Matrix4& m){
	Vector3 result = { m.m[0][0], m.m[0][1], m.m[0][2] };
	return result;
}
Vector3 RCMatrix4::getUp(const Matrix4& m){
	Vector3 result = { m.m[1][0], m.m[1][1], m.m[1][2] };
	return result;
}
Vector3 RCMatrix4::getFront(const Matrix4& m){
	Vector3 result = { m.m[2][0], m.m[2][1], m.m[2][2] };
	return result;
}
Vector3 RCMatrix4::getScale(const Matrix4& m){
	Vector3 result = {
		RCVector3::length(getLeft(m)),
		RCVector3::length(getUp(m)),
		RCVector3::length(getFront(m))
	};
	return result;
}
Matrix4& RCMatrix4::setPosition(Matrix4& m, const Vector3& position){
	m.m[3][0] = position.x;
	m.m[3][1] = position.y;
	m.m[3][2] = position.z;

	return m;
}
Matrix4& RCMatrix4::setLeft(Matrix4& m, const Vector3& left){
	m.m[0][0] = left.x;
	m.m[0][1] = left.y;
	m.m[0][2] = left.z;
	return m;
}
Matrix4& RCMatrix4::setUp(Matrix4& m, const Vector3& up){
	m.m[1][0] = up.x;
	m.m[1][1] = up.y;
	m.m[1][2] = up.z;
	return m;
}
Matrix4& RCMatrix4::setFront(Matrix4& m, const Vector3& front){
	m.m[2][0] = front.x;
	m.m[2][1] = front.y;
	m.m[2][2] = front.z;
	return m;
}
Matrix4 RCMatrix4::slerp(const Matrix4& start, const Matrix4& end, float t){
	//�g��k���̕⊮
	Vector3 s = RCVector3::lerp(getScale(start), getScale(end), t);
	// ��]�̕��
	Quaternion r =RCQuaternion::slerp(quaternion(normalize(start)), quaternion(normalize(end)), t);
	// �ʒu�̕��
	Vector3 p = RCVector3::lerp(getPosition(start), getPosition(end), t);
	// ��ԍs������߂�
	return scale(s) * (RCQuaternion::rotate(r)) * translate(p);
}

Matrix4 RCMatrix4::lerp(const Matrix4& start, const Matrix4& end, float t){
	Matrix4 m = {
		Math::lerp(start.m[0][0], end.m[0][0], t), Math::lerp(start.m[0][1], end.m[0][1], t), Math::lerp(start.m[0][2], end.m[0][2], t), Math::lerp(start.m[0][3], end.m[0][3], t),
		Math::lerp(start.m[1][0], end.m[1][0], t), Math::lerp(start.m[1][1], end.m[1][1], t), Math::lerp(start.m[1][2], end.m[1][2], t), Math::lerp(start.m[1][3], end.m[1][3], t),
		Math::lerp(start.m[2][0], end.m[2][0], t), Math::lerp(start.m[2][1], end.m[2][1], t), Math::lerp(start.m[2][2], end.m[2][2], t), Math::lerp(start.m[2][3], end.m[2][3], t),
		Math::lerp(start.m[3][0], end.m[3][0], t), Math::lerp(start.m[3][1], end.m[3][1], t), Math::lerp(start.m[3][2], end.m[3][2], t), Math::lerp(start.m[3][3], end.m[3][3], t)
	};

	return m;
}
float RCMatrix4::pitch(const Matrix4& m){
	return RCVector3::pitch(getFront(m));
}
float RCMatrix4::yaw(const Matrix4& m){
	return RCVector3::yaw(getFront(m));
}
float RCMatrix4::roll(const Matrix4& m){
	return Math::atan(m.m[0][1], m.m[1][1]);
}

Vector3 RCMatrix4::project(const Vector3& position,
	const Matrix4& screen,
	const Matrix4& projection,
	const Matrix4& view,
	const Matrix4& world){
	return position * world * view * projection * screen;
}
Vector3 RCMatrix4::unProject(const Vector3& position,
	const Matrix4& screen,
	const Matrix4& projection,
	const Matrix4& view,
	const Matrix4& world){
	return position * inverse(world * view * projection * screen);
}


//��]�s��̐��K��
Matrix4 RCMatrix4::normalize(const Matrix4& m){
	Matrix4 result = m;
	//�O�����̃x�N�g�����擾
	Vector3 front = getFront(m);
	//������̃x�N�g�����擾
	Vector3 up = getUp(m);
	// �������̃x�N�g�������߂�
	Vector3 left = RCVector3::normalize(RCVector3::cross(up, front));
	// ������̃x�N�g�������߂�
	up = RCVector3::normalize(RCVector3::cross(front, left));
	// �O�����̃x�N�g�������߂�
	front = RCVector3::cross(left, up);
	setLeft(result, left);
	setUp(result, up);
	setFront(result, front);
	return result;
}

