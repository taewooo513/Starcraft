#include "Stdafx.h"
#include "TimerManager.h"

HRESULT TimerManager::Init(void)
{
	_timer = new Timer;
	_timer->init();
	return S_OK;
}

void TimerManager::Release(void)
{
	if (_timer != nullptr)
	{
		SAFE_DELETE(_timer);
		_timer = nullptr; // 문제 생겨서 꺼지면  타이머에는 값이 들어가 있으므로 시동되자마자 공백만들어버림
	}
}

void TimerManager::Update(float lock)
{
	if (_timer != nullptr)
	{
		_timer->tick(lock);
	}
}

void TimerManager::Render()
{
	wstring wstr;
	cout << "ElapsedTime: " + to_string(_timer->getFrameRate()) << endl;
//#ifdef _DEBUG 
//	if (_timer != nullptr)
//	{
//		wstr = L"FramePerSec: " + to_wstring();
//		IMAGEMANAGER->D2dTextOut(wstr, 0, 0, { 255,255,255,1 });
//
//		
//		IMAGEMANAGER->D2dTextOut(wstr, 0, 20, { 255,255,255,1 });
//
//		wstr = L"WorldTime: " + to_wstring(_timer->getWorldTime());
//		IMAGEMANAGER->D2dTextOut(wstr, 0, 40, { 255,255,255,1 });
//	}
//#else
//	wstr = to_wstring(_timer->getFrameRate());
//	IMAGEMANAGER->D2dTextOut(wstr, 0, 0, { 255,255,255,1 });
//#endif // _DEBUG

}
