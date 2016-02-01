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
	// ���_��`
	struct VERTEX
	{
		// ���_���W
		D3DXVECTOR3   pos;
		// �@��
		D3DXVECTOR3   normal;
		// �e�N�Z��
		D3DXVECTOR2 texel;

		UINT boneIndex[4];
		UINT weight[4];
	};
	// ���_�o�b�t�@
	std::vector<ID3D11Buffer*> g_pVertexBufferList;
	// �C���f�b�N�X�o�b�t�@
	std::vector<ID3D11Buffer*> g_pIndexBufferList;

	std::vector<int> indexSize;

	std::vector<ID3D11ShaderResourceView*> tex;

	int meshCount;

	MeshUser m;
	AnimUser ani;
	LoadTexture lt;
	RgrLoader rL;
};