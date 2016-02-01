#pragma once
#include "../../Other/DX11User.h"
#include "../../Other/D3D11User.h"
#include "../Shader/Shader.h"
#include "MeshUser.h"
#include  "RgrLoader.h"
#include "../Animation/AnimUser.h"
#include "../Shader/LoadTexture.h"


class Mesh{
public:
	Mesh();
	~Mesh();
	void Release();
	HRESULT Load( const char* modelName);
	void Draw(D3DXMATRIX* matWorld, bool animFlag, Shader* shader, CAMERA_ID cID, D3DXCOLOR* color = NULL, bool alphaFlag = false);
	MeshUser* ReturnMeshUser();
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
	// 頂点バッファ
	std::vector<ID3D11Buffer*> g_pVertexBufferList;
	// インデックスバッファ
	std::vector<ID3D11Buffer*> g_pIndexBufferList;

	std::vector<int> indexSize;

	std::vector<ID3D11ShaderResourceView*> tex;

	int meshCount;

	MeshUser m;
	AnimUser ani;
	LoadTexture lt;
	RgrLoader rL;
};