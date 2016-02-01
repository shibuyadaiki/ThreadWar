#include "DX11User.h"
#include "D3D11User.h"
#include < locale.h >



// �f�o�b�N�E�B���h�E�ɏo��
void D3D11USER::OutputMsg(TCHAR* pMsg1, TCHAR* pMsg2, TCHAR* pMsg3)
{
#if defined(DEBUG) || defined(_DEBUG)
	TCHAR s[256];
	_stprintf_s(s, _T("������ %s [ %s ] �� %s\n"), pMsg1, pMsg2, pMsg3);
	OutputDebugString(s);
#endif
}

D3D11USER::D3D11USER()
{
	m_hWnd = NULL;
	m_D3DDevice = NULL;
	m_D3DDeviceContext = NULL;
	m_Adapter = NULL;
	m_SwapChain = NULL;
	m_RenderTargetView = NULL;
	m_DepthStencilView = NULL;
	m_UseDepthBuffer = TRUE;
	kasouFullScreen = true;
}

D3D11USER::~D3D11USER()
{
}

// �������J��
void D3D11USER::Invalidate(){
	ULONG cnt;
	TCHAR s[256];

	m_DisplayModeDesc.clear();

	if (m_DepthStencilView)
	{
		cnt = m_DepthStencilView->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("%d"), cnt);
			OutputMsg(_T("ID3D11DepthStencilView�̎Q�ƃJ�E���^���s���ł��B"), s, _T("�m�f"));
		}
		m_DepthStencilView = NULL;
	}

	if (m_RenderTargetView)
	{
		cnt = m_RenderTargetView->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("%d"), cnt);
			OutputMsg(_T("ID3D11RenderTargetView�̎Q�ƃJ�E���^���s���ł��B"), s, _T("�m�f"));
		}
		m_RenderTargetView = NULL;
	}

	if (m_SwapChain)
	{
		// �X���b�v �`�F�[�����t���X�N���[�� ���[�h�ŉ���ł��Ȃ����Ƃ�����炵���B
		// �t���X�N���[���̏ꍇ�E�B���h�E���[�h�ɐ؂�ւ��Ă���J������K�v������炵���B
		// DXGI �̊T�v
		BOOL FullScreen;
		m_SwapChain->GetFullscreenState(&FullScreen, NULL);
		// �t���X�N���[���̂Ƃ�
		if (FullScreen == TRUE) m_SwapChain->SetFullscreenState(FALSE, NULL);

		cnt = m_SwapChain->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("%d"), cnt);
			OutputMsg(_T("IDXGISwapChain�̎Q�ƃJ�E���^���s���ł��B"), s, _T("�m�f"));
		}
		m_SwapChain = NULL;
	}

	if (m_D3DDeviceContext)
	{
		cnt = m_D3DDeviceContext->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("%d"), cnt);
			OutputMsg(_T("ID3D11DeviceContext�̎Q�ƃJ�E���^���s���ł��B"), s, _T("�m�f"));
		}
		m_D3DDeviceContext = NULL;
	}

	if (m_D3DDevice)
	{
		cnt = m_D3DDevice->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("%d"), cnt);
			OutputMsg(_T("ID3D11Device�̎Q�ƃJ�E���^���s���ł��B"), s, _T("�m�f"));
		}
		m_D3DDevice = NULL;
	}

	if (m_Adapter)
	{
		cnt = m_Adapter->Release();
		if (cnt != 0)
		{
			_stprintf_s(s, _T("%d"), cnt);
			OutputMsg(_T("IDXGIAdapter�̎Q�ƃJ�E���^���s���ł��B"), s, _T("�m�f"));
		}
		m_Adapter = NULL;
	}
}

// �f�t�H���g�A�_�v�^�[�̃C���^�[�t�F�[�X���쐬����
HRESULT D3D11USER::CreateAdapter()
{
	HRESULT hr = E_FAIL;
	IDXGIFactory* pFactory = NULL;

	if (m_Adapter == NULL)
	{
		// �t�@�N�g�����쐬����B
		// CreateDXGIFactory
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
		if (FAILED(hr)) goto EXIT;

		// �f�t�H���g�A�_�v�^�[���擾
		// IDXGIFactory::EnumAdapters
		hr = pFactory->EnumAdapters(0, &m_Adapter);
		if (FAILED(hr)) goto EXIT;
	}

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pFactory);

	return hr;
}

// �f�t�H���g�f�B�X�v���C�̃��[�h��񋓂���
HRESULT D3D11USER::GetDisplayMode()
{
	HRESULT hr = E_FAIL;
	IDXGIOutput* pOutput = NULL;
	DXGI_MODE_DESC* desc = NULL;

	TCHAR s[256];

	OutputMsg(_T("�f�B�X�v���C���[�h�ꗗ�̗�"), _T(""), _T("�J�n"));

	m_DisplayModeDesc.clear();

	// �A�_�v�^�[���擾�B
	hr = CreateAdapter();
	if (FAILED(hr)) goto EXIT;

	// �A�_�v�^�[�ɐڑ�����Ă���v���C�}���f�B�X�v���C���擾�B
	// IDXGIAdapter::EnumOutputs
	hr = m_Adapter->EnumOutputs(0, &pOutput);
	if (FAILED(hr)) goto EXIT;

	UINT num;
	// �f�B�X�v���C���[�h�ꗗ�̔z�񐔂��擾
	// IDXGIOutput::GetDisplayModeList
	hr = pOutput->GetDisplayModeList(D3D11USER_FORMAT, 0, &num, NULL);
	if (FAILED(hr)) goto EXIT;

	desc = new DXGI_MODE_DESC[num];
	// �f�B�X�v���C���[�h�̈ꗗ���擾
	hr = pOutput->GetDisplayModeList(D3D11USER_FORMAT, 0, &num, desc);
	if (FAILED(hr)) goto EXIT;

	for (UINT i = 0; i < num; i++)
	{
		m_DisplayModeDesc.push_back(desc[i]);
		_stprintf_s(s, _T("�𑜓x( %d : %d )�@���t���b�V�����[�g( %d / %d )"),
			m_DisplayModeDesc[i].Width, m_DisplayModeDesc[i].Height, m_DisplayModeDesc[i].RefreshRate.Denominator, m_DisplayModeDesc[i].RefreshRate.Numerator);
		OutputMsg(_T("�f�B�X�v���C���[�h"), s, _T("���X�g�ɒǉ�"));
		displayModeSize = i;

		WINDOWSIZE_WIDTH = m_DisplayModeDesc[i].Width;
		WINDOWSIZE_HEIGHT = m_DisplayModeDesc[i].Height;
	}

	OutputMsg(_T("�f�B�X�v���C���[�h�ꗗ�̗�"), _T(""), _T("����"));

	hr = S_OK;

EXIT:
	SAFE_DELETE_ARRAY(desc);
	SAFE_RELEASE(pOutput);

	return hr;
}

// �E�B���h�E�̍쐬 ����� Direct3D�̏�����
HRESULT D3D11USER::InitD3D11(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode,
	BOOL Windowed, BOOL DirectX11Only, BOOL MultisampleEnabled, BOOL UseDepthBuffer)
{
	HRESULT hr = E_FAIL;

	m_UseDepthBuffer = UseDepthBuffer;

	// �E�B���h�E���쐬
	hr = InitWindow(AppName, hInstance, WndProc, pDisplayMode);
	if (FAILED(hr)) goto EXIT;

	// �A�_�v�^�[���擾�B
	hr = CreateAdapter();
	if (FAILED(hr)) goto EXIT;

	// �f�o�C�X�̍쐬
	hr = CreateDevice(DirectX11Only);
	if (FAILED(hr)) goto EXIT;

	// �X���b�v�`�F�[���̍쐬
	hr = CreateSwapChain(pDisplayMode, MultisampleEnabled);
	if (FAILED(hr)) goto EXIT;

	// �����_�����O�^�[�Q�b�g�r���[�̍쐬
	hr = CreateRenderTargetView();
	if (FAILED(hr)) goto EXIT;

	// �[�x�X�e���V���r���[�̍쐬
	hr = CreateDepthStencilView();
	if (FAILED(hr)) goto EXIT;

	// �^�[�Q�b�g�r���[�̐ݒ�
	// ID3D11DeviceContext::OMSetRenderTargets
	m_D3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	// �r���[�|�[�g�쐬
	hr = CreateViewport();
	if (FAILED(hr)) goto EXIT;

	// �\�����[�h�̎����؊����𖳌��ɂ���B
	// �����؊�����L���ɂ���ꍇ�͂��̊֐����R�����g�A�E�g����B
	// �������R�����g�A�E�g�����ꍇ�A�����N�����t���X�N���[�����[�h�ɂ����Ƃ����������삵�Ă���Ȃ��B
	hr = SetWindowAssociation();
	if (FAILED(hr)) goto EXIT;

	// �f�t�H���g�̃��X�^���C�U��ݒ肷��
	hr = SetDefaultRasterize();
	if (FAILED(hr)) goto EXIT;

	// �f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ肷��
	hr = SetDefaultDepthStencilState();
	if (FAILED(hr)) goto EXIT;

	// �E�B���h�E�쐬��X���b�v�`�F�[���̏��������ɂ̓E�B���h�E���[�h�ō쐬����B
	// ����N�����t���X�N���[���ɂ���ꍇ�́A�������I����ɕ\�����[�h��ύX����悤�ɂ���B
	if (Windowed == FALSE)
	{
		hr = ChangeWindowMode();
		if (FAILED(hr)) goto EXIT;
	}

	OutputMsg(_T("����������"), _T(""), _T("(^�B^)"));

	hr = S_OK;

EXIT:
	SAFE_RELEASE(m_Adapter);

	return hr;
}

