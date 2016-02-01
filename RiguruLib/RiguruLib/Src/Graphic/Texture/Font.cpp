#include "Font.h"
#include "../../Other/Device.h"
#include <iostream>
#include <string>
#include <fstream>
#include "../Rgr/RgrLoader.h"
#include "../../Math/MathUtility.h"
#include "../../Math/Converter.h"
using namespace std;


Font::Font(){
	fontVec['0'] = D3DXVECTOR2(0, 0);
	fontVec['1'] = D3DXVECTOR2(1, 0);
	fontVec['2'] = D3DXVECTOR2(2, 0);
	fontVec['3'] = D3DXVECTOR2(3, 0);
	fontVec['4'] = D3DXVECTOR2(4, 0);
	fontVec['5'] = D3DXVECTOR2(5, 0);
	fontVec['6'] = D3DXVECTOR2(6, 0);
	fontVec['7'] = D3DXVECTOR2(7, 0);
	fontVec['8'] = D3DXVECTOR2(8, 0);
	fontVec['9'] = D3DXVECTOR2(9, 0);
	fontVec['A'] = D3DXVECTOR2(0, 1);
	fontVec['B'] = D3DXVECTOR2(1, 1);
	fontVec['C'] = D3DXVECTOR2(2, 1);
	fontVec['D'] = D3DXVECTOR2(3, 1);
	fontVec['E'] = D3DXVECTOR2(4, 1);
	fontVec['F'] = D3DXVECTOR2(5, 1);
	fontVec['G'] = D3DXVECTOR2(6, 1);
	fontVec['H'] = D3DXVECTOR2(7, 1);
	fontVec['I'] = D3DXVECTOR2(8, 1);
	fontVec['J'] = D3DXVECTOR2(9, 1);
	fontVec['K'] = D3DXVECTOR2(0, 2);
	fontVec['L'] = D3DXVECTOR2(1, 2);
	fontVec['M'] = D3DXVECTOR2(2, 2);
	fontVec['N'] = D3DXVECTOR2(3, 2);
	fontVec['O'] = D3DXVECTOR2(4, 2);
	fontVec['P'] = D3DXVECTOR2(5, 2);
	fontVec['Q'] = D3DXVECTOR2(6, 2);
	fontVec['R'] = D3DXVECTOR2(7, 2);
	fontVec['S'] = D3DXVECTOR2(8, 2);
	fontVec['T'] = D3DXVECTOR2(9, 2);
	fontVec['U'] = D3DXVECTOR2(0, 3);
	fontVec['V'] = D3DXVECTOR2(1, 3);
	fontVec['W'] = D3DXVECTOR2(2, 3);
	fontVec['X'] = D3DXVECTOR2(3, 3);
	fontVec['Y'] = D3DXVECTOR2(4, 3);
	fontVec['Z'] = D3DXVECTOR2(5, 3);
	fontVec['a'] = D3DXVECTOR2(6, 3);
	fontVec['b'] = D3DXVECTOR2(7, 3);
	fontVec['c'] = D3DXVECTOR2(8, 3);
	fontVec['d'] = D3DXVECTOR2(9, 3);
	fontVec['e'] = D3DXVECTOR2(0, 4);
	fontVec['f'] = D3DXVECTOR2(1, 4);
	fontVec['g'] = D3DXVECTOR2(2, 4);
	fontVec['h'] = D3DXVECTOR2(3, 4);
	fontVec['i'] = D3DXVECTOR2(4, 4);
	fontVec['j'] = D3DXVECTOR2(5, 4);
	fontVec['k'] = D3DXVECTOR2(6, 4);
	fontVec['l'] = D3DXVECTOR2(7, 4);
	fontVec['m'] = D3DXVECTOR2(8, 4);
	fontVec['n'] = D3DXVECTOR2(9, 4);
	fontVec['o'] = D3DXVECTOR2(0, 5);
	fontVec['p'] = D3DXVECTOR2(1, 5);
	fontVec['q'] = D3DXVECTOR2(2, 5);
	fontVec['r'] = D3DXVECTOR2(3, 5);
	fontVec['s'] = D3DXVECTOR2(4, 5);
	fontVec['t'] = D3DXVECTOR2(5, 5);
	fontVec['u'] = D3DXVECTOR2(6, 5);
	fontVec['v'] = D3DXVECTOR2(7, 5);
	fontVec['w'] = D3DXVECTOR2(8, 5);
	fontVec['x'] = D3DXVECTOR2(9, 5);
	fontVec['y'] = D3DXVECTOR2(0, 6);
	fontVec['z'] = D3DXVECTOR2(1, 6);
	fontVec[':'] = D3DXVECTOR2(2, 6);
	fontVec['%'] = D3DXVECTOR2(3, 6);
	fontVec['+'] = D3DXVECTOR2(4, 6);
	fontVec['-'] = D3DXVECTOR2(5, 6);
	fontVec['*'] = D3DXVECTOR2(6, 6);
	fontVec['/'] = D3DXVECTOR2(7, 6);
	fontVec['='] = D3DXVECTOR2(8, 6);
	fontVec['.'] = D3DXVECTOR2(9, 6);
	fontVec[' '] = D3DXVECTOR2(0, 7);

}

