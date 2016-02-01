#pragma once
#include "../Other/DX11User.h"
#include <dinput.h>
#include "../Other/D3D11User.h"

struct Vector3;

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

enum INPUTMOUSEBUTTON{
	LEFT_BUTTON,
	RIGHT_BUTTON,
	CENTER_BUTTON
};

class Mouse{
public:
	Mouse();
	~Mouse();
	void Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput);
	void WindowActiveCheck(RECT rc);
	bool MouseButtonDown(INPUTMOUSEBUTTON id, bool trriger = false);
	bool MouseButtonUp(INPUTMOUSEBUTTON id);
	Vector3 MouseVec();
private:
	LPDIRECTINPUTDEVICE8 dInputDevice;
	DIMOUSESTATE2 nowDim, oldDim;
	bool activeFlag = true;
public:
};