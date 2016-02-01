#pragma once
#include "../Other/DX11User.h"
#include <dinput.h>
#include "../Other/D3D11User.h"
#include "../Math/Vector3.h"

#define PAD_MAX 4

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

namespace GAMEPADKEY
{
	const UINT BUTTON_SQUARE = 0;
	const UINT BUTTON_CROSS = 1;
	const UINT BUTTON_CURCLE = 2;
	const UINT BUTTON_TRIANGLE = 3;
	const UINT BUTTON_L1 = 4;
	const UINT BUTTON_R1 = 5;
	const UINT BUTTON_L2 = 6;
	const UINT BUTTON_R2 = 7;
	const UINT BUTTON_SELECT = 8;
	const UINT BUTTON_START = 9;
	const UINT BUTTON_L3 = 10;
	const UINT BUTTON_R3 = 11;
	const UINT BUTTON_HOME = 12;
}

class GamePad{
public:
	GamePad();
	~GamePad();
	void Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput);
	void WindowActiveCheck();
	Vector3 LeftStick(int padNo_);
	Vector3 RightStick(int padNo_);
	Vector3 PovVec(int padNo_);
	bool KeyDown(int padNo_,const UINT KeyCode, bool trigger = false)const;
	bool KeyUp(int padNo_,const UINT KeyCode)const;
private:
	DIJOYSTATE js[PAD_MAX];
	DIJOYSTATE oldJs[PAD_MAX];
public:
};
