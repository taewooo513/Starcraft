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

struct MapRegions
{
	~MapRegions()
	{
		nearRegions.clear();
	}
	Vector2 pos;
	int regionId;
	RECT self = { 0,0,0,0 };
	map<int, vector<Vector2>> nearRegionPosition;
	vector<pair<float, MapRegions*>> nearRegions;
	bool openNode = false;
	int whereRegionId;
	void AddRegion(int x, int y)
	{
		if (x < self.left)
		{
			self.left = x;
		}
		if (x + 1 > self.right)
		{
			self.right = x + 1;
		}
		if (y < self.top)
		{
			self.top = y;
		}
		if (y + 1 > self.bottom)
		{
			self.bottom = y + 1;
		}
	}
};

struct tileNum
{
	int regionsIds;
	vector<int> nearRegionIds;
	vector<Vector2> nearRegionPosition;

};

struct Regions
{
	tileNum regionsIds[512][512];
};

class MapReader
{
private:
	CImage* mapImg;
	float yoff = 0;

	float xoff = 0;
	int index = 0;
	TileSetData* tileSetData;
	TileSetData::CV5* cv5;
	TileSetData::VF4* vf4;
	TileSetData::VR4* vr4;
	TileSetData::VX4* vx4;
	TileSetData::WPE* wpe;
	ID2D1DeviceContext* m_context;
public:
	Regions* region;
	int miniTiles[128 * 4][128 * 4];
	vector<MapRegions*> mapRegions;
	MapReader() {}
	~MapReader() {}
	void Init(ID2D1DeviceContext* context);
	void MapRender(Vector2 mapPos);
	void UIMapRender();
	void MapRegionSetting();
	void RenderLine();
	void Release();
};

