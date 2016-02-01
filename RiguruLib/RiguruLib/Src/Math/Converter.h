#pragma once
#include "../Other/DX11User.h"
#include "MathUtility.h"
#include "MatrixUtility.h"
#include "Vector2Utility.h"
#include "Vector3Utility.h"
#include "Quaternion.h"

	//D3DXVECTOR2�ɕϊ�
D3DXVECTOR2 RConvert(Vector2* v);
	//D3DXVECTOR3�ɕϊ�
	D3DXVECTOR3 RConvert(Vector3* v);
	//D3DXMATRIX�ɕϊ�
	D3DXMATRIX RConvert(Matrix4* m);
	//D3DXQUATERNION�ɕϊ�
	D3DXQUATERNION RConvert(Quaternion* q);

	//Vector2�ɕϊ�
	Vector2 RConvert(D3DXVECTOR2* v);
	//Vector3�ɕϊ�
	Vector3 RConvert(D3DXVECTOR3* v);
	//Matrix4�ɕϊ�
	Matrix4 RConvert(D3DXMATRIX* m);
	//Quaternion�ɕϊ�
	Quaternion RConvert(D3DXQUATERNION* q);
