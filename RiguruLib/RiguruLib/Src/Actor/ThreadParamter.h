#pragma once
#include "../Math/Vector3.h"
//���̂��p�����[�^�[
struct ThreadParameter
{
	//���n�_�i�����_�j
	Vector3 startPosition;
	//���I�_
	Vector3 endPosition;
	//���݂̎��̏I�_�B
	Vector3 currentEndPosition;
	//���̏I�_����n�_�֌������P�ʃx�N�g��
	Vector3 startNormal;
	//���̎n�_����I�_�֌������P�ʃx�N�g��
	Vector3 endNormal;

	bool startPosIsGroundTouch = true;
};