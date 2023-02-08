#pragma once
#include <SDKDDKVer.h>
#pragma comment (lib, "msimg32.lib") //마이크로소프트(ms) 이미지(img) 32비트(32) 라이브러리


#include <wincodecsdk.h>
#pragma comment(lib,"windowscodecs.lib")
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include <initguid.h>
#include <windows.h>
#include <functional>
#include <locale>
#include <codecvt>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono> 
#include <random>
#include <time.h>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <unordered_map> 
#include <time.h>
#include <string>
#include <queue>
#include <bitset>
#include <wrl.h>
#include <wrl/client.h>
#include <d2d1effects_2.h>

#include<CoreWindow.h>
#include <Vfw.h>
#pragma comment (lib, "Vfw32.lib")

#include <opencv2/opencv.hpp>
#pragma comment (lib,"opencv_world460.lib")


#pragma region D2d1FactoryHeader
#pragma comment (lib, "msimg32.lib") 

//#pragma comment (lib, "winRT.lib") 
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>
#include <d2d1effectauthor.h>  
#include <d2d1effecthelpers.h>

//#include "./Storm/SFmpqapi.h"
//#include "./Storm/SFmpq_static.h"

#pragma comment(lib , "Dwrite.lib")
#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "d3d11.lib" )

#include <wincodecsdk.h>
#pragma comment(lib,"windowscodecs.lib")
#pragma endregion

//#pragma comment(lib,"./Storm/SFmpq.lib")
//#pragma comment(lib,"./Storm/SFmpq_static.lib")

using namespace std;

enum ObjectTag
{
	eObject,
	eNPC,
	eSummons,
	ePlayerProjectile,
	eEnemy,
	eItem,
	eBoss,
	ePlayer,
	eBossObject,
	eBossObject2,
	eEndTag
};

enum ItemTag
{
	eCaerleonSword,
	eEnd
};

struct StructureData
{
	string key = "";
	float x = 0;
	float y = 0;
	bool isBack = false;
	int page = 0;
};

enum class eSkulSpecies
{
	eBasic,
	ePredator,
	eDestroyer,
	eGambler,
	Empty
};

#define WIN_NAME "금강상태"
#define WINSTYLE WS_CAPTION | WS_SYSMENU
#define WINSTART_X 0
#define WINSTART_Y 0
#define WINSIZE_X 640.f * 1.625
#define WINSIZE_Y 480.f * 1.685


extern float g_TimeScale;

#define TIME_SCALE g_TimeScale


#define SAFE_DELETE(p)       {if(p){delete(p); (p) = nullptr;}}   //안전하게 초기화
#define SAFE_DELETE_ARRAY(p) {if(p){delete[] (p); (p)=nullptr;}}  //복수 객체일 경우
#define SAFE_RELEASE(p)      {if(p) {(p)->release();(p)=nullptr;}

extern HWND _hWnd;
extern POINT _ptMouse;
#include "TimerManager.h"

struct Vector2
{
	float x;
	float y;
	Vector2 operator - (Vector2 lvec)
	{
		return { x - lvec.x,y - lvec.y };
	}

	Vector2 operator / (float d)
	{
		return { x / d, y / d };
	}

	Vector2 operator + (Vector2 lvec)
	{
		return { x + lvec.x,y + lvec.y };
	}

	Vector2 operator * (float lvec)
	{
		return { x * lvec,y * lvec };
	}
	bool operator == (Vector2 lvec)
	{
		return { x == lvec.x && y == lvec.y };
	}
};
struct MoveNode
{
	Vector2 pos;
	int regionId;
};

#define DELTA_TIME g_TimeScale *  TimerManager::GetInstance()->GetElapsedTime()

#include "ObjectManager.h"
#include "GridManager.h"
#include "KeyManager.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "IOFillManager.h"
#include "Utility.h"
#include "EffectManager.h"
#include "CSoundMgr.h"
#include "VideoManager.h"
using namespace Microsoft::WRL;
//inline static float GetDeltaTime()
//{
//	return;
//}

typedef struct
{
	ID2D1Bitmap* bitmap;
	typedef struct
	{
		struct CV5Data
		{
			unsigned char Someting[20];
			unsigned short MegaTileIndex[16];
		} group[4096];
	} CV5;

	typedef struct
	{
		struct VF4Data
		{
			enum flags
			{
				eWalkBlock = 0x0001,
				eMidBlock = 0x0002,
				eHighBlock = 0x0004,
				eBlocksView = 0x0008,
				eRamp = 0x0010,
			};
			unsigned short flag[16];
		} pVX4Data[65536];
	} VF4;

	typedef struct
	{
		struct VX4Data
		{
			unsigned short VR4Index[16];
		} data[65536];
	} VX4;

	typedef struct
	{
		struct VR4Data
		{
			unsigned char color[64];
		} pVR4Data[32768];
	} VR4;

	typedef struct
	{
		struct WPEData
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char padding;
		} data[256];
	} WPE;

	CV5* cv5;
	VF4* vf4;
	VR4* vr4;
	VX4* vx4;
	WPE* wpe;
} TileSetData;

extern ID2D1Device* g_device;
extern ComPtr<ID3D11Device> g_d3dDevice;

#define DEVICE g_device

extern HINSTANCE hInst;

