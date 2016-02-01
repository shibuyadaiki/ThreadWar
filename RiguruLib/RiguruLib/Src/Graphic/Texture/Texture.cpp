#include "Texture.h"
#include "../../Other/Device.h"
#include <iostream>
#include <fstream>
#include "../Rgr/RgrLoader.h"
#include "../../Math/MathUtility.h"
#include "../../Math/Converter.h"
using namespace std;


Texture::Texture(){
}

Texture::~Texture(){
}

void Texture::Release(){
	SAFE_RELEASE(g_pIndexBufferList[0]);
	SAFE_RELEASE(g_pVertexBufferList[0]);
	SAFE_RELEASE(tex[0]);
}

HRESULT Texture::Load(const char* modelName){
	HRESULT hr = E_FAIL;
	std::vector<char> fileNameVec;
	char slash = '\\';

	ID3D11ShaderResourceView* texture = lt.LoadBindTexture(modelName);
	tex.push_back(texture);
	ID3D11Resource* res;
	texture->GetResource(&res);
	ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(res);
	D3D11_TEXTURE2D_DESC dec;
	tex->GetDesc(&dec);
	w = dec.Width;
	h = dec.Height;
	res->Release();

	std::vector<VERTEX> vertexVec;
	UINT* index = new UINT[6];

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 4;
	index[5] = 5;

	vertexVec.push_back({
		(D3DXVECTOR3(1.0f, 1.0f, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(1, 1)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(0, 1.0f, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(0, 1)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(1.0f, 0, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(1, 0)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(0, 1.0f, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(0, 1)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(0, 0)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(1.0f, 0, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(1, 0)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});

	int vertexSize = vertexVec.size();
	// 頂点の実データを設定
	VERTEX* v = new VERTEX[vertexSize];
	for (int i = 0; i < vertexSize; i++)
		v[i] = vertexVec[i];

	// 頂点バッファ
	ID3D11Buffer* VertexBuffer = NULL;
	// インデックスバッファ
	ID3D11Buffer* IndexBuffer = NULL;

	// インデックスバッファを作成する。
	hr = Device::GetInstance().Getd3d11User()->CreateIndexBuffer(&IndexBuffer, index, sizeof(UINT)* 6, 0);
	g_pIndexBufferList.push_back(IndexBuffer);
	if (FAILED(hr)) goto EXIT;
	delete(index);

	// 頂点バッファを作成する
	hr = Device::GetInstance().Getd3d11User()->CreateVertexBuffer(&VertexBuffer, v, sizeof(VERTEX)* vertexSize, 0);
	g_pVertexBufferList.push_back(VertexBuffer);
	if (FAILED(hr)) goto EXIT;
	delete(v);

	return S_OK;
EXIT:
	return hr;
}

void Texture::Draw(Vector3 pos, Vector3 size, Vector3 center, float left, float top, float right, float bottom, Shader* shader, D3DXCOLOR color, float rotate){
	// 行列をエフェクトに設定
	// 頂点バッファ設定
	//Device::GetInstance().Getd3d11User()->SetBlendState(&Device::GetInstance().Getd3d11User()->GetAlignmentBlendDesc(), 1, FALSE);
	Device::GetInstance().Getd3d11User()->SetAlphaBlend(TRUE);
	Device::GetInstance().Getd3d11User()->SetTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	float wi = 1920;
	float he = 1080;///2;
	D3DXVECTOR4 rect = { left, top, right, bottom };
	Vector3 centerPos;
	Vector3 leftUp, leftDown, rightUp, rightDown;
	Vector3 pos_ = pos - vector3(1.0f, 1.0f, 0.0f);
	leftUp = vector3(pos_.x + (w * size.x) * left, pos_.y + (h * size.y) * top, 0) - vector3(w * size.x * center.x, h * size.y * center.y, 0);
	leftDown = vector3(pos_.x + (w * size.x) * left, pos_.y + (h * size.y) * bottom, 0) - vector3(w * size.x * center.x, h * size.y * center.y, 0);
	rightUp = vector3(pos_.x + (w * size.x) * right, pos_.y + (h * size.y) * top, 0) - vector3(w * size.x * center.x, h * size.y * center.y, 0);
	rightDown = vector3(pos_.x + (w * size.x) * right, pos_.y + (h * size.y) * bottom, 0) - vector3(w * size.x * center.x, h * size.y * center.y, 0);
	centerPos = leftUp + vector3(w * size.x, h * size.y, 0) / 2.0f;
	leftUp = RCMatrix4::getPosition(RCMatrix4::translate(leftUp - centerPos) * RCMatrix4::rotateZ(rotate) * RCMatrix4::translate(centerPos));
	leftDown = RCMatrix4::getPosition(RCMatrix4::translate(leftDown - centerPos) * RCMatrix4::rotateZ(rotate) * RCMatrix4::translate(centerPos));
	rightUp = RCMatrix4::getPosition(RCMatrix4::translate(rightUp - centerPos) * RCMatrix4::rotateZ(rotate) * RCMatrix4::translate(centerPos));
	rightDown = RCMatrix4::getPosition(RCMatrix4::translate(rightDown - centerPos) * RCMatrix4::rotateZ(rotate) * RCMatrix4::translate(centerPos));

	leftUp = vector3((leftUp.x - wi / 2.0f) / (wi / 2.0f), (leftUp.y - he / 2.0f) / (he / 2.0f), 0);
	leftDown = vector3((leftDown.x - wi / 2.0f) / (wi / 2.0f), (leftDown.y - he / 2.0f) / (he / 2.0f), 0);
	rightUp = vector3((rightUp.x - wi / 2.0f) / (wi / 2.0f), (rightUp.y - he / 2.0f) / (he / 2.0f), 0);
	rightDown = vector3((rightDown.x - wi / 2.0f) / (wi / 2.0f), (rightDown.y - he / 2.0f) / (he / 2.0f), 0);

	shader->SetVector4("g_PosLeftUp", &D3DXVECTOR4(leftUp.x, leftUp.y, 0, 1));
	shader->SetVector4("g_PosLeftDown", &D3DXVECTOR4(leftDown.x, leftDown.y, 0, 1));
	shader->SetVector4("g_PosRightUp", &D3DXVECTOR4(rightUp.x, rightUp.y, 0, 1));
	shader->SetVector4("g_PosRightDown", &D3DXVECTOR4(rightDown.x, rightDown.y, 0, 1));

	shader->SetVector4("g_rect", &D3DXVECTOR4(rect.x, rect.y, rect.z - rect.x, rect.w - rect.y));
	shader->SetColor("materialColor", &color);
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Device::GetInstance().Getd3d11User()->SetVertexBuffer(&g_pVertexBufferList[0], &stride, &offset);

	Device::GetInstance().Getd3d11User()->SetIndexBuffer(g_pIndexBufferList[0]);
	// インデックスバッファ設定
	shader->SetTexture("g_Tex", tex[0]);
	// プリミティブ タイプおよびデータの順序に関する情報を設定
	//Zの時はSTRIP
	shader->ShaderApply();
	Device::GetInstance().Getd3d11User()->DrawIndexed(6, 0, 0);
	Device::GetInstance().Getd3d11User()->SetAlphaBlend(FALSE);
}