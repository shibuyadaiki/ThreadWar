#include "Audio.h"

Audio::Audio(){
}

Audio::~Audio(){

}

//�a�f�l�����[�h
void Audio::LoadBGM(BGM_ID id, TCHAR* FileName){
	bgm[id].Load(FileName,1);
	bgmId.push_back(id);
}
//�r�d�����[�h(bufferCount �����Đ���������)
void Audio::LoadSE(SE_ID id, TCHAR* FileName, int bufferCount){
	se[id].Load(FileName, bufferCount);
	seId.push_back(id);
}

//�a�f�l�Đ�
void Audio::PlayBGM(BGM_ID id, bool loop){
	bgm[id].Play(loop);
}
//�r�d�Đ�
void Audio::PlaySE(SE_ID id){
	se[id].Play();
}

//�a�f�l��~
void Audio::StopBGM(BGM_ID id){
	bgm[id].Stop();
}
//�r�d��~
void Audio::StopSE(SE_ID id){
	se[id].Stop();
}

//�a�f�l�{�����[���Z�b�g
void Audio::SetBGMVolume(int volume){
	for (auto& i : bgmId){
		bgm[i].SetVolume(volume);
	}
}
//�r�d�{�����[���Z�b�g
void Audio::SetSEVolume(int volume){
	for (auto& i : seId){
		se[i].SetVolume(volume);
	}
}