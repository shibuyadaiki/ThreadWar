#include "Shader.h"
#include "../../Other/Device.h"

Shader::Shader(){
}

Shader::~Shader(){

}

void Shader::Release(){
	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pDepthStencilState);
	SAFE_RELEASE(g_pLayout);
}

HRESULT Shader::CreateShader(const char* csoName, bool depthEnable){
	HRESULT hr = E_FAIL;
	unsigned char* cso_data = nullptr;

	long cso_sz;
	WCHAR	wstr[512];
	size_t wLen = 0;
	mbstowcs_s(&wLen, wstr, strlen(csoName) + 1, csoName, _TRUNCATE);

	//バイナリからエフェクトを読み込む
	Device::GetInstance().Getd3d11User()->ReadBinary(wstr, &cso_sz, &cso_data);
	
	//エフェクトを作成
	hr = Device::GetInstance().Getd3d11User()->CreateEffectFromMemory(cso_data, cso_sz, &g_pEffect);
	if (FAILED(hr)) goto EXIT;

	//深度ステンシルシート作成
	hr = Device::GetInstance().Getd3d11User()->CreateDepthStencilState(&g_pDepthStencilState,depthEnable);
	if (FAILED(hr)) goto EXIT;

	//サンプラーを取得
	hr = Device::GetInstance().Getd3d11User()->GetSampler("g_Sampler", g_pEffect, &g_pSampler);
	if (FAILED(hr)) goto EXIT;

	//サンプラーステート作成
	hr = Device::GetInstance().Getd3d11User()->CreateSamplerDesc(g_pSampler);
	if (FAILED(hr)) goto EXIT;

	SAFE_DELETE(cso_data);
	return hr;
EXIT:
	SAFE_DELETE(cso_data);
	return hr;
}
void Shader::SetShader(){
	// レイアウト設定
	Device::GetInstance().Getd3d11User()->SetInputLayout(g_pLayout);

	// 深度テストを有効にする
	Device::GetInstance().Getd3d11User()->SetDepthStencilState(g_pDepthStencilState);
}

HRESULT Shader::ShaderApply(){
	// パスに含まれているステートをデバイスに設定
	// ID3DX11EffectPass::Apply
	return Device::GetInstance().Getd3d11User()->ShaderApply(g_pEffectPass);
}

HRESULT Shader::SetMatrix(LPCSTR matrixName, D3DXMATRIX* matrix,int size){
	return g_pEffect->GetVariableByName(matrixName)->AsMatrix()->SetMatrixArray((float*)matrix,0,size);
}

HRESULT Shader::SetTexture(LPCSTR textureName, ID3D11ShaderResourceView* texture){
	return g_pEffect->GetVariableByName(textureName)->AsShaderResource()->SetResource(texture);
}

HRESULT Shader::SetVector4(LPCSTR vector4Name, D3DXVECTOR4* vector4){
	return g_pEffect->GetVariableByName(vector4Name)->AsVector()->SetFloatVector((float*)vector4);
}

HRESULT Shader::SetColor(LPCSTR vector4Name, D3DXCOLOR* vector4){
	return g_pEffect->GetVariableByName(vector4Name)->AsVector()->SetFloatVector((float*)vector4);
}


HRESULT Shader::SetTechniquePass(LPCSTR techniqueName, LPCSTR passName){
	HRESULT hr = S_FALSE;
	// 名前によってテクニックを取得
	// ID3DX11Effect::GetTechniqueByName
	g_pEffectTechnique = g_pEffect->GetTechniqueByName(techniqueName);
	if (g_pEffectTechnique->IsValid() == FALSE){ hr = E_FAIL; goto EXIT; }

	// 取得したテクニックから名前によってパスを取得
	// ID3DX11EffectTechnique::GetPassByName
	g_pEffectPass = g_pEffectTechnique->GetPassByName(passName);
	if (g_pEffectPass->IsValid() == FALSE){ hr = E_FAIL; goto EXIT; }
EXIT:
	return hr;
}

HRESULT Shader::CreateLayout(){
	//レイアウトを作成する
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	return Device::GetInstance().Getd3d11User()->CreateInputLayout(g_pEffectPass, layout, _countof(layout), &g_pLayout);
}