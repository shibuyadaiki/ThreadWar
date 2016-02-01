#pragma once
#include "../Other/DX11User.h"
#include "../Other/D3D11User.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"

class Input{
public:
	Input();
	~Input();
	void Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput);
	void WindowActiveCheck(RECT rc);
	bool KeyDown(const UINT KeyCode, bool trigger = false)const;
	bool KeyUp(const UINT KeyCode)const;
	bool MouseButtonDown(INPUTMOUSEBUTTON id, bool trriger = false);
	bool MouseButtonDownTrigger(INPUTMOUSEBUTTON id);
	bool MouseButtonUp(INPUTMOUSEBUTTON id);
	Vector3 MouseVec();
	Vector3 LeftStick(int padNo_);
	Vector3 RightStick(int padNo_);
	Vector3 PovVec(int padNo_);
	bool GamePadButtonDown(int padNo_, const UINT keyCode, bool trriger = false);
	bool GamePadButtonUp(int padNo_, const UINT keyCode);
private:
	KeyBoard k;
	Mouse m;
	GamePad g;
public:
};