#ifndef DX11USER_H
#define DX11USER_H



// ライブラリの追加
#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "d3dx9.lib" )   // 行列演算関数とか使うため
#pragma comment( lib, "Shlwapi.lib" ) // PathFileExists などの API 関数を使いたい
#pragma comment( lib, "winmm.lib" )   // timeGetTime を使いたい
#pragma comment( lib, "Effects11.lib" )   // エフェクトのライブラリ
#pragma comment( lib, "d3dcompiler.lib" ) // Effects11.lib を使用するのに必
#pragma comment( lib, "dinput8.lib")
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )

// 拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます。このワーニングを抑制する。
#pragma warning(disable: 4238)

// 非標準の拡張機能が使用されています: enum 'enum' が限定名で使用されます。このワーニングを抑制する。
#pragma warning(disable: 4482)

#define STRICT              // 型チェックを厳密に行う
#define WIN32_LEAN_AND_MEAN // ヘッダーからあまり使われない関数を除く

// インクルード
#include <windows.h>
#include <stdio.h>
#include <d3dx11.h>
#include <d3dx9.h>    // D3DXVECTOR3とか行列演算関数とか使うため
#include <tchar.h>
#include <vector>
#include <shlwapi.h>  // PathFileExists などの API 関数を使いたい
#include <mmsystem.h> // timeGetTime を使いたい
#include <d3dx11effect.h>                 // エフェクトのインクルードファイル

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// メモリ開放
#define SAFE_RELEASE(x) if( x != NULL ){ x->Release(); x = NULL; }
#define SAFE_DESTROY(x) if( x != NULL ){ x->Destroy(); x = nullptr; }
#define SAFE_DELETE(x)  if( x != NULL ){ delete x;  x = NULL; }
#define SAFE_DELETE_ARRAY(x)  if( x != NULL ){ delete[] x;  x = NULL; }
#define SAFE_FREE(x)  if( x != NULL ){ free( x );  x = NULL; }

#endif