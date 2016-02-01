#include "Sound.h"
#include "../Other/Device.h"
Sound::Sound(){
}

Sound::~Sound(){
	Stop();
	for (auto& i : g_Buffers){
		i->Release();
	}
	g_Buffers.clear();
	SAFE_RELEASE(g_Sound);
	SAFE_DELETE(waveData);
}

//音楽のロード
void Sound::Load(TCHAR* FileName, int bufferSize){
	g_Sound = NULL;
	//サウンドデバイス作成
	DirectSoundCreate8(NULL, &g_Sound, NULL);
	//強調レベルのセット
	g_Sound->SetCooperativeLevel(Device::GetInstance().Getd3d11User()->m_hWnd, DSSCL_NORMAL);

	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
	waveData = NULL;
	dataSize = 0;

	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(FileName, &mmioInfo, MMIO_READ);
	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
	}
	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		mmioClose(hMmio, 0);
	}
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&waveFormat, fmsize);
	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	dataSize = dataChunk.cksize;
	waveData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)waveData, dataChunk.cksize);

	if (size != dataChunk.cksize) {
		delete[] waveData;
	}
	mmioClose(hMmio, 0);

	//バッファ作成
	DSBUFFERDESC	desc = {};			// セカンダリバッファ作成用設定
	// チャンネル数での分岐、モノラルは1チャンネル、ステレオは2チャンネル
	//DSBCAPS_GROBALFOCUS 非アクティブ時再生
	if (waveFormat.nChannels == 1){
		desc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;
		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else{
		desc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLPAN | DSBCAPS_STATIC;
		// | DSBCAPS_CTRLFX;	エフェクトを追加すると Duplicate できない
		desc.guid3DAlgorithm = GUID_NULL;
	}
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwBufferBytes = dataSize;			// 音データサイズ指定
	desc.lpwfxFormat = &waveFormat;		// フォーマット指定

	//プライマリバッファをセカンダリバッファへコピー
	IDirectSoundBuffer *ptmpBuf = NULL;
	g_Sound->CreateSoundBuffer(&desc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&g_Buffer);
	ptmpBuf->Release();

	// セカンダリバッファにWaveデータ書き込み
	unsigned char* block1 = NULL;
	unsigned char* block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;
	if (DS_OK == g_Buffer->Lock(0, dataSize, (void**)&block1, &blockSize1, (void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER)) {
		memcpy(block1, waveData, dataSize);
		g_Buffer->Unlock(block1, blockSize1, block2, 0);
	}
	g_Buffers.resize(bufferSize,nullptr);
	for (int i = 0; i < bufferSize;i++)
	g_Sound->DuplicateSoundBuffer(g_Buffer, reinterpret_cast<LPDIRECTSOUNDBUFFER*>(&g_Buffers[i]));
}

//音楽再生
void Sound::Play(bool loop){
	//最初から再生
	//g_Buffer->SetCurrentPosition(0);
	//予約　優先度　再生方法
	for (auto& i : g_Buffers){
		unsigned long dWard;
		i->GetStatus(&dWard);
		if ((dWard & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING){
			continue;
		}
		else{
			i->Play(0, 0, loop);
			break;
		}
	}
}

//音楽停止
void Sound::Stop(){
	for (auto& i:g_Buffers)
	i->Stop();
}
//0～100までで調整可能
void Sound::SetVolume(LONG volume){
	LONG v = (volume * 100) - 10000;
	for (auto& i : g_Buffers)
	i->SetVolume(v);
}