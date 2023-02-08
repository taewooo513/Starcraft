#include "stdafx.h"
#include "VideoManager.h"

void VideoManager::CreateVideo(string key, string path)
{
	cv::VideoCapture cap(path);
	videoHwnds.insert(make_pair(key, cap));
	if (!cap.isOpened()) {
		std::cout << "Can't open video !!!" << std::endl;
	}
}

void VideoManager::Play(string key)
{
	auto find = videoHwnds.find(key);
	cv::Mat img;
	if (find != videoHwnds.end())
	{
		find->second >> img;
	}
	return;
}
