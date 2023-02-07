#pragma once
#include "GridManager.h"
#include <stack>
class Object;
class ObjectGrid
{
private:
	Object* obj;
	Vector2 m_size;
	Vector2 m_collisionGridSize;
	map<pair<int, int>, Vector2> openNodeCheck;
	vector<pair<Vector2, Vector2>> vectors;
public:
	ObjectGrid() {}
	~ObjectGrid() {}
public:
	float lastX, lastY;
	float x, y;
	void Astar();
	Vector2 destPos;
	void Init(Object* obj, Vector2 collisionGridSize, Vector2 gridSize, float x, float y);
	void Update();
	void Render();
	void Release();
	int nowTileRegionId = 0;
	stack<Vector2> moveStack2;
	struct comp {
		bool operator()(pair<pair<float, float>, GridManager::tileNum>A, pair<pair<float, float>, GridManager::tileNum>B)
		{
			if (A.first.second > B.first.second)
				return true;
			else if (A.first.second == B.first.second)
				return false;
			return false;
		}
	};

};

