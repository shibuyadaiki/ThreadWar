#pragma once
#include "../Other/DX11User.h"
#include <dinput.h>
#include "../Other/D3D11User.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif
namespace INPUTKEY
{
	const UINT KEY_RIGHT = DIK_RIGHTARROW;
	const UINT KEY_LEFT = DIK_LEFTARROW;
	const UINT KEY_UP = DIK_UPARROW;
	const UINT KEY_DOWN = DIK_DOWNARROW;
	const UINT KEY_A = DIK_A;
	const UINT KEY_B = DIK_B;
	const UINT KEY_C = DIK_C;
	const UINT KEY_D = DIK_D;
	const UINT KEY_E = DIK_E;
	const UINT KEY_F = DIK_F;
	const UINT KEY_G = DIK_G;
	const UINT KEY_H = DIK_H;
	const UINT KEY_I = DIK_I;
	const UINT KEY_J = DIK_J;
	const UINT KEY_K = DIK_K;
	const UINT KEY_L = DIK_L;
	const UINT KEY_M = DIK_M;
	const UINT KEY_N = DIK_N;
	const UINT KEY_O = DIK_O;
	const UINT KEY_P = DIK_P;
	const UINT KEY_Q = DIK_Q;
	const UINT KEY_R = DIK_R;
	const UINT KEY_S = DIK_S;
	const UINT KEY_T = DIK_T;
	const UINT KEY_U = DIK_U;
	const UINT KEY_V = DIK_V;
	const UINT KEY_W = DIK_W;
	const UINT KEY_X = DIK_X;
	const UINT KEY_Y = DIK_Y;
	const UINT KEY_Z = DIK_Z;
	const UINT KEY_LSHIFT = DIK_LSHIFT;
	const UINT KEY_LCTRL = DIK_LCONTROL;
	const UINT KEY_SPACE = DIK_SPACE;
	const UINT KEY_1 = DIK_1;
	const UINT KEY_2 = DIK_2;
	const UINT KEY_3 = DIK_3;
	const UINT KEY_4 = DIK_4;
	const UINT KEY_5 = DIK_5;
	const UINT KEY_6 = DIK_6;
	const UINT KEY_TAB = DIK_TAB;
}

class KeyBoard{
public:
	KeyBoard();
	~KeyBoard();
	void Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput);
	void WindowActiveCheck();
	BYTE KeyDown(const UINT KeyCode,bool trigger = false)const;
	bool KeyUp(const UINT KeyCode)const;
private:
	LPDIRECTINPUTDEVICE8 dInputDevice;
	BYTE keyBoardNowBuf[256],keyBoardOldBuf[256];
	DIMOUSESTATE2 nowDim, oldDim;
public:
};