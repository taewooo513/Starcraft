#pragma once

class Timer
{

private:
	float		_timeScale;   // ����ð� ����
	float		_timeElapsed; // ������ �ð��� ����ð��� �����
	bool		_isHardware;   // ���� Ÿ�̸� ���� ����

	__int64		_curTime; //  ����ð� __int 32 or 64(�p��Ʈ ����ϴ���) �������Ϸ� �̾Ƴ����� 64 ,32 ���� __int64 32��Ʈ�� 64�� ������ �ٲ��ش� __ <- ǥ�ؾ�� �ƴ϶�°�
	__int64		_lastTime; // �����ð�
	__int64		_periodFrequency; // �ð� �ֱ�

	/*

	__int64 vs long

	- int vs lonng
	32��Ʈ���� ���� 4��Ʈ -> 64��Ʈ�������� 8��Ʈ
	*/

	unsigned long	_frameRate;  //(�ʴ������Ӽ�)
	unsigned long   _FPSFrameCount; //fps ī��Ʈ
	float			_FPSTimeElapsed; //FPS �ð� �����
	float			_worldTime;		 // ������ ���۵� �� ����� ��ü�ð�


public:
	HRESULT init(void);
	void tick(float lockFPS = 0.0f);


	//���� FPS
	unsigned long getFrameRate(char* str = nullptr) const; //?
	//������ �ð��� ���� �ð��� �����
	inline float getElapsedTime(void) const { return _timeElapsed; } //?
	// ��ü ��� �ð�
	inline float getWorldTime(void) const { return _worldTime; } //?
	// ����Լ��� ���� ����Ʈ�� �ڷ� ������ -> ����Լ��� ���ȭ�� �ǹ�
	// �� �� �Լ������� ��� ������ �� ������ ���´�.
	// �� ��������� ����� �� �ִ� ��ȸ�� ���� ������ ���� ���� ���� �Ұ�
	// �� ������ x
	// �ܺο��� �������� ���ϰ� �ϱ����� ������

	// Timer -> Timer manger�� �尥��

	// ���� -> ���Ӿ��迡���� UPDATE�� �Ⱦ� (������ ������� ���� ���ǹ��� ������� ���ϴ°�쵵����)
	// ���� Ÿ�̸ӷ� ������� �����ϰ� �̾Ƴ��� �ִ�



public:
	Timer() {};
	~Timer() {};
};