// �E�B���h�E�쐬
HRESULT D3D11USER::InitWindow(TCHAR* AppName, HINSTANCE hInstance, WNDPROC WndProc, DXGI_MODE_DESC* pDisplayMode)
{
	HRESULT hr = E_FAIL;
	// �E�B���h�E�X�^�C��
	DWORD WindowStyle = 0;

	OutputMsg(_T("�E�B���h�E�쐬"), _T(""), _T("�J�n"));

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(DWORD);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = AppName;
	wc.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
	wc.hInstance = hInstance;
	::RegisterClassEx(&wc);

	// �����N�����̕\�����[�h�̓E�B���h�E���[�h�ɌŒ�Ƃ���B
	if (kasouFullScreen)
	WindowStyle = WS_POPUP;// D3D11USER_W_WINDOWSTYLE | WS_THICKFRAME;
	else
	WindowStyle =  D3D11USER_W_WINDOWSTYLE | WS_THICKFRAME;

	RECT rc;
	::SetRect(&rc, 0, 0, WINDOWSIZE_WIDTH, WINDOWSIZE_HEIGHT);
	// �E�B���h�E���[�h�̂Ƃ��^�C�g���o�[���l�����ĕ\���̈��ݒ肷�邽�߂ɕK�v�ȃT�C�Y���擾����
	// AdjustWindowRect
	::AdjustWindowRect(&rc, WindowStyle, FALSE);

	// �E�B���h�E���쐬
	m_hWnd = ::CreateWindow(
		wc.lpszClassName,
		wc.lpszClassName,
		WindowStyle,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (m_hWnd == NULL)
	{
		hr = E_FAIL;
		goto EXIT;
	}

	OutputMsg(_T("�E�B���h�E�쐬"), _T(""), _T("����"));

	hr = S_OK;
EXIT:
	return hr;
}

// �f�o�C�X�̍쐬
HRESULT D3D11USER::CreateDevice(BOOL DirectX11Only)
{
	HRESULT hr = E_FAIL;

	OutputMsg(_T("�f�o�C�X�쐬"), _T(""), _T("�J�n"));

	// �f�o�C�X�쐬���Ɏg�p����r�f�I�J�[�h�̐��\�ꗗ
	// D3D_FEATURE_LEVEL
	D3D_FEATURE_LEVEL FeatureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 SM 5
		D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
	};
	int featureCnt = 0;

	// �r�f�I�J�[�h��DirectX11����уV�F�[�_�[���f��5���T�|�[�g������ł̂ݎ��s�\
	if (DirectX11Only == TRUE)
		featureCnt = 1;
	// �r�f�I�J�[�h���T�|�[�g����Direct3D����уV�F�[�_�[���f���̃o�[�W��������ʂ��玩���I��
	else
		featureCnt = sizeof(FeatureLevel) / sizeof(D3D_FEATURE_LEVEL);

#if defined(DEBUG) || defined(_DEBUG)
	// �f�o�b�O�R���p�C���̏ꍇ�A�f�o�b�O���C���[��L���ɂ���B
	// �\�t�g�E�F�A ���C���[
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT createDeviceFlag = 0;
#endif

	// �f�o�C�X���쐬����B
	// D3D11CreateDevice
	hr = D3D11CreateDevice(
		m_Adapter,                  // �g�p����A�_�v�^�[��ݒ�BNULL�̏ꍇ�̓f�t�H���g�̃A�_�v�^�[�B
		D3D_DRIVER_TYPE_UNKNOWN,    // D3D_DRIVER_TYPE�̂����ꂩ�B�h���C�o�[�̎�ށBpAdapter�� NULL �ȊO�̏ꍇ�́AD3D_DRIVER_TYPE_UNKNOWN���w�肷��B
		NULL,                       // �\�t�g�E�F�A���X�^���C�U����������DLL�ւ̃n���h���BD3D_DRIVER_TYPE �� D3D_DRIVER_TYPE_SOFTWARE �ɐݒ肵�Ă���ꍇ�� NULL �ɂł��Ȃ��B
		createDeviceFlag,           // D3D11_CREATE_DEVICE_FLAG�̑g�ݍ��킹�B�f�o�C�X���쐬���Ɏg�p�����p�����[�^�B
		FeatureLevel,               // D3D_FEATURE_LEVEL�̃|�C���^
		featureCnt,                 // D3D_FEATURE_LEVEL�z��̗v�f��
		D3D11_SDK_VERSION,          // DirectX SDK�̃o�[�W�����B���̒l�͌Œ�B
		&m_D3DDevice,               // ���������ꂽ�f�o�C�X
		&m_FeatureLevel,            // �̗p���ꂽ�t�B�[�`���[���x��
		&m_D3DDeviceContext         // ���������ꂽ�f�o�C�X�R���e�L�X�g
		);
	if (FAILED(hr)) goto EXIT;

	// �A�_�v�^�[�̏����擾
	// DXGI_ADAPTER_DESC
	DXGI_ADAPTER_DESC desc;
	m_Adapter->GetDesc(&desc);

	// DirectX�̃o�[�W�����A�V�F�[�_���f���̃o�[�W�����ƍ̗p�����A�_�v�^�[�̖��O���f�o�b�N�E�B���h�E�ɏo��
	TCHAR s[256];
	switch (m_FeatureLevel)
	{
	case D3D_FEATURE_LEVEL_11_0:
		_stprintf_s(s, _T("Direct3D(11.0) SM(5)�@%s"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_10_1:
		_stprintf_s(s, _T("Direct3D(10.1) SM(4)�@%s"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_10_0:
		_stprintf_s(s, _T("Direct3D(10.0) SM(4)�@%s"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_9_3:
		_stprintf_s(s, _T("Direct3D(9.3) SM(3)�@%s"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_9_2:
		_stprintf_s(s, _T("Direct3D(9.2) SM(2)�@%s"), desc.Description);
		break;
	case D3D_FEATURE_LEVEL_9_1:
		_stprintf_s(s, _T("Direct3D(9.1) SM(2)�@%s"), desc.Description);
		break;
	}
	if (SUCCEEDED(hr))
		OutputMsg(_T("�r�f�I�J�[�h"), s, _T("�O�j"));
	else
		OutputMsg(_T("�r�f�I�J�[�h"), s, _T("�m�f"));

	OutputMsg(_T("�f�o�C�X�쐬"), _T(""), _T("����"));

	hr = S_OK;
EXIT:

	return hr;
}

// �X���b�v�`�F�[���̍쐬
HRESULT D3D11USER::CreateSwapChain(DXGI_MODE_DESC* pDisplayMode, BOOL MultisampleEnabled)
{
	HRESULT hr = E_FAIL;
	TCHAR s[256];
	IDXGIFactory* pFactory = NULL;

	OutputMsg(_T("�X���b�v�`�F�[���쐬"), _T(""), _T("�J�n"));

	// DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	// DXGI_MODE_DESC
	::CopyMemory(&(sd.BufferDesc), pDisplayMode, sizeof(DXGI_MODE_DESC));
	sd.BufferDesc.Format = D3D11USER_FORMAT;                         // �o�b�N�o�b�t�@�t�H�[�}�b�g
	// �o�b�N�o�b�t�@�̃T�[�t�F�X�g�p�@����� CPU �A�N�Z�X �I�v�V������\�� DXGI_USAGE �񋓌^�̃����o�[
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                // �o�b�N�o�b�t�@�Ƀ����_�����O�\�ɂ���B
	//�X���b�v �`�F�[���̃o�b�t�@�[����\���l�B
	sd.BufferCount = 1;
	// �o�̓E�B���h�E�ւ� HWND �n���h���B���̃����o�[�� NULL �ɂ͂ł��Ȃ��B
	sd.OutputWindow = m_hWnd;
	// �o�͂��E�B���h�E ���[�h�̏ꍇ�� TRUE�B����ȊO�̏ꍇ�� FALSE�B
	// �����N�����̕\�����[�h�̓E�B���h�E���[�h�ɌŒ�Ƃ���B
	sd.Windowed = TRUE;
	// �T�[�t�F�X�̃X���b�v������Ƀo�b�N�o�b�t�@�̓��e��ێ����邩�B
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;                        // �o�b�N�o�b�t�@�̓��e��ێ����Ȃ��B
	// �X���b�v �`�F�[���̓���̃I�v�V�����B
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;               // �t���X�N���[�����[�h�ɐ؂�ւ���ƁA�A�v���P�[�V���� �E�B���h�E�̃T�C�Y�ɍ��킹�ĉ𑜓x���ύX�����B

	// �t�@�N�g���[���쐬����
	// CreateDXGIFactory�ō쐬�����t�@�N�g���[���g�p����Ǝ��s�ł��邪���[�j���O�G���[�ɂȂ�̂� IDXGIAdapter ����쐬����B
	hr = m_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hr)) goto EXIT;

	// �}���`�T���v�����O�̐ݒ�
	// �}���`�T���v�����g�p���Ȃ��ꍇ�̐ݒ�
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	int startMultisample;

	// �T���v�����O�����ő�̂��̂��̗p����
	if (MultisampleEnabled == TRUE)
		startMultisample = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	// �}���`�T���v���𖳌��ɂ���B
	else
		startMultisample = 1;

	UINT Quality = 0;
	for (int i = startMultisample; i >= 0; i--)
	{
		// �T�|�[�g����N�H���e�B���x���̍ő�l���擾����
		// CheckMultisampleQualityLevels
		hr = m_D3DDevice->CheckMultisampleQualityLevels(pDisplayMode->Format, (UINT)i, &Quality);
		if (FAILED(hr)) goto EXIT;

		// 0 �ȊO�̂Ƃ��t�H�[�}�b�g�ƃT���v�����O���̑g�ݍ��킹���T�|�[�g����
		// �I�����C���w���v�ł�CheckMultisampleQualityLevels�̖߂�l�� 0 �̂Ƃ��T�|�[�g����Ȃ��Ƃ�����Ă��邪
		// pNumQualityLevels �̖߂�l�� 0 ���Ŕ��f����B
		// Direct3D 10 �� �̃I�����C���w���v�ɂ͂���������Ă���̂ŁA�I�����C���w���v�̊ԈႢ�B
		if (Quality != 0)
		{
			sd.SampleDesc.Count = i;
			sd.SampleDesc.Quality = Quality - 1;

			// �X���b�v�`�F�[�����쐬����B
			// CreateSwapChain
			hr = pFactory->CreateSwapChain(m_D3DDevice, &sd, &m_SwapChain);
			_stprintf_s(s, _T("Count(%d) Quality(%d)"), sd.SampleDesc.Count, sd.SampleDesc.Quality);
			if (SUCCEEDED(hr))
			{
				OutputMsg(_T("�}���`�T���v��"), s, _T("�O�j"));
				break;
			}
			else
				OutputMsg(_T("�}���`�T���v��"), s, _T("�m�f"));
		}
	}
	if (m_SwapChain == NULL)
	{
		hr = E_FAIL;
		goto EXIT;
	}

	_stprintf_s(s, _T("�𑜓x( %d : %d )�@���t���b�V�����[�g( %d / %d )"),
		pDisplayMode->Width, pDisplayMode->Height, pDisplayMode->RefreshRate.Denominator, pDisplayMode->RefreshRate.Numerator);
	OutputMsg(_T("�f�B�X�v���C���[�h"), s, _T("�I��"));

	OutputMsg(_T("�X���b�v�`�F�[���쐬"), _T(""), _T("����"));

	hr = S_OK;

EXIT:
	SAFE_RELEASE(pFactory);

	return hr;
}

// �����_�����O�^�[�Q�b�g�r���[�̍쐬�@�e�N�X�`���𒣂�t����̂ɕK�v
HRESULT D3D11USER::CreateRenderTargetView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D* pBackBuffer = NULL;

	OutputMsg(_T("�����_�����O�^�[�Q�b�g�r���[�쐬"), _T(""), _T("�J�n"));

	// �X���b�v�`�F�[������o�b�N�o�b�t�@���擾����
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr)) goto EXIT;

	// �����_�����O�^�[�Q�b�g�r���[�̍쐬
	// CreateRenderTargetView
	hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	if (FAILED(hr)) goto EXIT;

	OutputMsg(_T("�����_�����O�^�[�Q�b�g�r���[�쐬"), _T(""), _T("����"));

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBackBuffer);

	return hr;
}

// �[�x�X�e���V���r���[�̍쐬�@�^�C�����A�e�Ȃ�
HRESULT D3D11USER::CreateDepthStencilView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D* pBackBuffer = NULL;
	ID3D11Texture2D* pDepthBuffer = NULL;

	// D3D11_TEXTURE2D_DESC
	D3D11_TEXTURE2D_DESC descDS;

	// D3D11_DEPTH_STENCIL_VIEW_DESC
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

	if (m_UseDepthBuffer == FALSE)
	{
		hr = S_OK;
		goto EXIT;
	}

	if (m_SwapChain == NULL) goto EXIT;

	OutputMsg(_T("�[�x�o�b�t�@�r���[�쐬"), _T(""), _T("�J�n"));

	DXGI_SWAP_CHAIN_DESC chainDesc;
	// �X���b�v�`�F�[���̐ݒ���擾����
	// GetDesc
	hr = m_SwapChain->GetDesc(&chainDesc);
	if (FAILED(hr)) goto EXIT;

	::ZeroMemory(&descDS, sizeof(D3D11_TEXTURE2D_DESC));
	descDS.Width = chainDesc.BufferDesc.Width;   // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Height = chainDesc.BufferDesc.Height;  // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.MipLevels = 1;                            // �~�b�v�}�b�v���쐬���Ȃ�
	descDS.ArraySize = 1;                            // �e�N�X�`���[�̔z��
	descDS.Format = D3D11USER_DEPTH_FORMAT;       // �t�H�[�}�b�g
	descDS.SampleDesc.Count = chainDesc.SampleDesc.Count;   // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality; // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Usage = D3D11_USAGE_DEFAULT;          // GPU �ɂ��ǂݎ�肨��я������݃A�N�Z�X��K�v�Ƃ��郊�\�[�X�B
	//   descDS.BindFlags          = D3D11_BIND_DEPTH_STENCIL;     // �X�e���V���o�b�t�@
	descDS.BindFlags = D3D11_BIND_DEPTH_STENCIL |    // �[�x�X�e���V���o�b�t�@�Ƃ��č쐬����
		D3D11_BIND_SHADER_RESOURCE;   // �V�F�[�_�[���\�[�X�r���[�Ƃ��č쐬����
	descDS.CPUAccessFlags = 0;                            // CPU �A�N�Z�X���s�v�B
	descDS.MiscFlags = 0;                            // ���̑��̃t���O���ݒ肵�Ȃ��B
	// �[�x�o�b�t�@�p�̃e�N�X�`���[�쐬
	// CreateTexture2D
	hr = m_D3DDevice->CreateTexture2D(&descDS, NULL, &pDepthBuffer);
	if (FAILED(hr)) goto EXIT;

	::ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// �e�N�X�`���[�쐬���Ɏw�肵���t�H�[�}�b�g�ƌ݊���������A�[�x�X�e���V���r���[�Ƃ��Ďw��ł���t�H�[�}�b�g���w�肷��
	//   descDSV.Format = descDS.Format;
	switch (descDS.Format)
	{
		// 8�r�b�g�t�H�[�}�b�g�͎g�p�ł��Ȃ��H
	case DXGI_FORMAT_R8_TYPELESS:
		descDSV.Format = DXGI_FORMAT_R8_UNORM;
		break;
	case DXGI_FORMAT_R16_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D16_UNORM;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;
	default:
		descDSV.Format = descDS.Format;
	}

	// �}���`�T���v�����g�p���Ă���ꍇ
	if (chainDesc.SampleDesc.Count > 1) descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	// �}���`�T���v�����g�p���Ă��Ȃ��ꍇ
	else                              descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	descDSV.Texture2D.MipSlice = 0;
	// �[�x�X�e���V���r���[���쐬
	// CreateDepthStencilView
	hr = m_D3DDevice->CreateDepthStencilView(pDepthBuffer, &descDSV, &m_DepthStencilView);
	if (FAILED(hr)) goto EXIT;

	OutputMsg(_T("�[�x�o�b�t�@�r���[�쐬"), _T(""), _T("����"));

	hr = S_OK;

EXIT:
	SAFE_RELEASE(pDepthBuffer);
	SAFE_RELEASE(pBackBuffer);

	return hr;
}

// �r���[�|�[�g�̍쐬
HRESULT D3D11USER::CreateViewport()
{
	HRESULT hr = E_FAIL;

	if (m_SwapChain == NULL) goto EXIT;

	//OutputMsg(_T("�r���[�|�[�g�쐬"), _T(""), _T("�J�n"));

	DXGI_SWAP_CHAIN_DESC chainDesc;
	// �X���b�v�`�F�[���̐ݒ���擾����
	hr = m_SwapChain->GetDesc(&chainDesc);
	if (FAILED(hr)) goto EXIT;

	// D3D11_VIEWPORT
	D3D11_VIEWPORT Viewport[1];
	Viewport[0].TopLeftX = 0;
	Viewport[0].TopLeftY = 0;
	Viewport[0].Width = (FLOAT)chainDesc.BufferDesc.Width;
	Viewport[0].Height = (FLOAT)chainDesc.BufferDesc.Height;
	Viewport[0].MinDepth = 0.0f;
	Viewport[0].MaxDepth = 1.0f;
	// RSSetViewports
	m_D3DDeviceContext->RSSetViewports(1, Viewport);

	//OutputMsg(_T("�r���[�|�[�g�쐬"), _T(""), _T("����"));

	hr = S_OK;
EXIT:
	return hr;
}

// �E�B���h�E�A�\�V�G�[�V����
HRESULT D3D11USER::SetWindowAssociation()
{
	HRESULT hr = E_FAIL;
	IDXGIFactory* pFactory = NULL;

	OutputMsg(_T("�E�B���h�E�A�\�V�G�[�V�����ݒ�"), _T(""), _T("�J�n"));

	// �t�@�N�g���[���쐬����
	// CreateDXGIFactory�ō쐬�����t�@�N�g���[���g�p����Ǝ��s�ł��邪���[�j���O�G���[�ɂȂ�̂� IDXGIAdapter ����쐬����B
	hr = m_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hr)) goto EXIT;

	// �\�����[�h�̎����؊����𖳌��ɂ���B
	// MakeWindowAssociation
	hr = pFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
	if (FAILED(hr)) goto EXIT;

	OutputMsg(_T("�E�B���h�E�A�\�V�G�[�V�����ݒ�"), _T(""), _T("����"));

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pFactory);

	return hr;
}

