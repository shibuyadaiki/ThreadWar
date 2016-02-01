#ifndef D3D11USER_H
#define D3D11USER_H
#define D3D11USER_FORMAT        DXGI_FORMAT_R8G8B8A8_UNORM
#include <dsound.h>

// �[�x�X�e���V���r���[�ƃV�F�[�_�[���\�[�X�r���[�̂ǂ���Ƃ��Ă��g�p�ł���悤�ɂ��邽�ߌ^���w�肵�Ȃ�
// �t�H�[�}�b�g�ɂ��Ă�DXGI_FORMAT�Q��
//#define D3D11USER_DEPTH_FORMAT  DXGI_FORMAT_R8_TYPELESS  // Direct3D11�̃T���v���ł͐ݒ肳��Ă��邪�g���Ă݂�ƃT�|�[�g���ĂȂ����ăG���[�ɂȂ�
#define D3D11USER_DEPTH_FORMAT  DXGI_FORMAT_R16_TYPELESS
//#define D3D11USER_DEPTH_FORMAT  DXGI_FORMAT_R32_TYPELESS
//#define D3D11USER_DEPTH_FORMAT  DXGI_FORMAT_R24G8_TYPELESS

#define D3D11USER_F_WINDOWSTYLE WS_POPUP
#define D3D11USER_W_WINDOWSTYLE WS_BORDER | WS_CAPTION | WS_SYSMENU

//#define WINDOWSIZE_WIDTH 1920
//#define WINDOWSIZE_HEIGHT 1080
//
//#define WINDOWSIZE_WIDTH 1600
//#define WINDOWSIZE_HEIGHT 900

class D3D11USER
{
public:
	HWND                    m_hWnd;
	std::vector<DXGI_MODE_DESC> m_DisplayModeDesc;
	int displayModeSize;
private:
	D3D_FEATURE_LEVEL       m_FeatureLevel;            // Direct3D ��������A���ۂɍ̗p���ꂽ�t�B�[�`���[���x���B
	ID3D11Device*           m_D3DDevice;               // Direct3D11 �f�o�C�X�B
	ID3D11DeviceContext*    m_D3DDeviceContext;        // Direct3D11 �f�o�C�X�R���e�L�X�g�B
	IDXGIAdapter*           m_Adapter;                 // �A�_�v�^�[
	IDXGISwapChain*         m_SwapChain;               // �X���b�v�`�F�[��
	ID3D11RenderTargetView* m_RenderTargetView;        // �����_�����O�^�[�Q�b�g�r���[
	ID3D11DepthStencilView* m_DepthStencilView;
	BOOL m_UseDepthBuffer;
	bool kasouFullScreen;

	int WINDOWSIZE_WIDTH = 0;
	int WINDOWSIZE_HEIGHT = 0;


private:
	void OutputMsg(TCHAR* pMsg1, TCHAR* pMsg2, TCHAR* pMsg3);

	// �E�B���h�E�쐬
	HRESULT InitWindow(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode);

	// �A�_�v�^�[���擾
	HRESULT CreateAdapter();

	// �f�o�C�X�̍쐬
	HRESULT CreateDevice(BOOL DirectX11Only);

	// �X���b�v�`�F�[���̍쐬
	HRESULT CreateSwapChain(DXGI_MODE_DESC* pDisplayMode, BOOL MultisampleEnabled);

	// �����_�����O�^�[�Q�b�g�r���[�̍쐬
	HRESULT CreateRenderTargetView();

	// �[�x�X�e���V���r���[�̍쐬
	HRESULT CreateDepthStencilView();

	// �r���[�|�[�g�̍쐬
	HRESULT CreateViewport();

	// �E�B���h�E�A�\�V�G�[�V����
	HRESULT SetWindowAssociation();

	// �f�t�H���g�̃��X�^���C�U��ݒ�
	HRESULT SetDefaultRasterize();

	// �f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�
	HRESULT SetDefaultDepthStencilState();

	// ���_�A�C���f�b�N�X�A�萔�o�b�t�@���쐬����
	HRESULT CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag);

