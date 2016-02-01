#include "Mouse.h"
#include "../Math/Vector3.h"
Mouse::Mouse(){
}



Mouse::~Mouse(){

}

void Mouse::Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput){
	//DirectInputMouse初期化
	if (FAILED(dInput->CreateDevice(GUID_SysMouse, &dInputDevice, NULL))){
		::MessageBox(NULL, _T("DirectInputMouse初期化エラー"), _T("DirectInputMouse初期化エラー"), MB_OK);
	}

	//DirectInputSetFormat
	if (FAILED(dInputDevice->SetDataFormat(&c_dfDIMouse2))){
		::MessageBox(NULL, _T("DirectInputMouse初期化エラー"), _T("DirectInputMouse初期化エラー"), MB_OK);
	}

	//DirectInputsetCooperatilveLevel ケツの二つは非アクティブの時に操作が効かなくなるよってこと
	if (FAILED(dInputDevice->SetCooperativeLevel(d3d11User->m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))){
		::MessageBox(NULL, _T("DirectInputMouse初期化エラー"), _T("DirectInputMouse初期化エラー"), MB_OK);
	}

	// 軸モードを設定（相対値モードに設定）
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	//diprop.dwData       = DIPROPAXISMODE_ABS; // 絶対値モードの場合
	if (FAILED(dInputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph))){
		::MessageBox(NULL, _T("DirectInputMouse初期化エラー"), _T("DirectInputMouse初期化エラー"), MB_OK);
	}

	dInputDevice->Acquire();
}

void Mouse::WindowActiveCheck(RECT rc){
	oldDim = nowDim;
	activeFlag = true;
	if (FAILED(dInputDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &nowDim))){
		while (dInputDevice->Acquire() == DIERR_INPUTLOST);
		activeFlag = false;
	}
	else{
		SetCursorPos(rc.left + (rc.right - rc.left) / 2, rc.bottom + (rc.top - rc.bottom) / 2);
	}
	ShowCursor(!activeFlag);
}

bool Mouse::MouseButtonDown(INPUTMOUSEBUTTON id, bool trriger){
	if (!trriger)
		return ((nowDim.rgbButtons[id])) == 0 ? false:true;

	return ((nowDim.rgbButtons[id] && !oldDim.rgbButtons[id])) == 0 ? false : true;
}

bool Mouse::MouseButtonUp(INPUTMOUSEBUTTON id){
	return (!nowDim.rgbButtons[id] && oldDim.rgbButtons[id]) == 0 ? false : true;
}

Vector3 Mouse::MouseVec(){
	if (activeFlag)
	return vector3((float)nowDim.lX, -(float)nowDim.lY,0);
	
	return vector3(0, 0, 0);
}