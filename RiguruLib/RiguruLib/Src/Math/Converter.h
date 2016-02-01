#pragma once
#include "../Other/DX11User.h"
#include "MathUtility.h"
#include "MatrixUtility.h"
#include "Vector2Utility.h"
#include "Vector3Utility.h"
#include "Quaternion.h"

	//D3DXVECTOR2‚É•ÏŠ·
D3DXVECTOR2 RConvert(Vector2* v);
	//D3DXVECTOR3‚É•ÏŠ·
	D3DXVECTOR3 RConvert(Vector3* v);
	//D3DXMATRIX‚É•ÏŠ·
	D3DXMATRIX RConvert(Matrix4* m);
	//D3DXQUATERNION‚É•ÏŠ·
	D3DXQUATERNION RConvert(Quaternion* q);

	//Vector2‚É•ÏŠ·
	Vector2 RConvert(D3DXVECTOR2* v);
	//Vector3‚É•ÏŠ·
	Vector3 RConvert(D3DXVECTOR3* v);
	//Matrix4‚É•ÏŠ·
	Matrix4 RConvert(D3DXMATRIX* m);
	//Quaternion‚É•ÏŠ·
	Quaternion RConvert(D3DXQUATERNION* q);
