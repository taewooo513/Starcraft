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

typedef struct
{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE padding;
} RGBAbyte;


class MapReader
{
private:
	CImage* mapImg;
	float yoff = 0;

	float xoff = 0;

	TileSetData* tileSetData;
	TileSetData::CV5* cv5;
	TileSetData::VF4* vf4;
	TileSetData::VR4* vr4;
	TileSetData::VX4* vx4;
	TileSetData::WPE* wpe;


	ID2D1DeviceContext* m_context;
public:
	MapReader() {}
	~MapReader() {}
	void Init(ID2D1DeviceContext* context);
	void MapRender();
	void UIMapRender();

};