Font::~Font(){
}

void Font::Release(){
	SAFE_RELEASE(g_pIndexBufferList[0]);
	SAFE_RELEASE(g_pVertexBufferList[0]);
	SAFE_RELEASE(tex[0]);
}

HRESULT Font::Load(const char* modelName){
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
	w = (float)dec.Width;
	h = (float)dec.Height;
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
		(D3DXVECTOR3(1, 1, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(1, 1)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(0, 1, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(0, 1)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(1, 0, 0)),
		(D3DXVECTOR3(0, 0, 0)),
		(D3DXVECTOR2(1, 0)),
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	});
	vertexVec.push_back({
		(D3DXVECTOR3(0, 1, 0)),
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
		(D3DXVECTOR3(1, 0, 0)),
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
#include "../../Graphic/Graphic.h"
void Font::Draw(Vector2 pos, Vector2 size, float range, Shader* shader, std::string s, D3DXCOLOR* color, bool center){

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	//Device::GetInstance().Getd3d11User()->SetBlendState(&Device::GetInstance().Getd3d11User()->GetAlignmentBlendDesc(), 1, FALSE);
	Device::GetInstance().Getd3d11User()->SetAlphaBlend(TRUE);
	Device::GetInstance().Getd3d11User()->SetTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	float assa[4] = { 0.3f, 0.75f, 0.3f, 0 };

	Device::GetInstance().Getd3d11User()->SetVertexBuffer(&g_pVertexBufferList[0], &stride, &offset);

	Device::GetInstance().Getd3d11User()->SetIndexBuffer(g_pIndexBufferList[0]);
	// インデックスバッファ設定
	shader->SetTexture("g_Tex", tex[0]);
	shader->SetColor("materialColor", color);
	float witdh = 1920;
	float height = 1080;
	Vector3 truePos = vector3(pos.x * 2 / witdh - 1.0f, pos.y * 2 / height - 1.0f, 0);
	Vector3 trueSize = vector3(size.x * 2.0f / 10.0f, size.y * 2.0f / 10.0f, 0);

	if (center){
		Vector3 startPos = truePos;
		Vector3 endPos = truePos;
		for (int i = 0; i < s.size(); i++){
			endPos = endPos + vector3(trueSize.x * range, 0, 0);
		}
		endPos = endPos + vector3(trueSize.x * range,truePos.y + trueSize.y, 0);
		
		Vector3 centerPos = startPos + endPos / 2.0f;

		truePos = truePos - centerPos;
	}

	for (int i = 0; i < s.size(); i++){
		shader->SetVector4("g_Pos", &D3DXVECTOR4(truePos.x, truePos.y, 0, 1));
		shader->SetVector4("g_Size", &D3DXVECTOR4(trueSize.x, trueSize.y, 0, 1));
		D3DXVECTOR2 select = fontVec[s[i]];
		shader->SetVector4("texSizeFontSizeFontSelect", &D3DXVECTOR4(w, w / 10.0f, select.x, select.y));
		shader->ShaderApply();
		Device::GetInstance().Getd3d11User()->DrawIndexed(6, 0, 0);
		truePos = truePos + vector3(trueSize.x * range, 0, 0);
	}
}