// �f�t�H���g�̃��X�^���C�U��ݒ�
HRESULT D3D11USER::SetDefaultRasterize()
{
	HRESULT hr = E_FAIL;

	OutputMsg(_T("�f�t�H���g�̃��X�^���C�U��ݒ�"), _T(""), _T("�J�n"));

	// ID3D11RasterizerState
	ID3D11RasterizerState* g_pRasterState = NULL;
	// D3D11_RASTERIZER_DESC
	D3D11_RASTERIZER_DESC rsState;
	rsState.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;    // �|���S���ʕ`��
	rsState.CullMode = D3D11_CULL_BACK;     // ���ʂ�`�悵�Ȃ�

	//   rsState.FrontCounterClockwise = FALSE;  // ���v����\��
	rsState.FrontCounterClockwise = TRUE;   // �����v����\��
	rsState.DepthBias = 0;
	rsState.DepthBiasClamp = 0;
	rsState.SlopeScaledDepthBias = 0;
	rsState.DepthClipEnable = TRUE;
	rsState.ScissorEnable = FALSE;          // �V�U�[��`����

	// �X���b�v�`�F�[���̃}���`�T���v�����O�̐ݒ�ɂ��킹��
	DXGI_SWAP_CHAIN_DESC swapDesc;
	m_SwapChain->GetDesc(&swapDesc);
	if (swapDesc.SampleDesc.Count != 1)
		rsState.MultisampleEnable = TRUE;
	else
		rsState.MultisampleEnable = FALSE;

	rsState.AntialiasedLineEnable = FALSE;
	// CreateRasterizerState
	hr = m_D3DDevice->CreateRasterizerState(&rsState, &g_pRasterState);
	if (FAILED(hr)) goto EXIT;

	// RSSetState
	m_D3DDeviceContext->RSSetState(g_pRasterState);

	hr = S_OK;

	OutputMsg(_T("�f�t�H���g�̃��X�^���C�U��ݒ�"), _T(""), _T("����"));

EXIT:
	SAFE_RELEASE(g_pRasterState);
	return hr;
}

