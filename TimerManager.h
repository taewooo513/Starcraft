#pragma once
#include"singleton.h"
#include"CTimer.h"

class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* _timer;
public:

	HRESULT Init(void);
	void Release(void);
	void Update(float lock = 0.0f);
	void Render();
	// ������ �̹����� �̷��� �������ʿ䰡 ���� ������ ���ӳ�� ���x
	// �׷��Ƿ� �ڵ� �Ծุ ���缭 ����

	inline float GetWorldTime(void) const { return _timer->getWorldTime(); }
	inline float GetElapsedTime(void) const { return _timer->getElapsedTime(); }

	TimerManager() {}
	~TimerManager() {}

};

#define TIMERMANAGER TimerManager::GetInstance()