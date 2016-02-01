#include "Converter.h"

//D3DXVECTOR2�ɕϊ�
D3DXVECTOR2 RConvert(Vector2* v){
	D3DXVECTOR2 result;
	result.x = v->x;
	result.y = v->y;
	return result;
}
//D3DXVECTOR3�ɕϊ�
D3DXVECTOR3 RConvert(Vector3* v){
	D3DXVECTOR3 result;
	result.x = v->x;
	result.y = v->y;
	result.z = v->z;
	return result;
}
//D3DXMATRIX�ɕϊ�
D3DXMATRIX RConvert(Matrix4* m){
	D3DXMATRIX result;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			result.m[i][j] = m[0].m[i][j];
		}
	}
	return result;
}

//D3DXQUATERNION�ɕϊ�
D3DXQUATERNION RConvert(Quaternion* q){
	D3DXQUATERNION result;
	result.x = q->x; result.y = q->y; result.z = q->z; result.w = q->w;
	return result;
}

//Vector2�ɕϊ�
Vector2 RConvert(D3DXVECTOR2* v){
	Vector2 result;
	result.x = v->x;
	result.y = v->y;
	return result;
}
//Vector3�ɕϊ�
Vector3 RConvert(D3DXVECTOR3* v){
	Vector3 result;
	result.x = v->x;
	result.y = v->y;
	result.z = v->z;
	return result;
}
//Matrix4�ɕϊ�
Matrix4 RConvert(D3DXMATRIX* m){
	Matrix4 result;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			result.m[i][j] = m[0].m[i][j];
		}
	}
	//���W�Z�b�g�ȂǊ֐���OpenGL�̍��W�n�Ȃ̂ō��킹��B
	//�g�p����Ƃ��ɂ܂��g�����X�|�[�Y������
	//result = RCMatrix4::transpose(result);
	return result;
}
//Quaternion�ɕϊ�
Quaternion RConvert(D3DXQUATERNION* q){
	Quaternion result;
	result.x = q->x; result.y = q->y; result.z = q->z; result.w = q->w;
	return result;
}