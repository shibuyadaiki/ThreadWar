#include "KeyBoard.h"

KeyBoard::KeyBoard(){
}



KeyBoard::~KeyBoard(){

}

void KeyBoard::Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput){
	//DirectInputKeyBoard初期化
	if (FAILED(dInput->CreateDevice(GUID_SysKeyboard, &dInputDevice, NULL))){
		::MessageBox(NULL, _T("DirectInputKeyBoard初期化エラー"), _T("DirectInputKeyBoard初期化エラー"), MB_OK);
	}
	
	//DirectInputSetFormat
	dInputDevice->SetDataFormat(&c_dfDIKeyboard);
	
	//DirectInputsetCooperatilveLevel ケツの二つは非アクティブの時に操作が効かなくなるよってこと
	dInputDevice->SetCooperativeLevel(d3d11User->m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// 軸モードを設定（相対値モードに設定）
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	//diprop.dwData       = DIPROPAXISMODE_ABS; // 絶対値モードの場合
	dInputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	ZeroMemory(keyBoardOldBuf, sizeof(keyBoardOldBuf));
	ZeroMemory(keyBoardNowBuf, sizeof(keyBoardNowBuf));
	dInputDevice->Acquire();
}

void KeyBoard::WindowActiveCheck(){
	//今のキー情報を1フレーム前にコピー
	memcpy(keyBoardOldBuf, keyBoardNowBuf, sizeof(keyBoardNowBuf));
	//今のキーをクリア
	ZeroMemory(keyBoardNowBuf, sizeof(keyBoardNowBuf));
	oldDim = nowDim;
	//デバイスが非アクティブになった時のバグ対策
	if (FAILED(dInputDevice->GetDeviceState(sizeof(keyBoardNowBuf), keyBoardNowBuf))){
		while (dInputDevice->Acquire() == DIERR_INPUTLOST);
	}
}

BYTE KeyBoard::KeyDown(const UINT KeyCode,bool trigger)const{
	if (!trigger)
	return (keyBoardNowBuf[KeyCode]);

	return (keyBoardNowBuf[KeyCode] && !keyBoardOldBuf[KeyCode]);
}

bool KeyBoard::KeyUp(const UINT KeyCode)const{
	return (!keyBoardNowBuf[KeyCode] && keyBoardOldBuf[KeyCode]);
}