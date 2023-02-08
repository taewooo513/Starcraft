#include "stdafx.h"
#include "VideoManager.h"
void VideoManager::CreateVideo(string key, string path)
{
	HWND hWndVideo;
	hWndVideo = MCIWndCreate(_hWnd, hInst, MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD, path.c_str());
	MoveWindow(hWndVideo, 0, 0, WINSIZE_X, WINSIZE_Y, NULL);
	videoHwnds.insert(make_pair(key, hWndVideo));
}

void VideoManager::Play(string key)
{
	auto find = videoHwnds.find(key);
	cv::Mat img;
	if (find != videoHwnds.end())
	{
		MCIWndPlay(find->second);
	}
	return;
}
