//Vector3�̌v�Z���ꂱ��
#pragma once
#include "Vector3Utility.h"

namespace RCV3Calc
{
	//�Q�x�N�g���̂Ȃ��p���擾
	float InnerAngle(const Vector3& v1, const Vector3& v2);
	//p1����p2�֌������P�ʃx�N�g�����擾
	Vector3 Direction(const Vector3& p1, const Vector3& p2);
}