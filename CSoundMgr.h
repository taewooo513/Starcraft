#pragma once
#include <dsound.h>
#include <dinput.h>

#pragma comment(lib, "dsound.lib")

class CSound;
class CSoundMgr : public Singleton<CSoundMgr>
{
private:
	LPDIRECTSOUND8	m_pSound;	// ����ī�� ��ǥ ��ü
	CSound* m_pBGM;		// ���� ������ BGM Sound
	map<string, CSound*> m_sounds;
public:
	CSoundMgr();
	~CSoundMgr();
	int init(HWND hwnd);
	LPDIRECTSOUND8 GetSoundDevice() { return m_pSound; }
	void RegisterToBGM(CSound* _pSound);

	CSound* AddSound(string key, string path);
	CSound* FindSound(string key);
};

#define SOUNDMANAGER CSoundMgr::GetInstance()