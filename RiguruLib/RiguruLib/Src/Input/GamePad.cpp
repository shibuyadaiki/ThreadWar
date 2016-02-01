#include "GamePad.h"

LPDIRECTINPUTDEVICE8 dInputDeviceGP[PAD_MAX];
DIDEVCAPS dInputDevCapsGP;
LPDIRECTINPUT8 dInpGP;
int padNo;

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	static GUID pad_discrimination[4];// 各デバイスの識別子を格納
	// 既に取得済みのデバイスだった場合はもう一回実行する
	for (int i = 0; i < padNo; i++)
	{
		if (pad_discrimination[i] == pdidInstance->guidInstance)
			return DIENUM_CONTINUE;
	}

	HRESULT hr;
	LPDIRECTINPUTDEVICE8 di;
	hr = dInpGP->CreateDevice(pdidInstance->guidInstance, &di, NULL);
	dInputDeviceGP[padNo] = di;
	if (FAILED(hr)) return DIENUM_CONTINUE;
	// デバイスの識別子を保存
	pad_discrimination[padNo] = pdidInstance->guidInstance;
	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 10000;
	diprg.lMax = 0 + 10000;
	hr = dInputDeviceGP[padNo]->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

GamePad::GamePad(){
}



GamePad::~GamePad(){

}

void GamePad::Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput){
	dInpGP = dInput;
	for (padNo = 0; padNo < PAD_MAX; padNo++){
		dInputDeviceGP[padNo] = NULL;
		dInpGP->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
			NULL, DIEDFL_ATTACHEDONLY);

		if (dInputDeviceGP[padNo] != NULL){
			dInputDeviceGP[padNo]->SetDataFormat(&c_dfDIJoystick);
			dInputDeviceGP[padNo]->SetCooperativeLevel(d3d11User->m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
			dInputDeviceGP[padNo]->GetCapabilities(&dInputDevCapsGP);
			dInputDeviceGP[padNo]->EnumObjects(EnumAxesCallback, (VOID*)d3d11User->m_hWnd, DIDFT_AXIS);
		}
	}

	memset(js, 0, sizeof(DIJOYSTATE)* PAD_MAX);
	memset(oldJs, 0, sizeof(DIJOYSTATE)* PAD_MAX);
}

void GamePad::WindowActiveCheck(){
	HRESULT hr;
	for (padNo = 0; padNo < PAD_MAX; padNo++){
		if (dInputDeviceGP[padNo] != NULL){
			hr = dInputDeviceGP[padNo]->Poll();
			if (FAILED(hr)){
				hr = dInputDeviceGP[padNo]->Acquire();
				while (hr == DIERR_INPUTLOST){
					hr = dInputDeviceGP[padNo]->Acquire();
				}
			}
			oldJs[padNo] = js[padNo];
			dInputDeviceGP[padNo]->GetDeviceState(sizeof(DIJOYSTATE), &js[padNo]);
		}
	}
}

Vector3 GamePad::LeftStick(int padNo_){
	return vector3(abs(js[padNo_].lX) < 1000 ? 0 : (float)js[padNo_].lX, 0, abs(js[padNo_].lY) < 1000 ? 0 : (float)js[padNo_].lY);
}

Vector3 GamePad::RightStick(int padNo_){
	return vector3(abs(js[padNo_].lZ) < 1000 ? 0 : (float)js[padNo_].lZ, abs(js[padNo_].lRz) < 1000 ? 0 : -(float)js[padNo_].lRz, 0);
}

Vector3 GamePad::PovVec(int padNo_){
	return vector3((float)js[padNo_].lX, (float)js[padNo_].lY, 0);
}

bool GamePad::KeyDown(int padNo_,const UINT KeyCode, bool trigger)const{
	if (!trigger)
		return (js[padNo_].rgbButtons[KeyCode] & 0x80) == 0 ? false : true;

	return ((js[padNo_].rgbButtons[KeyCode] & 0x80) && !(oldJs[padNo_].rgbButtons[KeyCode] & 0x80)) == 0 ? false : true;
	
}

bool GamePad::KeyUp(int padNo_, const UINT KeyCode)const{
	return (!(js[padNo_].rgbButtons[KeyCode] & 0x80) && (oldJs[padNo_].rgbButtons[KeyCode] & 0x80)) == 0 ? false : true;
}