// �f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�
HRESULT D3D11USER::SetDefaultDepthStencilState()
{
	HRESULT hr = E_FAIL;

	OutputMsg(_T("�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�"), _T(""), _T("�J�n"));

	ID3D11DepthStencilState* pDepthStencilState = NULL;
	// D3D11_RASTERIZER_DESC
	D3D11_DEPTH_STENCIL_DESC dsState;
	::ZeroMemory(&dsState, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsState.DepthEnable = TRUE;
	dsState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dsState.DepthFunc = D3D11_COMPARISON_LESS;
	dsState.StencilEnable = FALSE;

	hr = m_D3DDevice->CreateDepthStencilState(&dsState, &pDepthStencilState);
	if (FAILED(hr)) goto EXIT;

	m_D3DDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);

	hr = S_OK;

	OutputMsg(_T("�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�"), _T(""), _T("����"));

EXIT:
	SAFE_RELEASE(pDepthStencilState);
	return hr;
}

// �\�����[�h�̐؂�ւ����œK��
// DXGI �̊T�v
HRESULT D3D11USER::ChangeWindowModeOptimization(UINT Width, UINT Height)
{
	HRESULT hr = E_FAIL;

	if (m_SwapChain == NULL) goto EXIT;

	OutputMsg(_T("�\�����[�h�ύX�̍œK������"), _T(""), _T("�J�n"));

	DXGI_SWAP_CHAIN_DESC desc;
	hr = m_SwapChain->GetDesc(&desc);
	if (FAILED(hr)) goto EXIT;

	// �^�[�Q�b�g�r���[������
	OutputMsg(_T("�^�[�Q�b�g�r���[������"), _T(""), _T("�J�n"));

	// OMSetRenderTargets
	m_D3DDeviceContext->OMSetRenderTargets(0, NULL, NULL);
	SAFE_RELEASE(m_RenderTargetView);
	SAFE_RELEASE(m_DepthStencilView);
	OutputMsg(_T("�^�[�Q�b�g�r���[������"), _T(""), _T("����"));

	// �X���b�v �`�F�[���̃o�b�N �o�b�t�@�[ �T�C�Y�A�t�H�[�}�b�g�A����уo�b�t�@�[����ύX����B
	// �A�v���P�[�V���� �E�B���h�E�̃T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo���K�v������B
	// ResizeBuffers
	hr = m_SwapChain->ResizeBuffers(desc.BufferCount, Width, Height, desc.BufferDesc.Format, desc.Flags);
	if (FAILED(hr)) goto EXIT;

	// �����_�����O�^�[�Q�b�g�r���[�̍쐬
	hr = CreateRenderTargetView();
	if (FAILED(hr)) goto EXIT;

	// �[�x�X�e���V���r���[�̍쐬
	hr = CreateDepthStencilView();
	if (FAILED(hr)) goto EXIT;

	// �^�[�Q�b�g�r���[�̐ݒ�
	m_D3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	OutputMsg(_T("�\�����[�h�ύX�̍œK������"), _T(""), _T("����"));

	hr = S_OK;
EXIT:
	return hr;
}

// �\�����[�h��؂�ւ���
HRESULT D3D11USER::ChangeWindowMode()
{
	HRESULT hr = E_FAIL;

	if (m_SwapChain == NULL) goto EXIT;

	OutputMsg(_T("�\�����[�h�ύX"), _T(""), _T("�J�n"));

	DXGI_SWAP_CHAIN_DESC desc;
	hr = m_SwapChain->GetDesc(&desc);
	if (FAILED(hr)) goto EXIT;

	BOOL FullScreen;

	// GetFullscreenState
	hr = m_SwapChain->GetFullscreenState(&FullScreen, NULL);
	if (FAILED(hr)) goto EXIT;

	// SetFullscreenState
	hr = m_SwapChain->SetFullscreenState(!FullScreen, NULL);
	if (FAILED(hr)) goto EXIT;

	switch (FullScreen)
	{
	case TRUE:
		OutputMsg(_T("�\�����[�h�ύX"), _T("�E�B���h�E���[�h�ɕύX"), _T("����"));
		break;
	case FALSE:
		OutputMsg(_T("�\�����[�h�ύX"), _T("�t���X�N���[���ɕύX"), _T("����"));
		break;
	}

	// �����N�����t���X�N���[�����[�h�ɂ����ꍇ�A�E�B���h�E���[�h�ɕύX�����
	// �E�B���h�E���A�N�e�B�u�ɂȂ�Ȃ��̂ŕ\��������B
	::ShowWindow(m_hWnd, SW_SHOW);

	// �t���X�N���[������E�B���h�E���[�h�ɐ؂�ւ���Ƃ� WM_SIZE �C�x���g�����������A���ʓI�� IDXGISwapChain::ResizeBuffers ���R�[������Ȃ��B
	// ���ɂ����悤�����A��ʏ�ɉ����\������Ȃ����ۂ���������\��������̂�
	// D3D11USER::ChangeWindowModeOptimization() �� D3D11USER::ChangeWindowMode() ���ŃR�[������悤�ɏC�����A�����ł̏����͖����ɂ���
	hr = ChangeWindowModeOptimization(0, 0);

	hr = S_OK;
EXIT:
	return hr;
}

// ���_�A�C���f�b�N�X�A�萔�o�b�t�@���쐬����
HRESULT D3D11USER::CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag)
{
	HRESULT hr = E_FAIL;

	// �o�b�t�@�[ ���\�[�X�B
	// D3D11_BUFFER_DESC
	D3D11_BUFFER_DESC BufferDesc;

	// �T�u���\�[�X
	// D3D11_SUBRESOURCE_DATA
	D3D11_SUBRESOURCE_DATA* resource = NULL;

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT CPUAccessFlags = 0;

	switch (CPUAccessFlag)
	{
		// CPU�A�N�Z�X�������Ȃ�
	case 0:
		Usage = D3D11_USAGE_DEFAULT;
		CPUAccessFlags = CPUAccessFlag;
		break;
		// CPU�A�N�Z�X��������
	default:
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = CPUAccessFlag;
		break;
	}

	// �����l��ݒ肷��
	if (pData)
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pData;
		resource->SysMemPitch = 0;
		resource->SysMemSlicePitch = 0;
	}

	// �o�b�t�@�̐ݒ�
	::ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = size;                      // �o�b�t�@�T�C�Y
	BufferDesc.Usage = Usage;                     // ���\�[�X�g�p�@����肷��
	BufferDesc.BindFlags = BindFlag;                  // �o�b�t�@�̎��
	BufferDesc.CPUAccessFlags = CPUAccessFlags;            // CPU �A�N�Z�X
	BufferDesc.MiscFlags = 0;                         // ���̑��̃t���O���ݒ肵�Ȃ�

	// �o�b�t�@���쐬����
	// ID3D11Device::CreateBuffer
	hr = m_D3DDevice->CreateBuffer(&BufferDesc, resource, pBuffer);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;

EXIT:
	SAFE_DELETE(resource);
	return hr;
}

// ���_�o�b�t�@���쐬����
HRESULT D3D11USER::CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_VERTEX_BUFFER);
}

// �C���f�b�N�X�o�b�t�@���쐬����
HRESULT D3D11USER::CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_INDEX_BUFFER);
}

// �萔�o�b�t�@���쐬����
HRESULT D3D11USER::CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, CPUAccessFlag, D3D11_BIND_CONSTANT_BUFFER);
}

