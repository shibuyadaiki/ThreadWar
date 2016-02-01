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
	// レンダリング エフェクト
	// ID3DX11Effect
	ID3DX11Effect* g_pEffect = NULL;

	// パスの集合
	// ID3DX11EffectTechnique
	ID3DX11EffectTechnique* g_pEffectTechnique = NULL;

	//// 深度ステンシルステート
	ID3D11DepthStencilState*  g_pDepthStencilState = NULL;

	// パス
	// ID3DX11EffectPass
	ID3DX11EffectPass* g_pEffectPass = NULL;

	// サンプラーステート
	// ID3DX11EffectSamplerVariable
	ID3DX11EffectSamplerVariable* g_pSampler = NULL;

	// 入力レイアウト
	ID3D11InputLayout* g_pLayout = NULL;


};