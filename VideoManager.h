#pragma once
#include "singleton.h"
class VideoManager : public Singleton<VideoManager>
{
public:
	VideoManager() {}
	~VideoManager() {}
public:
	map<string, cv::VideoCapture > videoHwnds;
	void CreateVideo(string key, string path);
	void Play(string key);
};

#define VIDEOMANAGER VideoManager::GetInstance()