// �e�N�X�`���[�t�@�C������V�F�[�_�[���\�[�X�r���[�z����쐬����
HRESULT D3D11USER::CreateShaderResourceViewArray(TCHAR* pTextureFileNames[], UINT NumTextures, ID3D11ShaderResourceView** pSRVView)
{
	HRESULT hr = E_FAIL;

	ID3D11Texture2D* pTexture = NULL;

	D3DX11_IMAGE_LOAD_INFO info;
	::ZeroMemory(&info, sizeof(D3DX11_IMAGE_LOAD_INFO));
	info.Width = D3DX11_DEFAULT;
	info.Height = D3DX11_DEFAULT;
	info.Depth = D3DX11_DEFAULT;
	info.FirstMipLevel = D3DX11_DEFAULT;          // �e�N�X�`���[�̍ō��𑜓x�̃~�b�v�}�b�v ���x���B
	info.MipLevels = 1;                           // �~�b�v�}�b�v���B�����ł�1�݂̂Ƃ���B
	info.Usage = D3D11_USAGE_STAGING;             // GPU ���� CPU �ւ̃f�[�^�]�� (�R�s�[) ���T�|�[�g
	info.BindFlags = 0;
	info.CpuAccessFlags = D3D11_CPU_ACCESS_READ;  // ���\�[�X���}�b�v�\�ɂ��ACPU �����̃��\�[�X�̓��e��ǂݎ���悤�ɂ���
	info.MiscFlags = 0;
	info.Format = DXGI_FORMAT_FROM_FILE;
	info.Filter = D3DX11_FILTER_POINT;            // �e�N�X�`���[�ǂݍ��ݎ��Ɏg�p����t�B���^�[
	info.MipFilter = D3DX11_FILTER_POINT;         // �~�b�v�}�b�v�쐬���Ɏg�p����t�B���^�[

	D3D11_TEXTURE2D_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	for (UINT i = 0; i < NumTextures; i++)
	{
		ID3D11Resource* pLoadedRes = NULL;
		ID3D11Texture2D* pLoadedTexture2D = NULL;

		// �摜�t�@�C����ǂݍ���Ńe�N�X�`���[���쐬����
		hr = D3DX11CreateTextureFromFile(m_D3DDevice, pTextureFileNames[i], &info, NULL, &pLoadedRes, NULL);
		if (FAILED(hr))
			goto EXIT;

		hr = pLoadedRes->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast <void**>(&pLoadedTexture2D));
		if (FAILED(hr))
			goto EXIT;

		// ��e�N�X�`���[�쐬
		if (i == 0)
		{
			// �摜�t�@�C����ǂݍ���ō쐬�����e�N�X�`���[�̏����擾����
			pLoadedTexture2D->GetDesc(&desc);

			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.ArraySize = NumTextures;
			hr = m_D3DDevice->CreateTexture2D(&desc, NULL, &pTexture);
			if (FAILED(hr))
				goto EXIT;
		}

		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// �t�@�C������摜�t�@�C����ǂݍ���ō쐬�����e�N�X�`���[���A�ǂݍ��݃��[�h�Ń}�b�v����
		hr = m_D3DDeviceContext->Map(pLoadedTexture2D, NULL, D3D11_MAP_READ, 0, &mappedResource);
		if (FAILED(hr))
			goto EXIT;

		// ��e�N�X�`���[�̃T�u���\�[�X���t�@�C������摜�t�@�C����ǂݍ���ō쐬�����e�N�X�`���[�̓��e�ōX�V����
		for (UINT iMip = 0; iMip < desc.MipLevels; iMip++)
		{
			m_D3DDeviceContext->UpdateSubresource(pTexture,
				D3D11CalcSubresource(iMip, i, desc.MipLevels),
				NULL,
				mappedResource.pData,
				mappedResource.RowPitch,
				0
				);
		}
		m_D3DDeviceContext->Unmap(pLoadedTexture2D, NULL);

		SAFE_RELEASE(pLoadedTexture2D);
		SAFE_RELEASE(pLoadedRes);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
	SRVDesc.Texture2DArray.ArraySize = NumTextures;
	// �V�F�[�_�[���\�[�X�r���[���쐬����
	hr = m_D3DDevice->CreateShaderResourceView(pTexture, &SRVDesc, pSRVView);
	if (FAILED(hr))
		goto EXIT;

	hr = S_OK;

EXIT:
	SAFE_RELEASE(pTexture);

	return hr;
}

// ���_�V�F�[�_�[���t�@�C������쐬����
HRESULT D3D11USER::CreateVertexShaderFromFile(OUT ID3D11VertexShader** pVertexShader,
	IN TCHAR pSrcFile[], IN CHAR pFunctionName[], IN CHAR pProfile[],
	OUT ID3D11InputLayout** g_pLayout,
	IN D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements)
{
	HRESULT hr = E_FAIL;
	CHAR Profile[256];

	ID3D10Blob* pBlob = NULL;

	// �s����D��Őݒ肵�A�Â��`���̋L�q�������Ȃ��悤�ɂ���
	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
	// �œK�����x����ݒ肷��
#if defined(DEBUG) || defined(_DEBUG)
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	// Direct3D 9 ���[�h�Ŏ��s���̏ꍇ�́A�o�[�W������ύX����K�v������
	switch (m_FeatureLevel)
	{
	case D3D_FEATURE_LEVEL_9_1:
	case D3D_FEATURE_LEVEL_9_2:
		strcpy_s(Profile, "vs_4_0_level_9_1");
		break;
	case D3D_FEATURE_LEVEL_9_3:
		strcpy_s(Profile, "vs_4_0_level_9_3");
		break;
	default:
		strcpy_s(Profile, pProfile);
		break;
	}

	// �t�@�C�������ɃG�t�F�N�g���R���p�C������
	hr = D3DX11CompileFromFile(pSrcFile, NULL, NULL, pFunctionName, Profile, Flag1, 0, NULL, &pBlob, NULL, NULL);
	if (FAILED(hr)) goto EXIT;

	// �R���p�C���ς݃V�F�[�_�[����A���_�V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pVertexShader);
	if (FAILED(hr)) goto EXIT;

	BYTE* vsbuffer = (LPBYTE)pBlob->GetBufferPointer();
	size_t vsbuffersize = pBlob->GetBufferSize();

	// �V�F�[�_�[�Ŏg�p�������̓o�b�t�@�[���L�q���邽�߂̓��̓��C�A�E�g�I�u�W�F�N�g���쐬����B
	hr = m_D3DDevice->CreateInputLayout(pLayoutDesc, NumElements, vsbuffer, vsbuffersize, g_pLayout);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBlob);
	return hr;
}

// ���_�V�F�[�_�[���w�b�_�t�@�C������쐬����
HRESULT D3D11USER::CreateVertexShaderFromMemory(ID3D11VertexShader** pVertexShader,
	const BYTE* pShader, size_t size,
	OUT ID3D11InputLayout** g_pLayout,
	IN D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements)
{
	HRESULT hr = E_FAIL;

	// �R���p�C���ς݃V�F�[�_�[����A���_�V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreateVertexShader(pShader, size, NULL, pVertexShader);
	if (FAILED(hr)) goto EXIT;

	// �V�F�[�_�[�Ŏg�p�������̓o�b�t�@�[���L�q���邽�߂̓��̓��C�A�E�g�I�u�W�F�N�g���쐬����B
	hr = m_D3DDevice->CreateInputLayout(pLayoutDesc, NumElements, pShader, size, g_pLayout);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	return hr;
}

// �n���V�F�[�_�[���t�@�C������쐬����
HRESULT D3D11USER::CreateHullShaderFromFile(ID3D11HullShader** pHullShader,
	TCHAR pSrcFile[],
	CHAR pFunctionName[],
	CHAR pProfile[])
{
	HRESULT hr = E_FAIL;
	CHAR Profile[256];

	ID3D10Blob* pBlob = NULL;

	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	strcpy_s(Profile, pProfile);

	// �t�@�C�������ɃG�t�F�N�g���R���p�C������
	hr = D3DX11CompileFromFile(pSrcFile, NULL, NULL, pFunctionName, Profile, Flag1, 0, NULL, &pBlob, NULL, NULL);
	if (FAILED(hr)) goto EXIT;

	// �R���p�C���ς݃V�F�[�_�[����A�n���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreateHullShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pHullShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBlob);
	return hr;
}

// �n���V�F�[�_�[������������쐬����
HRESULT D3D11USER::CreateHullShaderFromMemory(ID3D11HullShader** pHullShader, const BYTE* pShader, size_t size)
{
	HRESULT hr = E_FAIL;

	// �R���p�C���ς݃V�F�[�_�[����A�n���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreateHullShader(pShader, size, NULL, pHullShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	return hr;
}

// �h���C���V�F�[�_�[���t�@�C������쐬����
HRESULT D3D11USER::CreateDomainShaderFromFile(ID3D11DomainShader** pDomainShader,
	TCHAR pSrcFile[],
	CHAR pFunctionName[],
	CHAR pProfile[])
{
	HRESULT hr = E_FAIL;
	CHAR Profile[256];

	ID3D10Blob* pBlob = NULL;

	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	strcpy_s(Profile, pProfile);

	// �t�@�C�������ɃG�t�F�N�g���R���p�C������
	hr = D3DX11CompileFromFile(pSrcFile, NULL, NULL, pFunctionName, Profile, Flag1, 0, NULL, &pBlob, NULL, NULL);
	if (FAILED(hr)) goto EXIT;

	// �R���p�C���ς݃V�F�[�_�[����A�h���C���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreateDomainShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pDomainShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBlob);
	return hr;
}

