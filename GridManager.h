#pragma once
#include "singleton.h"
class Object;
class ObjectGrid;
class  GridManager : public Singleton<GridManager>
{
public:
	GridManager() {}
	~GridManager();

	struct tileNum
	{
		int regionsIds;
		float x, y;
		int isBuildTag;
		vector<int> nearRegionIds;
		float fogTag = 0;
	};

	struct Regions
	{
		tileNum regionsIds[512][512];
	};

public:
	tileNum regionsTile[512][512];

	vector<ObjectGrid*> objectGrid;
public:
	ObjectGrid* AddGrid(Object* _obj, float sizeX, float sizeY, float searchX, float searchY, float x, float y);
	void Init();
	void Update();
	void Render();
	void Astar();

	tileNum** GetRegionTile()
	{
		return (tileNum**)regionsTile;
	}
};

#define GRIDMANAGER GridManager::GetInstance() 