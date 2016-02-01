#pragma once
#include "../../Other/DX11User.h"
#include  "../../Other/D3D11User.h"
#include "../Shader/Shader.h"
#include "../Rgr/MeshUser.h"
#include  "../Rgr/RgrLoader.h"
#include "../Shader/LoadTexture.h"
class Texture{
public:
	Texture();
	~Texture();
	void Release();
	HRESULT Load(const char* modelName);
	void Draw(Vector3 pos, Vector3 size, Vector3 center, float left, float top, float right, float bottom, Shader* shader, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1), float angle = 0.0f);
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
	LoadTexture lt;
	std::vector<ID3D11ShaderResourceView*> tex;
	int meshCount;
	float w, h;
};