// �h���C���V�F�[�_�[������������쐬����
HRESULT D3D11USER::CreateDomainShaderFromMemory(ID3D11DomainShader** pDomainShader, const BYTE* pShader, size_t size)
{
	HRESULT hr = E_FAIL;

	// �R���p�C���ς݃V�F�[�_�[����A�h���C���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreateDomainShader(pShader, size, NULL, pDomainShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	return hr;
}

// �W�I���g���V�F�[�_�[���t�@�C������쐬����
HRESULT D3D11USER::CreateGeometryShaderFromFile(ID3D11GeometryShader** pGeometryShader,
	TCHAR pSrcFile[],
	CHAR pFunctionName[],
	CHAR pProfile[])
{
	HRESULT hr = E_FAIL;
	CHAR Profile[256];

	ID3D10Blob* pBlob = NULL;

	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	//// Direct3D 9 ���[�h�Ŏ��s���̏ꍇ�̓G���[�Ƃ���
	//switch( m_FeatureLevel )
	//{
	//case D3D_FEATURE_LEVEL_9_1:
	//case D3D_FEATURE_LEVEL_9_2:
	//case D3D_FEATURE_LEVEL_9_3:
	//   goto EXIT;
	//default:
	strcpy_s(Profile, pProfile);
	//   break;
	//}

	// �t�@�C�������ɃG�t�F�N�g���R���p�C������
	// D3DX11CompileFromFile
	hr = D3DX11CompileFromFile(pSrcFile, NULL, NULL, pFunctionName, Profile, Flag1, 0, NULL, &pBlob, NULL, NULL);
	if (FAILED(hr)) goto EXIT;

	// �R���p�C���ς݃V�F�[�_�[����A�W�I���g���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	// ID3D11Device::CreateGeometryShader
	hr = m_D3DDevice->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pGeometryShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBlob);
	return hr;
}

// �W�I���g���V�F�[�_�[������������쐬����
HRESULT D3D11USER::CreateGeometryShaderFromMemory(ID3D11GeometryShader** pGeometryShader, const BYTE* pShader, size_t size)
{
	HRESULT hr = E_FAIL;

	// �R���p�C���ς݃V�F�[�_�[����A�W�I���g���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreateGeometryShader(pShader, size, NULL, pGeometryShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	return hr;
}

// �X�g���[�~���O�o�̓o�b�t�@�[�ւ̏������݂��\�ȃW�I���g�� �V�F�[�_�[���t�@�C������쐬����
HRESULT D3D11USER::CreateGeometryShaderWithStreamOutputFromFile(
	ID3D11GeometryShader** pGeometryShader,
	TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[],
	const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
	UINT NumEntries,
	const UINT *pBufferStrides,
	UINT NumStrides)
{
	HRESULT hr = E_FAIL;
	CHAR Profile[256];

	ID3D10Blob* pBlob = NULL;

	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif
	strcpy_s(Profile, pProfile);

	// �t�@�C�������ɃG�t�F�N�g���R���p�C������
	hr = D3DX11CompileFromFile(pSrcFile, NULL, NULL, pFunctionName, Profile, Flag1, 0, NULL, &pBlob, NULL, NULL);
	if (FAILED(hr)) goto EXIT;

	// �X�g���[���o�͂���W�I���g���V�F�[�_�[���쐬
	// ID3D11Device::CreateGeometryShaderWithStreamOutput
	hr = m_D3DDevice->CreateGeometryShaderWithStreamOutput(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		pSODeclaration,
		NumEntries,
		pBufferStrides,
		NumStrides,
		D3D11_SO_NO_RASTERIZED_STREAM,
		NULL,
		pGeometryShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBlob);
	return hr;
}

// �X�g���[�~���O�o�̓o�b�t�@�[�ւ̏������݂��\�ȃW�I���g�� �V�F�[�_�[������������쐬����
HRESULT D3D11USER::CreateGeometryShaderWithStreamOutputFromMemory(
	ID3D11GeometryShader** pGeometryShader,
	const BYTE* pShader, size_t size,
	const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
	UINT NumEntries,
	const UINT *pBufferStrides,
	UINT NumStrides)
{
	HRESULT hr = E_FAIL;

	// �X�g���[���o�͂���W�I���g���V�F�[�_�[���쐬
	// ID3D11Device::CreateGeometryShaderWithStreamOutput
	hr = m_D3DDevice->CreateGeometryShaderWithStreamOutput(
		pShader,
		size,
		pSODeclaration,
		NumEntries,
		pBufferStrides,
		NumStrides,
		D3D11_SO_NO_RASTERIZED_STREAM,
		NULL,
		pGeometryShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	return hr;
}

// �s�N�Z���V�F�[�_�[���t�@�C������쐬����
HRESULT D3D11USER::CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader,
	TCHAR pSrcFile[],
	CHAR  pFunctionName[],
	CHAR  pProfile[]
	)
{
	HRESULT hr = E_FAIL;
	CHAR Profile[256];

	ID3D10Blob* pBlob = NULL;

	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
#else
	Flag1 |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

	// Direct3D 9 ���[�h�Ŏ��s���̏ꍇ�́A�o�[�W������ύX����K�v������
	switch (m_FeatureLevel)
	{
	case D3D_FEATURE_LEVEL_9_1:
	case D3D_FEATURE_LEVEL_9_2:
		strcpy_s(Profile, "ps_4_0_level_9_1");
		break;
	case D3D_FEATURE_LEVEL_9_3:
		strcpy_s(Profile, "ps_4_0_level_9_3");
		break;
	default:
		strcpy_s(Profile, pProfile);
		break;
	}

	// �t�@�C�������ɃG�t�F�N�g���R���p�C������
	// D3DX11CompileFromFile
	hr = D3DX11CompileFromFile(pSrcFile, NULL, NULL, pFunctionName, Profile, Flag1, 0, NULL, &pBlob, NULL, NULL);
	if (FAILED(hr)) goto EXIT;

	// �R���p�C���ς݃V�F�[�_�[����A�s�N�Z���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	// ID3D11Device::CreatePixelShader
	hr = m_D3DDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pPixelShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBlob);
	return hr;
}

// �s�N�Z���V�F�[�_�[���w�b�_�t�@�C������쐬����
HRESULT D3D11USER::CreatePixelShaderFromMemory(ID3D11PixelShader** pPixelShader, const BYTE* pShader, size_t size)
{
	HRESULT hr = E_FAIL;

	// �R���p�C���ς݃V�F�[�_�[����A�s�N�Z���V�F�[�_�[ �I�u�W�F�N�g���쐬����
	hr = m_D3DDevice->CreatePixelShader(pShader, size, NULL, pPixelShader);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;
EXIT:
	return hr;
}

// �X�N���[���V���b�g�쐬
HRESULT D3D11USER::CreateScreenShot()
{
	HRESULT hr = E_FAIL;
	TCHAR root[] = _T("ScreenShot");
	TCHAR path[256];
	int maxFileCnt = 20;

	ID3D11Texture2D* pBackBuffer = NULL;
	ID3D11Texture2D* pTexture = NULL;

	OutputMsg(_T("�X�N���[���V���b�g�쐬"), _T(""), _T("�J�n"));

	// ScreenShot �t�H���_������܂����H
	if (::PathFileExists(root) == false)
	{
		// �Ȃ��̂� ScreenShot�t�H���_���쐬����
		if (::CreateDirectory(root, NULL) == FALSE) goto EXIT;
	}

	int i = 0;
	//�t�@�C�����쐬�������ăf�B�X�N���p���N���邱�Ƃ��Ȃ��悤�ɂ��邽�߁A�t�@�C���̍쐬�\�������肷��B
	while (i < maxFileCnt)
	{
		_stprintf_s(path, _T("%s\\%02d.jpg"), root, i);

		//�t�@�C��������܂����H
		if (::PathFileExists(path) == FALSE) break;

		i++;
	}
	// �X�N���[���V���b�g�̍쐬�\�����I�[�o�[���Ă��邽�ߍ쐬�ł��܂���B
	if (i >= maxFileCnt)
	{
		OutputMsg(_T("�X�N���[���V���b�g�쐬"), _T(""), _T("�쐬���I�[�o�["));
		goto EXIT;
	}

	// �X���b�v�`�F�[������o�b�N�o�b�t�@���擾����
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr)) goto EXIT;

	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	m_SwapChain->GetDesc(&swapchainDesc);

	// �o�b�N�o�b�t�@���}���`�T���v�������ō쐬����Ă���ꍇ
	if (swapchainDesc.SampleDesc.Count == 1 && swapchainDesc.SampleDesc.Quality == 0)
	{
		// ���̏ꍇ�͂��̂܂܃t�@�C���o�͂���
		// D3DX11SaveTextureToFile
		hr = D3DX11SaveTextureToFile(m_D3DDeviceContext, pBackBuffer, D3DX11_IFF_JPG, path);
		if (FAILED(hr)) goto EXIT;
	}
	// �o�b�N�o�b�t�@���}���`�T���v�����L���ō쐬����Ă���ꍇ
	else
	{
		D3D11_TEXTURE2D_DESC texDesc;
		pBackBuffer->GetDesc(&texDesc);
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		// �R�s�[��p�̃e�N�X�`���[���쐬����B
		hr = m_D3DDevice->CreateTexture2D(&texDesc, NULL, &pTexture);
		if (FAILED(hr)) goto EXIT;

		// �}���`�T���v�����O���ꂽ���\�[�X���}���`�T���v�����O����Ă��Ȃ����\�[�X�ɃR�s�[����
		// ID3D11DeviceContext::ResolveSubresource
		m_D3DDeviceContext->ResolveSubresource(pTexture, NULL, pBackBuffer, NULL, texDesc.Format);

		// �t�@�C���o��
		hr = D3DX11SaveTextureToFile(m_D3DDeviceContext, pTexture, D3DX11_IFF_JPG, path);
		if (FAILED(hr)) goto EXIT;
	}

	OutputMsg(_T("�X�N���[���V���b�g�쐬"), path, _T("����"));

	hr = S_OK;

