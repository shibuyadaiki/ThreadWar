#pragma once
#include "../../Other/DX11User.h"
#include  "../../Other/D3D11User.h"
class Shader{
public:

	Shader();
	~Shader();
	void Release();
	HRESULT CreateShader(const char* csoName,bool depthEnable = TRUE);
	void SetShader();

	HRESULT ShaderApply();

	HRESULT SetTechniquePass(LPCSTR techniqueName, LPCSTR passName);

	HRESULT CreateLayout();

	HRESULT SetMatrix(LPCSTR matrixName, D3DXMATRIX* matrix,int size = 1);

	HRESULT SetTexture(LPCSTR textureName, ID3D11ShaderResourceView* texture);

	HRESULT SetVector4(LPCSTR vector4Name, D3DXVECTOR4* vector4);

	HRESULT SetColor(LPCSTR vector4Name, D3DXCOLOR* vector4);

private:
	// �����_�����O �G�t�F�N�g
	// ID3DX11Effect
	ID3DX11Effect* g_pEffect = NULL;

	// �p�X�̏W��
	// ID3DX11EffectTechnique
	ID3DX11EffectTechnique* g_pEffectTechnique = NULL;

	//// �[�x�X�e���V���X�e�[�g
	ID3D11DepthStencilState*  g_pDepthStencilState = NULL;

	// �p�X
	// ID3DX11EffectPass
	ID3DX11EffectPass* g_pEffectPass = NULL;

	// �T���v���[�X�e�[�g
	// ID3DX11EffectSamplerVariable
	ID3DX11EffectSamplerVariable* g_pSampler = NULL;

	// ���̓��C�A�E�g
	ID3D11InputLayout* g_pLayout = NULL;


};