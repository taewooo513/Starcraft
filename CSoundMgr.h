#pragma once
#include "singleton.h"
constexpr auto soundBuffer = 40;
constexpr auto extraSoundChannel = 5;
constexpr auto totalSoundChannel = soundBuffer + extraSoundChannel;

using namespace FMOD;

class CSoundMgr : public Singleton <CSoundMgr>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;
	typedef map<string, Channel**> arrChannels;
	typedef map<string, Channel**>::iterator arrChannelIter;

	System* _system;   //노래가 끝나면 다음노래 재생 등의 기능, 사운드를 켜주고 꺼주는 기능
	Sound** _sound;      //퍼즈, 재생, 반복 등등의 기능
	Channel** _channel;   //사운드에 대한 메모리 버퍼, 사운드는 전방향에서 들려야함, 소리를 어디 위치에서부터 들리게 할지 선택하는 기능 채널과 사운드는 반드시 1대1 매칭해야함

	arrSounds _mTotalSounds;

public:
	CSoundMgr();
	~CSoundMgr() { release(); }
	HRESULT init(void);
	void release(void);
	void update(void);

	void addSound(string keyName, string soundName, bool backGround, bool loop);

	void play(string keyName, float volume);
	void pause(string keyName);
	void stop(string keyName);
	void resume(string keyName);
	bool isPause(string key);
	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);

};

#define SOUNDMANAGER CSoundMgr::GetInstance()