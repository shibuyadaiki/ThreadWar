#pragma once
#include "../Other/DX11User.h"
#include <dsound.h>
#include <mmsystem.h>

class Sound{
public:
	Sound();
	~Sound();
	//音楽のロード
	void Load(TCHAR* FileName,int bufferSize);
	//音楽再生
	void Play(bool loop = false);
	//音楽停止
	void Stop();
	//0～100までで調整可能
	void SetVolume(LONG volume);

private:
	IDirectSound8 *g_Sound;

	WAVEFORMATEX waveFormat;
	char* waveData; 
	int dataSize;
	IDirectSoundBuffer8 *g_Buffer;
	std::vector<IDirectSoundBuffer8*> g_Buffers;
};