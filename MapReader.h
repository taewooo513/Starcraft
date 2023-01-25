#pragma once

template <class T>
class arr2D
{
public:
	T* _p;
	const int _pitch;
	arr2D(T* p, int pitch) : _p(p), _pitch(pitch) {}
	T* operator[] (int index) { return _p + (_pitch * index); }
};

class MapReader
{
private:
	CImage* mapImg;
	float yoff = 0;

	float xoff = 0;
	
	typedef struct
	{
		struct CV5Data
		{
			unsigned char Someting[20];
			unsigned short MegaTileIndex[16];
		} pCV5Data[4096];
	} CV5;

	typedef struct
	{
		struct VX4Data
		{
			unsigned short VR4Index[16];
		} pVX4Data[65536];
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
		struct WREData
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char padding;
		} pWREData[256];
	} WRE;
	struct RGBAbyte
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	CV5* cv5Data;
	VX4* vx4Data;
	VR4* ve4Data;
	WRE* wreData;

	ID2D1Bitmap* bitmap;
	ID2D1DeviceContext* m_context;
public:
	MapReader() {}
	~MapReader() {}
	void Init(ID2D1DeviceContext* context);
	void MapRender();
};

