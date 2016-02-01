#include "KeyBoard.h"

KeyBoard::KeyBoard(){
}



KeyBoard::~KeyBoard(){

}

void KeyBoard::Initialize(D3D11USER* d3d11User, HINSTANCE hInst, LPDIRECTINPUT8 dInput){
	//DirectInputKeyBoard������
	if (FAILED(dInput->CreateDevice(GUID_SysKeyboard, &dInputDevice, NULL))){
		::MessageBox(NULL, _T("DirectInputKeyBoard�������G���["), _T("DirectInputKeyBoard�������G���["), MB_OK);
	}
	
	//DirectInputSetFormat
	dInputDevice->SetDataFormat(&c_dfDIKeyboard);
	
	//DirectInputsetCooperatilveLevel �P�c�̓�͔�A�N�e�B�u�̎��ɑ��삪�����Ȃ��Ȃ����Ă���
	dInputDevice->SetCooperativeLevel(d3d11User->m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// �����[�h��ݒ�i���Βl���[�h�ɐݒ�j
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	//diprop.dwData       = DIPROPAXISMODE_ABS; // ��Βl���[�h�̏ꍇ
	dInputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	ZeroMemory(keyBoardOldBuf, sizeof(keyBoardOldBuf));
	ZeroMemory(keyBoardNowBuf, sizeof(keyBoardNowBuf));
	dInputDevice->Acquire();
}

void KeyBoard::WindowActiveCheck(){
	//���̃L�[����1�t���[���O�ɃR�s�[
	memcpy(keyBoardOldBuf, keyBoardNowBuf, sizeof(keyBoardNowBuf));
	//���̃L�[���N���A
	ZeroMemory(keyBoardNowBuf, sizeof(keyBoardNowBuf));
	oldDim = nowDim;
	//�f�o�C�X����A�N�e�B�u�ɂȂ������̃o�O�΍�
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