#include "Input.h"
#include "../Math/Vector3.h"
Input::Input(){
}



Input::~Input(){

}

void Input::Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput){
	k.Initialize(d3d11User, hInst, dInput);
	m.Initialize(d3d11User, hInst, dInput);
	g.Initialize(d3d11User, hInst, dInput);
}

void Input::WindowActiveCheck(RECT rc){
	k.WindowActiveCheck();
	m.WindowActiveCheck(rc);
	g.WindowActiveCheck();
}

bool Input::KeyDown(const UINT KeyCode, bool trigger)const{
	return k.KeyDown(KeyCode, trigger);
}

bool Input::KeyUp(const UINT KeyCode)const{
	return k.KeyUp(KeyCode);
}

bool Input::MouseButtonDown(INPUTMOUSEBUTTON id, bool trriger){
	return m.MouseButtonDown(id, trriger);
}

bool Input::MouseButtonDownTrigger(INPUTMOUSEBUTTON id){
	return m.MouseButtonDown(id, true);
}

bool Input::MouseButtonUp(INPUTMOUSEBUTTON id){
	return m.MouseButtonUp(id);
}

Vector3 Input::MouseVec(){
	return m.MouseVec();
}
Vector3 Input::LeftStick(int padNo_){
	return g.LeftStick(padNo_);
}
Vector3 Input::RightStick(int padNo_){
	return g.RightStick(padNo_);
}
Vector3 Input::PovVec(int padNo_){
	return g.PovVec(padNo_);
}
bool Input::GamePadButtonDown(int padNo_,const UINT keyCode, bool trriger){
	return g.KeyDown(padNo_, keyCode, trriger);
}
bool Input::GamePadButtonUp(int padNo_, const UINT keyCode){
	return g.KeyUp(padNo_, keyCode);
}