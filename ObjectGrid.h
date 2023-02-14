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
	int gridTag = 0;
	bool isDestory = false;
	float lastX, lastY;
	float x, y;
	void Astar(float searchSizeX, float searchSizeY);
	Vector2 destPos;
	void Init(Object* obj, Vector2 collisionGridSize, Vector2 gridSize, float x, float y);
	void Update();
	void Render();
	void Release();
	int nowTileRegionId = 0;
	stack<Vector2> moveStack2;
	struct comp {
		bool operator()(pair<Vector2, pair<float, float>> A, pair<Vector2, pair<float, float>> B)
		{
			if (A.second.first + A.second.second > B.second.first + B.second.second)
				return true;
			return false;
		}
	};
	struct comp2 {
		bool operator()(pair<pair<float, float>, Vector2> A, pair<pair<float, float>, Vector2 > B)
		{
			if (A.first.first + A.first.second > B.first.first + B.first.second)
				return true;
			return false;
		}
	};
};