EXIT:
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pBackBuffer);

	return hr;
}

// �u�����h �X�e�[�g�𖳌��ɂ��邽�߂̐ݒ���擾����
D3D11_RENDER_TARGET_BLEND_DESC D3D11USER::GetDefaultBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable = FALSE;
	RenderTarget.SrcBlend = D3D11_BLEND_ONE;
	RenderTarget.DestBlend = D3D11_BLEND_ZERO;
	RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

// ���`�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
D3D11_RENDER_TARGET_BLEND_DESC D3D11USER::GetAlignmentBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable = TRUE;
	RenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

// ���Z�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
D3D11_RENDER_TARGET_BLEND_DESC D3D11USER::GetAddBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable = TRUE;
	RenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend = D3D11_BLEND_ONE;
	RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

// ���Z�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
D3D11_RENDER_TARGET_BLEND_DESC D3D11USER::GetSubtractBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable = TRUE;
	RenderTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	RenderTarget.DestBlend = D3D11_BLEND_ONE;
	RenderTarget.BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

// �ώZ�����p�u�����h �X�e�[�g�̂��߂̂�ݒ���擾����
D3D11_RENDER_TARGET_BLEND_DESC D3D11USER::GetMultipleBlendDesc()
{
	D3D11_RENDER_TARGET_BLEND_DESC RenderTarget;

	RenderTarget.BlendEnable = TRUE;
	RenderTarget.SrcBlend = D3D11_BLEND_ZERO;
	RenderTarget.DestBlend = D3D11_BLEND_SRC_COLOR;
	RenderTarget.BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
	RenderTarget.DestBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	RenderTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return RenderTarget;
}

// �u�����h�X�e�[�g��ݒ肷��
HRESULT D3D11USER::SetBlendState(D3D11_RENDER_TARGET_BLEND_DESC BlendStateArray[], UINT NumBlendState, BOOL AlphaToCoverageEnable)
{
	HRESULT hr = E_FAIL;

	ID3D11BlendState* pBlendState = NULL;

	if (NumBlendState > 8)
		goto EXIT;

	D3D11_BLEND_DESC BlendDesc;
	::ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = AlphaToCoverageEnable;
	// TRUE�̏ꍇ�A�}���`�����_�[�^�[�Q�b�g�Ŋe�����_�[�^�[�Q�b�g�̃u�����h�X�e�[�g�̐ݒ���ʂɐݒ�ł���
	// FALSE�̏ꍇ�A0�Ԗڂ݂̂��g�p�����
	BlendDesc.IndependentBlendEnable = TRUE;

	for (UINT i = 0; i < NumBlendState; i++)
	{
		::CopyMemory(&BlendDesc.RenderTarget[i], &BlendStateArray[i], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	}

	hr = m_D3DDevice->CreateBlendState(&BlendDesc, &pBlendState);
	if (FAILED(hr))
		goto EXIT;

	m_D3DDeviceContext->OMSetBlendState(pBlendState, 0, 0xffffffff);

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pBlendState);
	return hr;
}

// �T���v���[�X�e�[�g���쐬����
HRESULT D3D11USER::CreateSamplerState(ID3D11SamplerState** ppSamplerState, D3D11_FILTER Filter, UINT MaxAnisotropy)
{
	HRESULT hr = E_FAIL;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = Filter;                           // �T���v�����O���Ɏg�p����t�B���^
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W
	samplerDesc.MipLODBias = 0;                            // �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	samplerDesc.MaxAnisotropy = MaxAnisotropy;             // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ��r�I�v�V�����B
	::CopyMemory(samplerDesc.BorderColor, &D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(D3DXVECTOR4)); // ���E�F
	samplerDesc.MinLOD = 0;                                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l
	hr = m_D3DDevice->CreateSamplerState(&samplerDesc, ppSamplerState);
	if (FAILED(hr))
		goto EXIT;

	hr = S_OK;
EXIT:
	return hr;
}

// �[�x�X�e���V���X�e�[�g�̒��Őݒ�ύX�p�x���������ȃp�����[�^��ݒ肷��
HRESULT D3D11USER::SetDepthStencilState(BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteEnabled)
{
	HRESULT hr = E_FAIL;

	// �[�x�X�e���V���X�e�[�g
	ID3D11DepthStencilState*  pDepthStencilState = NULL;

	// �[�x�X�e���V���X�e�[�g���쐬����
	D3D11_DEPTH_STENCIL_DESC ddsDesc;
	::ZeroMemory(&ddsDesc, sizeof(ddsDesc));
	ddsDesc.DepthEnable = DepthEnable;           // �[�x�e�X�g���s����
	ddsDesc.DepthWriteMask = DepthWriteEnabled;  // �[�x�o�b�t�@�ւ̏������݂��s����
	ddsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ddsDesc.StencilEnable = FALSE;
	hr = m_D3DDevice->CreateDepthStencilState(&ddsDesc, &pDepthStencilState);
	if (FAILED(hr))
		goto EXIT;

	m_D3DDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);

	hr = S_OK;
EXIT:
	SAFE_RELEASE(pDepthStencilState);
	return hr;
}

