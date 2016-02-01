#pragma once
#include "../../Other/DX11User.h"
#include  "../../Other/D3D11User.h"
#include "../Shader/Shader.h"
#include "../Rgr/MeshUser.h"
#include  "../Rgr/RgrLoader.h"
#include "../Shader/LoadTexture.h"
#include <string>
#include "../../Math/Vector2.h"
#pragma warning(disable:4996)


class Font{
public:
	Font();
	~Font();
	void Release();
	HRESULT Load(const char* modelName);
	void Draw(Vector2 pos, Vector2 size, float range, Shader* shader, std::string string, D3DXCOLOR* color,bool center);
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

	std::map<char, D3DXVECTOR2> fontVec;
	// ���_�o�b�t�@
	ID3D11Buffer* VertexBuffer = NULL;
	// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer* IndexBuffer = NULL;
};