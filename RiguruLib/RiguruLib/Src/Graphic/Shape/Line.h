#pragma once
#include "../../Other/DX11User.h"
#include  "../../Other/D3D11User.h"
#include "../Shader/Shader.h"
#include "../Shader/LoadTexture.h"


class Line{
public:
	Line();
	~Line();
	void Release();
	HRESULT Load(); 
	void Draw(Shader* shader, const Vector3& startPos, const Vector3& endPos, CAMERA_ID cID, D3DXCOLOR& color);
private:
	// 頂点定義
	struct VERTEX
	{
		// 頂点座標
		D3DXVECTOR3   pos;
		// 法線
		D3DXVECTOR3   normal;
		// テクセル
		D3DXVECTOR2 texel;

		UINT boneIndex[4];
		UINT weight[4];
	};
	// インデックスバッファ
	std::vector<ID3D11Buffer*> g_pIndexBufferList, g_pVertexBufferList;

	std::vector<int> indexSize;
	LoadTexture lt;
};