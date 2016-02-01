#pragma once

#include "Vector3.h"
#include "MathUtility.h"
#include <cmath>
namespace RCVector3{
	//xutil�Ƌ�������̂�_������
	float distance_(const Vector3& my, const Vector3& target);//�Q�_�Ԃ̋���
	Vector3 direction(const Vector3& my, const Vector3& target);//���������߂�
	Vector3 velocity(const Vector3& my, const Vector3& target, float speed = 1.0f);//�^�[�Q�b�g�����ւ̈ړ��ʂ����߂�
	Vector3 lerp(const Vector3& start, const Vector3& end, float t);//�x�N�g���̐��`���
	Vector3 interpolatePower(const Vector3& start, const Vector3& end, float t, float power);//�ׂ���֐����g�p������Ԋ֐�
	Vector3 interpolateSinPower(const Vector3& start, const Vector3& end, float t, float power);	//�ׂ���֐�(pow �֐�)�ƎO�p�֐��isin �֐��j�g�p������Ԋ֐�
	Vector3 fromPitchYaw(float pitch, float yaw);//�s�b�`�ƃ��[����x�N�g�������߂�
	float pitch(const Vector3& v);//�s�b�`�����߂�
	float yaw(const Vector3& v);//���[�����߂�
	Vector3 abs(const Vector3& v);
}