public:
	D3D11USER();
	~D3D11USER();
	// �������J��
	void Invalidate();

	// �f�B�X�v���C���[�h�ꗗ���쐬
	HRESULT GetDisplayMode();

	// Direct3D�̏�����
	HRESULT InitD3D11(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode,
		BOOL Windowed, BOOL DirectX11Only, BOOL MultisampleEnabled, BOOL UseDepthBuffer);

	// �\�����[�h�̐؂�ւ����œK��
	HRESULT ChangeWindowModeOptimization(UINT Width, UINT Height);
	// �\�����[�h��؂�ւ���
	HRESULT ChangeWindowMode();

	// ���_�o�b�t�@���쐬����
	HRESULT CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);
	// �C���f�b�N�X�o�b�t�@���쐬����
	HRESULT CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);

	// �萔�o�b�t�@���쐬����
	HRESULT CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);

	// �e�N�X�`���[�t�@�C������V�F�[�_�[���\�[�X�r���[�z����쐬����
	HRESULT CreateShaderResourceViewArray(TCHAR* pTextureFileNames[], UINT NumTextures, ID3D11ShaderResourceView** pSRVView);

	// ���_�V�F�[�_�[���t�@�C������쐬����
	HRESULT CreateVertexShaderFromFile(OUT ID3D11VertexShader** pVertexShader,
		IN TCHAR pSrcFile[], IN CHAR pFunctionName[], CHAR pProfile[],
		OUT ID3D11InputLayout** g_pLayout,
		IN D3D11_INPUT_ELEMENT_DESC pInputElementDescs[], UINT NumElements);
	// ���_�V�F�[�_�[������������쐬����
	HRESULT CreateVertexShaderFromMemory(OUT ID3D11VertexShader** pVertexShader,
		IN const BYTE* pShader, size_t size,
		OUT ID3D11InputLayout** g_pLayout,
		IN D3D11_INPUT_ELEMENT_DESC pInputElementDescs[], UINT NumElements);

	// �n���V�F�[�_�[���t�@�C������쐬����
	HRESULT CreateHullShaderFromFile(ID3D11HullShader** pHullShader, TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[]);
	// �n���V�F�[�_�[������������쐬����
	HRESULT CreateHullShaderFromMemory(ID3D11HullShader** pHullShader, const BYTE* pShader, size_t size);

	// �h���C���V�F�[�_�[���t�@�C������쐬����
	HRESULT CreateDomainShaderFromFile(ID3D11DomainShader** pDomainShader, TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[]);
	// �h���C���V�F�[�_�[������������쐬����
	HRESULT CreateDomainShaderFromMemory(ID3D11DomainShader** pDomainShader, const BYTE* pShader, size_t size);

	// �W�I���g���V�F�[�_�[���t�@�C������쐬����
	HRESULT CreateGeometryShaderFromFile(ID3D11GeometryShader** pGeometryShader, TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[]);
	// �W�I���g���V�F�[�_�[������������쐬����
	HRESULT CreateGeometryShaderFromMemory(ID3D11GeometryShader** pGeometryShader, const BYTE* pShader, size_t size);

	// �X�g���[�~���O�o�̓o�b�t�@�[�ւ̏������݂��\�ȃW�I���g�� �V�F�[�_�[���t�@�C������쐬����
	HRESULT CreateGeometryShaderWithStreamOutputFromFile(
		ID3D11GeometryShader** pGeometryShader,
		TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[],
		const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
		UINT NumEntries,
		const UINT *pBufferStrides,
		UINT NumStrides);
	// �X�g���[�~���O�o�̓o�b�t�@�[�ւ̏������݂��\�ȃW�I���g�� �V�F�[�_�[������������쐬����
	HRESULT CreateGeometryShaderWithStreamOutputFromMemory(
		ID3D11GeometryShader** pGeometryShader,
		const BYTE* pShader, size_t size,
		const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
		UINT NumEntries,
		const UINT *pBufferStrides,
		UINT NumStrides);

	// �s�N�Z���V�F�[�_�[���t�@�C������쐬����
	HRESULT CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader, TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[]);
	// �s�N�Z���V�F�[�_�[������������쐬����
	HRESULT CreatePixelShaderFromMemory(ID3D11PixelShader** pPixelShader, const BYTE* pShader, size_t size);

	// �X�N���[���V���b�g�쐬
	HRESULT CreateScreenShot();

	// �u�����h �X�e�[�g�𖳌��ɂ��邽�߂̐ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetDefaultBlendDesc();
	// ���`�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetAlignmentBlendDesc();
	// ���Z�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetAddBlendDesc();
	// ���Z�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetSubtractBlendDesc();
	// �ώZ�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
	D3D11_RENDER_TARGET_BLEND_DESC GetMultipleBlendDesc();
	// �u�����h�X�e�[�g��ݒ肷��
	HRESULT SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC BlendStateArray[], UINT NumBlendState, BOOL AlphaToCoverageEnable);

	// �T���v���[�X�e�[�g���쐬����
	HRESULT CreateSamplerState(ID3D11SamplerState** ppSamplerState, D3D11_FILTER Filter, UINT MaxAnisotropy);

	// �[�x�X�e���V���X�e�[�g�̒��Őݒ�ύX�p�x���������ȃp�����[�^��ݒ肷��
	HRESULT SetDepthStencilState(BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteEnabled);

	// �[�x�X�e���V���r���[����V�F�[�_�[���\�[�X�r���[���擾
	ID3D11ShaderResourceView* GetSRViewFromDepthStencil();

	// �V�F�[�_�[�Ƀo�C���h�\�ȃ����_�[�^�[�Q�b�g�r���[���쐬����
	HRESULT CreateRenderTargetView(ID3D11RenderTargetView** ppRTView, D3DXVECTOR2* pRTViewSize, DXGI_FORMAT Format);

	// �V�F�[�_�[�Ƀo�C���h�\�ȃ����_�[�^�[�Q�b�g�r���[���쐬����
	HRESULT CreateRenderTargetView(ID3D11RenderTargetView** ppRTView, D3DXVECTOR2* pRTViewSize, DXGI_FORMAT Format, UINT MipLevels);

	// �����_�[�^�[�Q�b�g�r���[����V�F�[�_�[���\�[�X�r���[���擾����
	ID3D11ShaderResourceView* GetSRViewFromRTView(ID3D11RenderTargetView* pRTView);

	//�쐬
	// �o�C�i�� �G�t�F�N�g�܂��̓o�C�i�� �t�@�C������G�t�F�N�g���쐬
	HRESULT CreateEffectFromMemory(unsigned char* cso_data, long cso_sz, ID3DX11Effect** g_pEffect);

	//�[�x�X�e���V���X�e�[�g�쐬
	HRESULT CreateDepthStencilState(ID3D11DepthStencilState** depthStencilState, bool depthEnable = TRUE);

	//�T���v���[�擾
	HRESULT GetSampler(LPCSTR samplerName, ID3DX11Effect* effect, ID3DX11EffectSamplerVariable** sampler);

	//�T���v���[�X�e�[�g�쐬
	HRESULT CreateSamplerDesc(ID3DX11EffectSamplerVariable* sampler);
	
	//�o�C�i������G�t�F�N�g��ǂݍ���
	int ReadBinary(TCHAR* csoName, long* cso_sz, unsigned char** cso_data);

	//�C���v�b�g���C�A�E�g�쐬
	HRESULT CreateInputLayout(ID3DX11EffectPass* effectPass,D3D11_INPUT_ELEMENT_DESC layout[], UINT layoutCount, ID3D11InputLayout** pLayout);

	
	//�Z�b�g
	//�C���v�b�g���C�A�E�g���Z�b�g
	void SetInputLayout(ID3D11InputLayout* layout);

	//�[�x�e�X�g��L���ɂ���
	void SetDepthStencilState(ID3D11DepthStencilState* depthStencilState);

	// �p�X�Ɋ܂܂�Ă���X�e�[�g���f�o�C�X�ɐݒ�
	HRESULT ShaderApply(ID3DX11EffectPass* effectPass);

	// �o�b�N�o�b�t�@���N���A
	void ClearRenderTargetView(float ClearColor[]);

	//�[�x�o�b�t�@���N���A
	void ClearDepthStencilView();

	//���_�o�b�t�@�ݒ�
	void SetVertexBuffer(ID3D11Buffer** vertexBuffer, UINT* stride, UINT* offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	void SetIndexBuffer(ID3D11Buffer* indexBuffer);

	//texture����
	void CreateTexture(TCHAR* texName, ID3D11ShaderResourceView** texture, D3DX11_IMAGE_LOAD_INFO info);

	//�A���t�@�u�����f�B���O�ݒ�
	void SetAlphaBlend( bool alphaEnable);

	//�`��
	void DrawIndexed(UINT indexCount, UINT StartIndexLocation, INT BaseVertexLocation);

	//�����_�����O���ꂽ�C���[�W�����[�U�[�ɕ\��
	void Present();

	//�g�|���W�[�̃Z�b�g
	void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);

	//TCHAR�ɕς��Ȃ��Ɠ��{��Ή��ł��Ȃ��S�~
	void SetMyWindowText(HWND hwnd,std::string text);

	// �r���[�|�[�g�̕ύX
	HRESULT D3D11USER::ChangeViewport(float xStart, float xEnd, float yStart, float yEnd);
};

#endif