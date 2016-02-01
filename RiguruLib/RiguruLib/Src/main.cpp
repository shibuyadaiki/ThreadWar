#include "Other\DX11User.h"
#include "Other\Device.h"
#include "scene\SceneManager.h"
#include "scene\GamePlayScene.h"
#include "scene\TitleScene.h"
#include "scene\DemoScene.h"
#include "scene\ResultScene.h"
#include "Math\Vector3.h"
#include "Math\Vector2.h"
#include "Graphic\Graphic.h"
#include "Actor\ID.h"
#include <string>
#include <crtdbg.h>


// �A�v���P�[�V������
TCHAR* AppName = _T("Thread War");

SceneManager sm;
float frameTime;
LARGE_INTEGER timeStart;	//�p�t�H�[�}���X�J�E���^�[�̊J�n�l
LARGE_INTEGER timeEnd;		//�p�t�H�[�}���X�J�E���^�[�̏I���l
LARGE_INTEGER timerFreq;	//�p�t�H�[�}���X�J�E���^�[�̎��g��
const float FRAME_RATE = 60;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; // maximum time used in calculations
DWORD sleepTime;					//�t���[���ԂŃX���[�v���鎞��
float fps = 60.0f;
HWND hwnd_;
// �������J��
void Invalidate()
{
	Graphic::GetInstance().Release();
	Device::GetInstance().ClearAll();
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	hwnd_ = hWnd;
	switch (msg)
	{
	case WM_KEYUP:
		// �A�v���I��
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		//// F2�L�[�������ƁA�E�B���h�E���[�h��؂�ւ���B
		//// �����I�ɃE�B���h�E���[�h��؂�ւ���@�\�����邪�A�E�B���h�E�X�^�C�������R�ɕύX���邽�߂Ɏ����Ŏ������邱�Ƃɂ����B
		if (wParam == VK_F2)
		{
			Device::GetInstance().Getd3d11User()->ChangeWindowMode();
		}
		//break;

	case WM_ACTIVATE:
		//�E�B���h�E����A�N�e�B�u
		//if (wParam == SIZE_RESTORED)
		//	::DestroyWindow(hWnd);

		//// �t���X�N���[������E�B���h�E���[�h�ɐ؂�ւ���Ƃ� WM_SIZE �C�x���g�����������A���ʓI�� IDXGISwapChain::ResizeBuffers ���R�[������Ȃ��B
		//// ���ɂ����悤�����A��ʏ�ɉ����\������Ȃ����ۂ���������\��������̂�
		//// D3D11USER::ChangeWindowModeOptimization() �� D3D11USER::ChangeWindowMode() ���ŃR�[������悤�ɏC�����A�����ł̏����͖����ɂ���
		//   case WM_SIZE:
		//Device::GetInstance().Getd3d11User()->ChangeWindowModeOptimization(1280, 760);
		//      break;
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0L;
}
// ���C���֐�
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPTSTR    /*lpCmpLine*/,
	INT       /*nCmdShow*/)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	HRESULT hr = E_FAIL;
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	LPDIRECTINPUT8 dInput;
	//DirectInput������
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL))){
		::MessageBox(NULL, _T("DirectInput�������G���["), _T("DirectInput�������G���["), MB_OK);
	}
	// ���\�[�X�̏�����
	Device::GetInstance().Initialize();
	// �\�����[�h���L�q���邽�߂̍\���́B
	DXGI_MODE_DESC sd;
	// �f�B�X�v���C���[�h�ꗗ���擾����B
	// �擾�����l�̓N���X�����ɕێ������B
	hr = Device::GetInstance().Getd3d11User()->GetDisplayMode();
	if (FAILED(hr))
	{
		::MessageBox(NULL, _T("�f�B�X�v���C���[�h�擾�G���["), _T("�������G���["), MB_OK);
		goto EXIT;
	}

	// �Ƃ肠�����ŏ��Ɍ��������f�B�X�v���C���[�h��I������
	int gasitu = (Device::GetInstance().Getd3d11User()->displayModeSize - 1) ;// = 28;
	if (gasitu < 0)gasitu = 0;
	CopyMemory(&sd, &Device::GetInstance().Getd3d11User()->m_DisplayModeDesc[gasitu], sizeof(DXGI_MODE_DESC));

	// �E�B���h�E�̍쐬�����Direct3D �̏�����
	hr = Device::GetInstance().Getd3d11User()->InitD3D11(AppName, hInstance, WndProc, &sd, TRUE, false, TRUE, TRUE);
	if (FAILED(hr)){
		::MessageBox(NULL, _T("Direct3D 11.0 �������G���["), _T("�������G���["), MB_OK);
		goto EXIT;
	}
	//Device::GetInstance().Getd3d11User()->ChangeWindowMode();
	//Device::GetInstance().Getd3d11User()->ChangeWindowModeOptimization(4,3);
	//input�̏�����

	::ShowWindow(hwnd_, SW_SHOW);
	::UpdateWindow(hwnd_);

	VICTORY_ID winner = VICTORY_ID::PLAYER_WIN;
	sm.Add(Scene::Demo, std::make_shared<DemoScene>());
	sm.Add(Scene::Title, std::make_shared<TitleScene>());
	sm.Add(Scene::GamePlay, std::make_shared<GamePlayScene>(winner));
	sm.Add(Scene::Ending, std::make_shared<ResultScene>(winner));
	sm.SetScene(Scene::Demo);
	//������\�^�C�}�[�̏��������݂�
	if (QueryPerformanceFrequency(&timerFreq) == false)
		::MessageBox(NULL, _T("Error initializing high resolution timer"), _T("Error initializing high resolution timer"), MB_OK);
	QueryPerformanceCounter(&timeStart);
	Device::GetInstance().GetInput()->Initialize(Device::GetInstance().Getd3d11User(), hInstance, dInput);
	setlocale(LC_ALL, "japanese");
	// ���b�Z�[�W���[�v
	do
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			//�Ō�̃t���[������̌o�ߎ��Ԃ��v�ZframeTime�ɕۑ�
			QueryPerformanceCounter(&timeEnd);
			frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
			//�ȓd�̓R�[�h(winmm.lib���K�v)
			//��]����t���[�����[�g�ɑ΂��Čo�ߎ��Ԃ��Z���ꍇ
			bool ok = true;
			if (frameTime < MIN_FRAME_TIME){
				sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
				timeBeginPeriod(1);	//1�~���b�̕���\��windows�^�C�}�[�ɗv��
				Sleep(sleepTime);	//sleepTime�̊ԁACPU�����
				timeEndPeriod(1);	//1�~���b�̃^�C�}�[����\���I��
				ok = false;
			}
			if (ok){
				if (frameTime > 0.0)
					fps = (fps * 0.99f) + (0.01f / frameTime); //����fps
				if (frameTime > MAX_FRAME_TIME)	//�t���[�����[�g�����ɒx���ꍇ
					frameTime = MAX_FRAME_TIME;	//�ő�frameTime�𐧌�
				timeStart = timeEnd;

				float frameTime_ = frameTime;
				std::string frameTimeString = std::to_string(fps);

				//input��WindowActive���m

				float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

				sm.Change();
				
				RECT rc;
				GetWindowRect(hwnd_, &rc);
				Device::GetInstance().GetInput()->WindowActiveCheck(rc);
				// �o�b�N�o�b�t�@���N���A
				Device::GetInstance().Getd3d11User()->ClearRenderTargetView(ClearColor);

				// �[�x�o�b�t�@���N���A
				Device::GetInstance().Getd3d11User()->ClearDepthStencilView();
				sm.Update(frameTime_);
				sm.Draw();
				frameTimeString = std::to_string((int)fps);
				//Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(0, 1080 - 60.0f * 0.5f), vector2(0.20f, 0.25f), 0.5f, "FPS:" + frameTimeString ,vector3(1,1,1));

				//Graphic::GetInstance().DrawAllFont();
				// �����_�����O���ꂽ�C���[�W�����[�U�[�ɕ\���B
				Device::GetInstance().Getd3d11User()->Present();
			}
			if (FAILED(hr))
				::DestroyWindow(Device::GetInstance().Getd3d11User()->m_hWnd);
		}
	} while (msg.message != WM_QUIT);

EXIT:
	if (Device::GetInstance().Getd3d11User() && Device::GetInstance().Getd3d11User()->m_hWnd)
		::DestroyWindow(Device::GetInstance().Getd3d11User()->m_hWnd);

	::UnregisterClass(AppName, hInstance);
	Invalidate();
	return msg.wParam;
}