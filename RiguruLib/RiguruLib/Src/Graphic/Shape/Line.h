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
	// �C���f�b�N�X�o�b�t�@
	std::vector<ID3D11Buffer*> g_pIndexBufferList, g_pVertexBufferList;

	std::vector<int> indexSize;
	LoadTexture lt;
};