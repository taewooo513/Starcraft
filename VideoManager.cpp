#include "stdafx.h"
#include "VideoManager.h"
//#include <Digitalv.h>
//#include <dshow.h>
void VideoManager::CreateVideo(string key, string path)
{
	//UINT wDeviceID;
	//DWORD dwReturn;
	//MCI_DGV_OPEN_PARMS    mciOpen;
	//mciOpen.lpstrElementName = (LPSTR)path.c_str();  // Set the filename.
	//mciOpen.dwStyle = WS_CHILD;            // Set the style. 
	//mciOpen.hWndParent = _hWnd;             // Give a window handle. 
	//HRESULT hr = mciSendString("open digitalvideo", NULL, 0, NULL) ;
	//cout << hr;
	//if (mciSendCommand(0, MCI_OPEN,
	//	(DWORD)(MCI_OPEN_ELEMENT | MCI_DGV_OPEN_PARENT | MCI_DGV_OPEN_32BIT),
	//	(DWORD)(LPSTR)&mciOpen) == 0)
	//{
	//	// Open operation is successful. Continue. 
	//}
}

void VideoManager::Play(string key)
{
	return;
}
