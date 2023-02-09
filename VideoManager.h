#pragma once

#include "singleton.h"
class VideoManager : public Singleton<VideoManager>
{
public:
	VideoManager() {}
	~VideoManager() {}
public:
	map<string, HWND > videoHwnds;
	void CreateVideo(string key, string path);
	void Play(string key);
};

#define VIDEOMANAGER VideoManager::GetInstance()