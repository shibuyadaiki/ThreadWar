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


// アプリケーション名
TCHAR* AppName = _T("Thread War");

SceneManager sm;
float frameTime;
LARGE_INTEGER timeStart;	//パフォーマンスカウンターの開始値
LARGE_INTEGER timeEnd;		//パフォーマンスカウンターの終了値
LARGE_INTEGER timerFreq;	//パフォーマンスカウンターの周波数
const float FRAME_RATE = 60;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; // maximum time used in calculations
DWORD sleepTime;					//フレーム間でスリープする時間
float fps = 60.0f;
HWND hwnd_;
// メモリ開放
void Invalidate()
{
	Graphic::GetInstance().Release();
	Device::GetInstance().ClearAll();
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	hwnd_ = hWnd;
	switch (msg)
	{
	case WM_KEYUP:
		// アプリ終了
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		//// F2キーを押すと、ウィンドウモードを切り替える。
		//// 自動的にウィンドウモードを切り替える機能もあるが、ウィンドウスタイルを自由に変更するために自分で実装することにした。
		if (wParam == VK_F2)
		{
			Device::GetInstance().Getd3d11User()->ChangeWindowMode();
		}
		//break;

	case WM_ACTIVATE:
		//ウィンドウが非アクティブ
		//if (wParam == SIZE_RESTORED)
		//	::DestroyWindow(hWnd);

		//// フルスクリーンからウィンドウモードに切り替えるとき WM_SIZE イベントが発生せず、結果的に IDXGISwapChain::ResizeBuffers がコールされない。
		//// 環境にもよるようだが、画面上に何も表示されない現象が発生する可能性があるので
		//// D3D11USER::ChangeWindowModeOptimization() は D3D11USER::ChangeWindowMode() 内でコールするように修正し、ここでの処理は無効にする
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
// メイン関数
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
	//DirectInput初期化
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL))){
		::MessageBox(NULL, _T("DirectInput初期化エラー"), _T("DirectInput初期化エラー"), MB_OK);
	}
	// リソースの初期化
	Device::GetInstance().Initialize();
	// 表示モードを記述するための構造体。
	DXGI_MODE_DESC sd;
	// ディスプレイモード一覧を取得する。
	// 取得した値はクラス内部に保持される。
	hr = Device::GetInstance().Getd3d11User()->GetDisplayMode();
	if (FAILED(hr))
	{
		::MessageBox(NULL, _T("ディスプレイモード取得エラー"), _T("初期化エラー"), MB_OK);
		goto EXIT;
	}

	// とりあえず最初に見つかったディスプレイモードを選択する
	int gasitu = (Device::GetInstance().Getd3d11User()->displayModeSize - 1) ;// = 28;
	if (gasitu < 0)gasitu = 0;
	CopyMemory(&sd, &Device::GetInstance().Getd3d11User()->m_DisplayModeDesc[gasitu], sizeof(DXGI_MODE_DESC));

	// ウィンドウの作成およびDirect3D の初期化
	hr = Device::GetInstance().Getd3d11User()->InitD3D11(AppName, hInstance, WndProc, &sd, TRUE, false, TRUE, TRUE);
	if (FAILED(hr)){
		::MessageBox(NULL, _T("Direct3D 11.0 初期化エラー"), _T("初期化エラー"), MB_OK);
		goto EXIT;
	}
	//Device::GetInstance().Getd3d11User()->ChangeWindowMode();
	//Device::GetInstance().Getd3d11User()->ChangeWindowModeOptimization(4,3);
	//inputの初期化

	::ShowWindow(hwnd_, SW_SHOW);
	::UpdateWindow(hwnd_);

	VICTORY_ID winner = VICTORY_ID::PLAYER_WIN;
	sm.Add(Scene::Demo, std::make_shared<DemoScene>());
	sm.Add(Scene::Title, std::make_shared<TitleScene>());
	sm.Add(Scene::GamePlay, std::make_shared<GamePlayScene>(winner));
	sm.Add(Scene::Ending, std::make_shared<ResultScene>(winner));
	sm.SetScene(Scene::Demo);
	//高分解能タイマーの準備を試みる
	if (QueryPerformanceFrequency(&timerFreq) == false)
		::MessageBox(NULL, _T("Error initializing high resolution timer"), _T("Error initializing high resolution timer"), MB_OK);
	QueryPerformanceCounter(&timeStart);
	Device::GetInstance().GetInput()->Initialize(Device::GetInstance().Getd3d11User(), hInstance, dInput);
	setlocale(LC_ALL, "japanese");
	// メッセージループ
	do
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			//最後のフレームからの経過時間を計算frameTimeに保存
			QueryPerformanceCounter(&timeEnd);
			frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
			//省電力コード(winmm.libが必要)
			//希望するフレームレートに対して経過時間が短い場合
			bool ok = true;
			if (frameTime < MIN_FRAME_TIME){
				sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
				timeBeginPeriod(1);	//1ミリ秒の分解能をwindowsタイマーに要求
				Sleep(sleepTime);	//sleepTimeの間、CPUを解放
				timeEndPeriod(1);	//1ミリ秒のタイマー分解能を終了
				ok = false;
			}
			if (ok){
				if (frameTime > 0.0)
					fps = (fps * 0.99f) + (0.01f / frameTime); //平均fps
				if (frameTime > MAX_FRAME_TIME)	//フレームレートが非常に遅い場合
					frameTime = MAX_FRAME_TIME;	//最大frameTimeを制限
				timeStart = timeEnd;

				float frameTime_ = frameTime;
				std::string frameTimeString = std::to_string(fps);

				//inputのWindowActive検知

				float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

				sm.Change();
				
				RECT rc;
				GetWindowRect(hwnd_, &rc);
				Device::GetInstance().GetInput()->WindowActiveCheck(rc);
				// バックバッファをクリア
				Device::GetInstance().Getd3d11User()->ClearRenderTargetView(ClearColor);

				// 深度バッファをクリア
				Device::GetInstance().Getd3d11User()->ClearDepthStencilView();
				sm.Update(frameTime_);
				sm.Draw();
				frameTimeString = std::to_string((int)fps);
				//Graphic::GetInstance().DrawFontDirect(FONT_ID::TEST_FONT, vector2(0, 1080 - 60.0f * 0.5f), vector2(0.20f, 0.25f), 0.5f, "FPS:" + frameTimeString ,vector3(1,1,1));

				//Graphic::GetInstance().DrawAllFont();
				// レンダリングされたイメージをユーザーに表示。
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