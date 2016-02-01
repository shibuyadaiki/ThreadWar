#include "LoadTexture.h"
#include  "../../Other/Device.h"

LoadTexture::LoadTexture(){

}
LoadTexture::~LoadTexture(){
}
ID3D11ShaderResourceView* LoadTexture::LoadBindTexture(const char* texName){
	D3DX11_IMAGE_LOAD_INFO info;
	ID3D11ShaderResourceView* texture;
	// �t�@�C������V�F�[�_�[���\�[�X�r���[���쐬����
	::ZeroMemory(&info, sizeof(D3DX11_IMAGE_LOAD_INFO));
	info.Width = D3DX11_DEFAULT;
	info.Height = D3DX11_DEFAULT;
	info.Depth = D3DX11_DEFAULT;
	info.FirstMipLevel = D3DX11_DEFAULT;          // �e�N�X�`���[�̍ō��𑜓x�̃~�b�v�}�b�v ���x���B���ۂ̎g�p���@�s��
	info.MipLevels = 0;                           // �~�b�v�}�b�v���B0 �܂��� D3DX11_DEFAULT ���g�p����Ƃ��ׂẴ~�b�v�}�b�v �`�F�[�����쐬����
	info.Usage = D3D11_USAGE_DEFAULT;
	info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	info.CpuAccessFlags = 0;
	info.MiscFlags = 0;
	info.Format = DXGI_FORMAT_FROM_FILE;
	info.Filter = D3DX11_FILTER_POINT;            // �e�N�X�`���[�ǂݍ��ݎ��Ɏg�p����t�B���^�[
	info.MipFilter = D3DX11_FILTER_POINT;         // �~�b�v�}�b�v�쐬���Ɏg�p����t�B���^�[
	info.pSrcInfo = NULL;

	TCHAR	wstr[512];
	size_t wLen = 0;
	const char* texName_;
	if (texName != NULL){
		texName_ = texName;
		std::string s = texName;
		int a = s.size();
	}
	else{
		texName_ = "Shader/white.bmp";
	}
	mbstowcs_s(&wLen, wstr, strlen(texName_) + 1, texName_, _TRUNCATE);
	Device::GetInstance().Getd3d11User()->CreateTexture(wstr, &texture, info);
	return texture;
}