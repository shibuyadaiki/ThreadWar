#pragma once
#include "Vector2.h"
#include "MathUtility.h"
#include <cmath>
namespace RCVector2{
	float distance_(const Vector2& my, const Vector2& target);//�Q�_�Ԃ̋��������߂�
	Vector2 velocity(const Vector2& my, const Vector2& target, float speed = 1.0f);//�^�[�Q�b�g�����ւ̈ړ��ʂ����߂�
	Vector2 velocity(float angle, float speed = 1.0f);//�w������ւ̈ړ��ʂ����߂�
	float angle(const Vector2& v);//�x�N�g���̌����Ă���p�x�����߂�
	float angle(const Vector2& my, const Vector2& target);//�^�[�Q�b�g�����ւ̊p�x�����߂�
	float innerAngle(const Vector2& a, const Vector2& b);//�Q�̃x�N�g���̂Ȃ��p�x�����߂�
	Vector2 lerp(const Vector2& start, const Vector2& end, float t);//�x�N�g���̐��`���
	Vector2 lerp2(const Vector2& start, const Vector2& end, float t);//�x�N�g���̐��`���
	Vector2 lerp3(const Vector2& start, const Vector2& end, float t);//�x�N�g���̐��`���
	Vector2 lerp4(const Vector2& start, const Vector2& end, float t);//�x�N�g���̐��`���
	Vector2 lerp5(const Vector2& start, const Vector2& end, float t);//�x�N�g���̐��`���
	float normalize(float angle);//�p�x�̐��K��
	float subtractAngle(float my, float target);//�p�x�������߂�
	float subtractAngle(const Vector2& my, float dir, const Vector2& target);//�^�[�Q�b�g�����ւ̊p�x�������߂�
}