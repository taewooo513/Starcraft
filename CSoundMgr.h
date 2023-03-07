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

	System* _system;   //�뷡�� ������ �����뷡 ��� ���� ���, ���带 ���ְ� ���ִ� ���
	Sound** _sound;      //����, ���, �ݺ� ����� ���
	Channel** _channel;   //���忡 ���� �޸� ����, ����� �����⿡�� �������, �Ҹ��� ��� ��ġ�������� �鸮�� ���� �����ϴ� ��� ä�ΰ� ����� �ݵ�� 1��1 ��Ī�ؾ���

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