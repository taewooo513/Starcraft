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
		bool isBuildAble;
		vector<int> nearRegionIds;
	};

	struct Regions
	{
		tileNum regionsIds[512][512];
	};

	tileNum regionsTile[512][512];
public:

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