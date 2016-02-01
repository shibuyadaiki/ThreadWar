#include "Audio.h"

Audio::Audio(){
}

Audio::~Audio(){

}

//‚a‚f‚l‚ğƒ[ƒh
void Audio::LoadBGM(BGM_ID id, TCHAR* FileName){
	bgm[id].Load(FileName,1);
	bgmId.push_back(id);
}
//‚r‚d‚ğƒ[ƒh(bufferCount “¯Ä¶‚µ‚½‚¢”)
void Audio::LoadSE(SE_ID id, TCHAR* FileName, int bufferCount){
	se[id].Load(FileName, bufferCount);
	seId.push_back(id);
}

//‚a‚f‚lÄ¶
void Audio::PlayBGM(BGM_ID id, bool loop){
	bgm[id].Play(loop);
}
//‚r‚dÄ¶
void Audio::PlaySE(SE_ID id){
	se[id].Play();
}

//‚a‚f‚l’â~
void Audio::StopBGM(BGM_ID id){
	bgm[id].Stop();
}
//‚r‚d’â~
void Audio::StopSE(SE_ID id){
	se[id].Stop();
}

//‚a‚f‚lƒ{ƒŠƒ…[ƒ€ƒZƒbƒg
void Audio::SetBGMVolume(int volume){
	for (auto& i : bgmId){
		bgm[i].SetVolume(volume);
	}
}
//‚r‚dƒ{ƒŠƒ…[ƒ€ƒZƒbƒg
void Audio::SetSEVolume(int volume){
	for (auto& i : seId){
		se[i].SetVolume(volume);
	}
}