// �[�x�X�e���V���r���[�̃V�F�[�_�[���\�[�X�r���[���擾
ID3D11ShaderResourceView* D3D11USER::GetSRViewFromDepthStencil()
{
	HRESULT hr = E_FAIL;
	ID3D11ShaderResourceView* pSRView = NULL;
	ID3D11Resource* pResource = NULL;

	if (m_DepthStencilView == NULL) goto EXIT;

	// �[�x�X�e���V���r���[�̐ݒ���擾����
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	m_DepthStencilView->GetDesc(&DSVDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	::ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	// ���\�[�X�Ƀr���[��K�p����ꍇ�A���\�[�X �r���[�̋L�q�ŁA���\�[�X �t�H�[�}�b�g�ƌ݊����̂���^�̃t�H�[�}�b�g���w�肷��K�v������
	switch (DSVDesc.Format)
	{
		// 8�r�b�g�t�H�[�}�b�g�͎g�p�ł���̂��H
	case DXGI_FORMAT_R8_UNORM:
		SRVDesc.Format = DXGI_FORMAT_R8_UNORM;
		break;
	case DXGI_FORMAT_D16_UNORM:
		SRVDesc.Format = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	}
	DXGI_SWAP_CHAIN_DESC SCDesc;
	// �X���b�v�`�F�[���̐ݒ���擾����
	m_SwapChain->GetDesc(&SCDesc);
	// �}���`�T���v�����O���g�p���Ȃ�
	if (SCDesc.SampleDesc.Count == 1)
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	// �}���`�T���v�����O���g�p����
	else
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	// �[�x�X�e���V���r���[���烊�\�[�X���擾
	m_DepthStencilView->GetResource(&pResource);
	// �V�F�[�_�[���\�[�X�r���[���쐬����
	hr = m_D3DDevice->CreateShaderResourceView(pResource, &SRVDesc, &pSRView);
	if (FAILED(hr)) goto EXIT;

	hr = S_OK;

EXIT:
	SAFE_RELEASE(pResource);
	return pSRView;
}

// �V�F�[�_�[�Ƀo�C���h�\�ȃ����_�[�^�[�Q�b�g�r���[���쐬����
HRESULT D3D11USER::CreateRenderTargetView(ID3D11RenderTargetView** ppRTView, D3DXVECTOR2* pRTViewSize, DXGI_FORMAT Format)
{
	return CreateRenderTargetView(ppRTView, pRTViewSize, Format, 1);
}

// �V�F�[�_�[�Ƀo�C���h�\�ȃ����_�[�^�[�Q�b�g�r���[���쐬����
HRESULT D3D11USER::CreateRenderTargetView(ID3D11RenderTargetView** ppRTView, D3DXVECTOR2* pRTViewSize, DXGI_FORMAT Format, UINT MipLevels)
{
	HRESULT hr = E_FAIL;

	ID3D11Texture2D* pTexture2D = NULL;

	// �X���b�v�`�F�[���̏����擾����
	DXGI_SWAP_CHAIN_DESC SCDesc;
	m_SwapChain->GetDesc(&SCDesc);

	UINT Width, Height;
	if (pRTViewSize == NULL)
	{
		Width = SCDesc.BufferDesc.Width;
		Height = SCDesc.BufferDesc.Height;
	}
	else
	{
		Width = (UINT)pRTViewSize->x;
		Height = (UINT)pRTViewSize->y;
	}

	D3D11_TEXTURE2D_DESC Tex2DDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;

	::ZeroMemory(&Tex2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.Format = Format;
	Tex2DDesc.Width = Width;
	Tex2DDesc.Height = Height;
	Tex2DDesc.MipLevels = MipLevels;
	Tex2DDesc.SampleDesc.Count = SCDesc.SampleDesc.Count;
	Tex2DDesc.SampleDesc.Quality = SCDesc.SampleDesc.Quality;

	::ZeroMemory(&RTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RTVDesc.Format = Tex2DDesc.Format;
	if (Tex2DDesc.SampleDesc.Count == 1)
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	else
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	RTVDesc.Texture2D.MipSlice = 0;

	// �~�b�v�}�b�v�`�F�[���̍쐬��������
	if (MipLevels != 1)
		Tex2DDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = m_D3DDevice->CreateTexture2D(&Tex2DDesc, NULL, &pTexture2D);
	if (FAILED(hr))
		goto EXIT;

	hr = m_D3DDevice->CreateRenderTargetView(pTexture2D, &RTVDesc, ppRTView);
	if (FAILED(hr))
		goto EXIT;

	SAFE_RELEASE(pTexture2D);

	hr = S_OK;
EXIT:
	return hr;
}

// �����_�[�^�[�Q�b�g�r���[����V�F�[�_�[���\�[�X�r���[���擾����
ID3D11ShaderResourceView* D3D11USER::GetSRViewFromRTView(ID3D11RenderTargetView* pRTView)
{
	ID3D11Resource* pResource = NULL;
	ID3D11ShaderResourceView* pSRView = NULL;

	pRTView->GetResource(&pResource);
	m_D3DDevice->CreateShaderResourceView(pResource, NULL, &pSRView);
	SAFE_RELEASE(pResource);

	return pSRView;
}

//�G�t�F�N�g���쐬
HRESULT D3D11USER::CreateEffectFromMemory(unsigned char* cso_data, long cso_sz, ID3DX11Effect** g_pEffect){
	return D3DX11CreateEffectFromMemory(cso_data, cso_sz, false, m_D3DDevice, g_pEffect);
}

//�[�x�X�e���V���X�e�[�g�쐬
HRESULT D3D11USER::CreateDepthStencilState(ID3D11DepthStencilState** depthStencilState,bool depthEnable){
	D3D11_DEPTH_STENCIL_DESC ddsDesc;
	::ZeroMemory(&ddsDesc, sizeof(ddsDesc));
	ddsDesc.DepthEnable = depthEnable;                                     // �[�x�e�X�g���g�p����
	ddsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ddsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ddsDesc.StencilEnable = FALSE;
	return m_D3DDevice->CreateDepthStencilState(&ddsDesc, depthStencilState);

}

//�T���v���[�擾
HRESULT D3D11USER::GetSampler(LPCSTR samplerName, ID3DX11Effect* effect, ID3DX11EffectSamplerVariable** sampler){
	// ID3DX11EffectVariable
	ID3DX11EffectVariable* pVariable = NULL;

	// ���O�ɂ���ăT���v���[�X�e�[�g���擾
	pVariable = effect->GetVariableByName(samplerName);

	// ID3DX11EffectVariable::AsSampler
	*sampler = pVariable->AsSampler();

	return pVariable->IsValid();
}

//�T���v���[�X�e�[�g�쐬
HRESULT D3D11USER::CreateSamplerDesc(ID3DX11EffectSamplerVariable* sampler){
	// �T���v���[�X�e�[�g�̍쐬
	ID3D11SamplerState* samplerState = NULL;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;         // �T���v�����O���Ɏg�p����t�B���^�B�����ł͈ٕ����t�B���^�[���g�p����B
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� u �e�N�X�`���[���W�̕`����@
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� v �e�N�X�`���[���W�̕`����@
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;     // 0 �` 1 �͈̔͊O�ɂ��� w �e�N�X�`���[���W�̕`����@
	samplerDesc.MipLODBias = 0;                            // �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	samplerDesc.MaxAnisotropy = 16;                        // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ��r�I�v�V�����B
	::CopyMemory(samplerDesc.BorderColor, &D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(D3DXVECTOR4)); // ���E�F
	samplerDesc.MinLOD = 0;                                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);
	HRESULT hr = S_FALSE;
	// �T���v���[�X�e�[�g�͓���̃G�t�F�N�g���ŕύX����K�v���邱�Ƃ͂��܂�Ȃ��Ǝv���̂ł����Őݒ肷��B
	hr = sampler->SetSampler(0, samplerState);
	SAFE_RELEASE(samplerState);

	return hr;
}

//�o�C�i������G�t�F�N�g��ǂݍ���
int D3D11USER::ReadBinary(TCHAR* csoName, long* cso_sz, unsigned char** cso_data){
	// �o�C�i���t�@�C����ǂݍ���//
	FILE* fp;
	_wfopen_s(&fp, csoName, _T("rb"));
	if (fp == NULL){
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	long Mcso_sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* Mcso_data = new unsigned char[Mcso_sz];
	fread(Mcso_data, Mcso_sz, 1, fp);
	fclose(fp);
	*cso_data = Mcso_data;
	*cso_sz = Mcso_sz;

	return 0;
}

//�C���v�b�g���C�A�E�g�쐬
HRESULT D3D11USER::CreateInputLayout(ID3DX11EffectPass* effectPass, D3D11_INPUT_ELEMENT_DESC layout[], UINT layoutCount, ID3D11InputLayout** pLayout){
	// D3DX11_PASS_DESC
	D3DX11_PASS_DESC passDesc;
	effectPass->GetDesc(&passDesc);

	// ���̓��C�A�E�g���쐬����
	return m_D3DDevice->CreateInputLayout(layout, layoutCount, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, pLayout);
}


//�Z�b�g
//�C���v�b�g���C�A�E�g���Z�b�g
void D3D11USER::SetInputLayout(ID3D11InputLayout* layout){
	m_D3DDeviceContext->IASetInputLayout(layout);
}

//�[�x�e�X�g��L���ɂ���
void D3D11USER::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState){
	m_D3DDeviceContext->OMSetDepthStencilState(depthStencilState, 0);
}

// �p�X�Ɋ܂܂�Ă���X�e�[�g���f�o�C�X�ɐݒ�
HRESULT D3D11USER::ShaderApply(ID3DX11EffectPass* effectPass){
	return effectPass->Apply(0, m_D3DDeviceContext);
}

// �o�b�N�o�b�t�@���N���A
void D3D11USER::ClearRenderTargetView(float ClearColor[]){
	m_D3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, ClearColor);
}

//�[�x�o�b�t�@���N���A
void D3D11USER::ClearDepthStencilView(){

	if (m_DepthStencilView)
		m_D3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//���_�o�b�t�@�ݒ�
void D3D11USER::SetVertexBuffer(ID3D11Buffer** vertexBuffer, UINT* stride, UINT* offset){
	m_D3DDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer, stride, offset);
}

//�C���f�b�N�X�o�b�t�@�ݒ�
void D3D11USER::SetIndexBuffer(ID3D11Buffer* indexBuffer){
	m_D3DDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

//texture����
void D3D11USER::CreateTexture(TCHAR* texName, ID3D11ShaderResourceView** texture, D3DX11_IMAGE_LOAD_INFO info){
	D3DX11CreateShaderResourceViewFromFile(m_D3DDevice, texName, &info, NULL, texture, NULL);
}

//�A���t�@�u�����f�B���O�ݒ�
void D3D11USER::SetAlphaBlend(bool alphaEnable){
	D3D11_RENDER_TARGET_BLEND_DESC BlendDesc;
	BlendDesc = GetDefaultBlendDesc();
	SetBlendState(&BlendDesc, 1, alphaEnable);
}

//�`��
void D3D11USER::DrawIndexed(UINT indexCount, UINT StartIndexLocation, INT BaseVertexLocation){
	m_D3DDeviceContext->DrawIndexed(indexCount, StartIndexLocation, BaseVertexLocation);
}

//�����_�����O���ꂽ�C���[�W�����[�U�[�ɕ\��
void D3D11USER::Present(){
	m_SwapChain->Present(0, 0);
}

//�g�|���W�[�̃Z�b�g
void D3D11USER::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology){
	m_D3DDeviceContext->IASetPrimitiveTopology(topology);
}

//TCHAR�ɕς��Ȃ��Ɠ��{��Ή��ł��Ȃ��S�~
void D3D11USER::SetMyWindowText(HWND hwnd, std::string text){
	const char* frameTimeChar = text.c_str();
	TCHAR wstr[512];
	size_t wLen = 0;
	mbstowcs_s(&wLen, wstr, strlen(frameTimeChar) + 1, frameTimeChar, _TRUNCATE);
	SetWindowText(hwnd, wstr);
}

// �r���[�|�[�g�̕ύX
HRESULT D3D11USER::ChangeViewport(float xStart,float xEnd,float yStart,float yEnd)
{
	HRESULT hr = E_FAIL;

	if (m_SwapChain == NULL) goto EXIT;

	OutputMsg(_T("�r���[�|�[�g�쐬"), _T(""), _T("�J�n"));

	DXGI_SWAP_CHAIN_DESC chainDesc;
	// �X���b�v�`�F�[���̐ݒ���擾����
	hr = m_SwapChain->GetDesc(&chainDesc);
	if (FAILED(hr)) goto EXIT;

	// D3D11_VIEWPORT
	D3D11_VIEWPORT Viewport[1];
	Viewport[0].TopLeftX = (FLOAT)chainDesc.BufferDesc.Width * xStart;
	Viewport[0].TopLeftY = (FLOAT)chainDesc.BufferDesc.Height * yStart;
	Viewport[0].Width = (FLOAT)chainDesc.BufferDesc.Width * xEnd;
	Viewport[0].Height = (FLOAT)chainDesc.BufferDesc.Height * yEnd;
	Viewport[0].MinDepth = 0.0f;
	Viewport[0].MaxDepth = 1.0f;
	// RSSetViewports
	m_D3DDeviceContext->RSSetViewports(1, Viewport);

	OutputMsg(_T("�r���[�|�[�g�쐬"), _T(""), _T("����"));

	hr = S_OK;
EXIT:
	return hr;
}