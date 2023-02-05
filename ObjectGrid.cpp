#include "stdafx.h"
#include "ObjectGrid.h"
#include "Object.h"
#include "Unit.h"
void ObjectGrid::Astar()
{
	Vector2 tileStartPos;
	tileStartPos = obj->position / 1.5 / 8;

	int nowTileRegionId = GRIDMANAGER->regionsTile[(int)tileStartPos.x][(int)tileStartPos.y].regionsIds;
	auto unit = dynamic_cast<Unit*>(obj);

	int searchNodeId = 0;

	if (unit->moveNodeStack.empty() == true)
	{
		return;
	}
	searchNodeId = unit->moveNodeStack.top()->regionId;

	vector<pair<pair<float, float>, GridManager::tileNum>> openNode;
	// cost or node
	//priority_queue <> regionQueue;
	priority_queue < pair<pair<float, float>, GridManager::tileNum>, vector<pair<pair<float, float>, GridManager::tileNum>>, comp> regionQueue;
	openNode.push_back(make_pair(make_pair(0, 0), GRIDMANAGER->regionsTile[(int)tileStartPos.x][(int)tileStartPos.y]));
	map<pair<int, int>, Vector2> openNodeCheck;
	regionQueue.push(make_pair(make_pair(0, 0), openNode[0].second));
	float startX = regionQueue.top().second.x;
	float startY = regionQueue.top().second.y;
	openNodeCheck.insert(make_pair(make_pair(regionQueue.top().second.x, regionQueue.top().second.y), Vector2{ (float)0,(float)0 }));

	int c = 0;
	bool isFind = false;

	while (regionQueue.empty() == false)
	{
		auto iter = regionQueue.top();
		regionQueue.pop();

		if (iter.second.y < 511)
		{
			GridManager::tileNum aNode = GRIDMANAGER->regionsTile[(int)iter.second.x][(int)iter.second.y + 1];
			if (aNode.isBuildAble == true)
			{
				if (startY + m_size.y / 2 >= aNode.y)
				{
					if (openNodeCheck.find(pair<int, int>(aNode.x, aNode.y)) == openNodeCheck.end())
					{
						float dx = abs(aNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(aNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), aNode));
						openNodeCheck.insert(make_pair(make_pair(aNode.x, aNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.y > 0)
		{
			GridManager::tileNum bNode = GRIDMANAGER->regionsTile[(int)iter.second.x][(int)iter.second.y - 1];
			if (bNode.isBuildAble == true)
			{
				if (startY - m_size.y / 2 <= bNode.y)
				{
					if (openNodeCheck.find(pair<int, int>(bNode.x, bNode.y)) == openNodeCheck.end())
					{
						float dx = abs(bNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(bNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), GRIDMANAGER->regionsTile[(int)iter.second.x][(int)iter.second.y - 1]));
						openNodeCheck.insert(make_pair(make_pair(bNode.x, bNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.x > 0)
		{
			GridManager::tileNum dNode = GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y];
			if (dNode.isBuildAble == true)
			{
				if (startX - m_size.x / 2 <= dNode.x)
				{
					if (openNodeCheck.find(pair<int, int>(dNode.x, dNode.y)) == openNodeCheck.end())
					{
						float dx = abs(dNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(dNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y]));
						openNodeCheck.insert(make_pair(make_pair(dNode.x, dNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.x < 511)
		{
			GridManager::tileNum cNode = GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y];
			if (cNode.isBuildAble == true)
			{
				if (startX + m_size.x / 2 >= cNode.x)
				{
					if (openNodeCheck.find(pair<int, int>(cNode.x, cNode.y)) == openNodeCheck.end())
					{
						float dx = abs(cNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(cNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y]));
						openNodeCheck.insert(make_pair(make_pair(cNode.x, cNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.x > 0 && iter.second.y < 511)
		{
			GridManager::tileNum eNode = GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y + 1];
			if (eNode.isBuildAble == true)
			{
				if (startX - m_size.x / 2 <= eNode.x && startY + m_size.y / 2 >= eNode.y)
				{
					if (openNodeCheck.find(pair<int, int>(eNode.x, eNode.y)) == openNodeCheck.end())
					{
						float dx = abs(eNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(eNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y + 1]));
						openNodeCheck.insert(make_pair(make_pair(eNode.x, eNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.y > 0 && iter.second.x < 511)
		{
			GridManager::tileNum fNode = GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y - 1];
			if (fNode.isBuildAble == true)
			{
				if (startX + m_size.x / 2 >= fNode.x && startY - m_size.y / 2 <= fNode.y)
				{
					if (openNodeCheck.find(pair<int, int>(fNode.x, fNode.y)) == openNodeCheck.end())
					{
						float dx = abs(fNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(fNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y - 1]));
						openNodeCheck.insert(make_pair(make_pair(fNode.x, fNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.x < 511 && iter.second.y < 511)
		{
			GridManager::tileNum gNode = GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y + 1];
			if (gNode.isBuildAble == true)
			{
				if (startX + m_size.x / 2 >= gNode.x && startY + m_size.y / 2 >= gNode.y)
				{
					if (openNodeCheck.find(pair<int, int>(gNode.x, gNode.y)) == openNodeCheck.end())
					{
						float dx = abs(gNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(gNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y + 1]));
						openNodeCheck.insert(make_pair(make_pair(gNode.x, gNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.x > 0 && iter.second.y > 0)
		{
			GridManager::tileNum hNode = GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y - 1];
			if (hNode.isBuildAble == true)
			{
				if (startX - m_size.x / 2 <= hNode.x && startY - m_size.y / 2 <= hNode.y)
				{
					if (openNodeCheck.find(pair<int, int>(hNode.x, hNode.y)) == openNodeCheck.end())
					{
						float dx = abs(hNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(hNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 + e2 + iter.first.first + 14;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y - 1]));
						openNodeCheck.insert(make_pair(make_pair(hNode.x, hNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
				}
			}
		}

		if (iter.second.regionsIds == searchNodeId)
		{
			map<pair<int, int>, Vector2>::iterator find = openNodeCheck.find(pair(iter.second.x, iter.second.y));
			find = openNodeCheck.find(pair(find->second.x, find->second.y));

			if (find != openNodeCheck.end())
			{
				while (true)
				{
					find = openNodeCheck.find(pair(find->second.x, find->second.y));

					if (find != openNodeCheck.end())
					{
						if (find->first.first == startX && find->first.second == startY)
						{
							break;
						}
						else
						{
							moveStack2.push(Vector2{ (float)find->first.first ,(float)find->first.second });
						}
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
	}
	openNodeCheck.clear();
}

void ObjectGrid::Init(Object* obj, Vector2 collisionGridSize, Vector2 gridSize, float x, float y)
{
	this->obj = obj;
	m_size = gridSize;
	m_collisionGridSize = collisionGridSize;
	this->x = x;
	this->y = y;
	lastX = 0, lastY = 0;
}

void ObjectGrid::Update()
{
	if (lastX != 0 && lastY != 0)
	{
		for (int i = lastX; i < lastX + m_collisionGridSize.x * 4; i++)
		{
			for (int j = lastY; j < lastY + m_collisionGridSize.y * 4; j++)
			{
				GRIDMANAGER->regionsTile[i][j].isBuildAble = true;
			}
		}
	}
	int fx = (int)(obj->position.x / 8 / 1.5) + x;
	int fy = (int)(obj->position.y / 8 / 1.5) + y;

	for (int i = fx; i < fx + m_collisionGridSize.x * 4; i++)
	{
		for (int j = fy; j < fy + m_collisionGridSize.y * 4; j++)
		{
			GRIDMANAGER->regionsTile[i][j].isBuildAble = false;
		}
	}
	lastX = fx;
	lastY = fy;
}

void ObjectGrid::Render()
{

}

void ObjectGrid::Release()
{
}
