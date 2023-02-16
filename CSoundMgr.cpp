#include "Stdafx.h"
#include "CSoundMgr.h"

CSoundMgr::CSoundMgr() : _system(nullptr),
_channel(nullptr),
_sound(nullptr)
{
}

HRESULT CSoundMgr::init(void)
{
	// 사운드 시스템 생성
	System_Create(&_system);

	_system->init(totalSoundChannel, FMOD_INIT_NORMAL, 0);

	_sound = new Sound * [totalSoundChannel];
	_channel = new Channel * [totalSoundChannel];

	// 메모리 한번 정리
	memset(_sound, 0, sizeof(Sound*) * (totalSoundChannel));
	memset(_channel, 0, sizeof(Channel*) * (totalSoundChannel));

	return S_OK;
}

void CSoundMgr::release(void)
{
	// 사운드 삭제
	if (_channel != nullptr || _sound != nullptr)
	{
		for (int i = 0; i < totalSoundChannel; i++)
		{
			if (_channel != nullptr)
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != nullptr)
			{
				if (_sound != nullptr) _sound[i]->release();
			}
		}
	}

	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	// 시스템 해제
	if (_system != nullptr)
	{
		_system->release();
		_system->close();
	}
}

void CSoundMgr::update(void)
{
	_system->update();
}

void CSoundMgr::addSound(string keyName, string soundName, bool backGround, bool loop)
{
	if (loop)
	{
		if (backGround)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	else
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void CSoundMgr::play(string keyName, float volume)
{
	arrSoundIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_system->playSound(*iter->second, nullptr,false, &_channel[count]);

			_channel[count]->setVolume(volume);

			break;
		}
	}


}



void CSoundMgr::stop(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();

			break;
		}
	}
}

void CSoundMgr::pause(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);

			break;
		}
	}
}
void CSoundMgr::resume(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);

			break;
		}
	}
}

bool CSoundMgr::isPlaySound(string keyName)
{
	arrSoundIter iter = _mTotalSounds.begin();
	bool isPlay;
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);

			break;
		}
	}
	return isPlay;
}

bool CSoundMgr::isPauseSound(string keyName)
{
	bool isPause;
	int count = 0;

	arrSoundIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPause);
			break;
		}
	}

	return isPause;
}

