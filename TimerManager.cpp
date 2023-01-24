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
		_timer = nullptr; // ���� ���ܼ� ������  Ÿ�̸ӿ��� ���� �� �����Ƿ� �õ����ڸ��� ���鸸������
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
//#ifdef _DEBUG 
//	if (_timer != nullptr)
//	{
//		wstr = L"FramePerSec: " + to_wstring(_timer->getFrameRate());
//		IMAGEMANAGER->D2dTextOut(wstr, 0, 0, { 255,255,255,1 });
//
//		wstr = L"ElapsedTime: " + to_wstring(DELTA_TIME);
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
