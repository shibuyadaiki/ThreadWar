#pragma once
#include "../Other/DX11User.h"
#include <dsound.h>
#include <mmsystem.h>

class Sound{
public:
	Sound();
	~Sound();
	//���y�̃��[�h
	void Load(TCHAR* FileName,int bufferSize);
	//���y�Đ�
	void Play(bool loop = false);
	//���y��~
	void Stop();
	//0�`100�܂łŒ����\
	void SetVolume(LONG volume);

private:
	IDirectSound8 *g_Sound;

	WAVEFORMATEX waveFormat;
	char* waveData; 
	int dataSize;
	IDirectSoundBuffer8 *g_Buffer;
	std::vector<IDirectSoundBuffer8*> g_Buffers;
};