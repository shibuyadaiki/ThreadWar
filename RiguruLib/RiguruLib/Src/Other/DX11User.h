#ifndef DX11USER_H
#define DX11USER_H



// ���C�u�����̒ǉ�
#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "d3dx9.lib" )   // �s�񉉎Z�֐��Ƃ��g������
#pragma comment( lib, "Shlwapi.lib" ) // PathFileExists �Ȃǂ� API �֐����g������
#pragma comment( lib, "winmm.lib" )   // timeGetTime ���g������
#pragma comment( lib, "Effects11.lib" )   // �G�t�F�N�g�̃��C�u����
#pragma comment( lib, "d3dcompiler.lib" ) // Effects11.lib ���g�p����̂ɕK
#pragma comment( lib, "dinput8.lib")
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )

// �g���@�\���g�p����Ă��܂� : �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂��B���̃��[�j���O��}������B
#pragma warning(disable: 4238)

// ��W���̊g���@�\���g�p����Ă��܂�: enum 'enum' �����薼�Ŏg�p����܂��B���̃��[�j���O��}������B
#pragma warning(disable: 4482)

#define STRICT              // �^�`�F�b�N�������ɍs��
#define WIN32_LEAN_AND_MEAN // �w�b�_�[���炠�܂�g���Ȃ��֐�������

// �C���N���[�h
#include <windows.h>
#include <stdio.h>
#include <d3dx11.h>
#include <d3dx9.h>    // D3DXVECTOR3�Ƃ��s�񉉎Z�֐��Ƃ��g������
#include <tchar.h>
#include <vector>
#include <shlwapi.h>  // PathFileExists �Ȃǂ� API �֐����g������
#include <mmsystem.h> // timeGetTime ���g������
#include <d3dx11effect.h>                 // �G�t�F�N�g�̃C���N���[�h�t�@�C��

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// �������J��
#define SAFE_RELEASE(x) if( x != NULL ){ x->Release(); x = NULL; }
#define SAFE_DESTROY(x) if( x != NULL ){ x->Destroy(); x = nullptr; }
#define SAFE_DELETE(x)  if( x != NULL ){ delete x;  x = NULL; }
#define SAFE_DELETE_ARRAY(x)  if( x != NULL ){ delete[] x;  x = NULL; }
#define SAFE_FREE(x)  if( x != NULL ){ free( x );  x = NULL; }

#endif