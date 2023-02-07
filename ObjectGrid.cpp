#include "stdafx.h"
#include "ObjectGrid.h"
#include "Object.h"
#include "Unit.h"
#include "MapReader.h"
void ObjectGrid::Astar()
{
	Vector2 tileStartPos;
	tileStartPos = obj->position / 1.5 / 8;

	nowTileRegionId = GRIDMANAGER->regionsTile[(int)tileStartPos.x][(int)tileStartPos.y].regionsIds;
	auto unit = dynamic_cast<Unit*>(obj);

	int searchNodeId = 0;
	Vector2 tileEndPos;
	while (moveStack2.empty() == false)
	{
		moveStack2.pop();
	}
	Vector2 lastEndPos;
	searchNodeId = GRIDMANAGER->regionsTile[(int)unit->moveNodeStack.top()->pos.x][(int)unit->moveNodeStack.top()->pos.y].regionsIds;
	auto endRegion = GRIDMANAGER->regionsTile[(int)unit->moveNodeStack.top()->pos.x][(int)unit->moveNodeStack.top()->pos.y];
	openNodeCheck.clear();

	auto find = IMAGEMANAGER->GetMapReader()->mapRegions[searchNodeId]->nearRegionPosition.find(nowTileRegionId);

	if (find == IMAGEMANAGER->GetMapReader()->mapRegions[searchNodeId]->nearRegionPosition.end())
	{
		for (auto iter : IMAGEMANAGER->GetMapReader()->mapRegions[searchNodeId]->nearRegionPosition)
		{
			cout << iter.first << endl;
		}
		return;
	}
	else
	{
		float dest2 = 20000;

		for (auto iter : find->second)
		{
			float dx = abs(tileStartPos.x - iter.x);
			float dy = abs(tileStartPos.y - iter.y);
			float	e1 = abs(dx - dy);
			float	e2 = min(dx, dy);
			float dest = e1 * 10 + e2 * 14;
			if (dest < dest2)
			{
				tileEndPos = { iter.x,iter.y };
				dest2 = dest;
			}
		}
	}

	vector<pair<pair<float, float>, GridManager::tileNum>> openNode;
	// cost or node
	//priority_queue <> regionQueue;
	queue < pair<pair<float, float>, GridManager::tileNum>> regionQueue;
	openNode.push_back(make_pair(make_pair(0, 0), GRIDMANAGER->regionsTile[(int)tileStartPos.x][(int)tileStartPos.y]));
	regionQueue.push(make_pair(make_pair(0, 0), openNode[0].second));
	float startX = regionQueue.front().second.x;
	float startY = regionQueue.front().second.y;
	openNodeCheck.insert(make_pair(make_pair(startX, startY), Vector2{ (float)startX,(float)startY }));
	vector<GridManager::tileNum> tiles;
	int c = 0;
	bool isFind = false;

	while (regionQueue.empty() == false)
	{
		auto iter = regionQueue.front();
		regionQueue.pop();

		if (iter.second.y < 511)
		{
			GridManager::tileNum aNode = GRIDMANAGER->regionsTile[(int)iter.second.x][(int)iter.second.y + 1];
			if (aNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(aNode.x, aNode.y)) == openNodeCheck.end())
				{
					if (startY + m_size.y / 2 >= aNode.y)
					{
						float dx = abs(aNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(aNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), aNode));
						openNodeCheck.insert(make_pair(make_pair(aNode.x, aNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.y > 0)
		{
			GridManager::tileNum bNode = GRIDMANAGER->regionsTile[(int)iter.second.x][(int)iter.second.y - 1];
			if (bNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(bNode.x, bNode.y)) == openNodeCheck.end())
				{
					if (startY - m_size.y / 2 <= bNode.y)
					{
						float dx = abs(bNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(bNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), GRIDMANAGER->regionsTile[(int)iter.second.x][(int)iter.second.y - 1]));
						openNodeCheck.insert(make_pair(make_pair(bNode.x, bNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.x > 0)
		{
			GridManager::tileNum dNode = GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y];
			if (dNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(dNode.x, dNode.y)) == openNodeCheck.end())
				{
					if (startX - m_size.x / 2 <= dNode.x)
					{
						float dx = abs(dNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(dNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y]));
						openNodeCheck.insert(make_pair(make_pair(dNode.x, dNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.x < 511)
		{
			GridManager::tileNum cNode = GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y];
			if (cNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(cNode.x, cNode.y)) == openNodeCheck.end())
				{
					if (startX + m_size.x / 2 >= cNode.x)
					{
						float dx = abs(cNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(cNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 10), GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y]));
						openNodeCheck.insert(make_pair(make_pair(cNode.x, cNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.x > 0 && iter.second.y < 511)
		{
			GridManager::tileNum eNode = GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y + 1];
			if (eNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(eNode.x, eNode.y)) == openNodeCheck.end())
				{
					if (startX - m_size.x / 2 <= eNode.x && startY + m_size.y / 2 >= eNode.y)
					{
						float dx = abs(eNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(eNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y + 1]));
						openNodeCheck.insert(make_pair(make_pair(eNode.x, eNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.y > 0 && iter.second.x < 511)
		{
			GridManager::tileNum fNode = GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y - 1];
			if (fNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(fNode.x, fNode.y)) == openNodeCheck.end())
				{
					if (startX + m_size.x / 2 >= fNode.x && startY - m_size.y / 2 <= fNode.y)
					{
						float dx = abs(fNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(fNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y - 1]));
						openNodeCheck.insert(make_pair(make_pair(fNode.x, fNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.x < 511 && iter.second.y < 511)
		{
			GridManager::tileNum gNode = GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y + 1];
			if (gNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(gNode.x, gNode.y)) == openNodeCheck.end())
				{
					if (startX + m_size.x / 2 >= gNode.x && startY + m_size.y / 2 >= gNode.y)
					{
						float dx = abs(gNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(gNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14 + iter.first.second;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x + 1][(int)iter.second.y + 1]));
						openNodeCheck.insert(make_pair(make_pair(gNode.x, gNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.x > 0 && iter.second.y > 0)
		{
			GridManager::tileNum hNode = GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y - 1];
			if (hNode.isBuildAble == true)
			{
				if (openNodeCheck.find(pair<int, int>(hNode.x, hNode.y)) == openNodeCheck.end())
				{
					if (startX - m_size.x / 2 <= hNode.x && startY - m_size.y / 2 <= hNode.y)
					{
						float dx = abs(hNode.x - unit->moveNodeStack.top()->pos.x);
						float dy = abs(hNode.y - unit->moveNodeStack.top()->pos.y);
						float	e1 = abs(dx - dy);
						float	e2 = min(dx, dy);
						float dest = e1 + e2 + iter.first.first + 14;

						regionQueue.push(make_pair(make_pair(dest, iter.first.second + 14), GRIDMANAGER->regionsTile[(int)iter.second.x - 1][(int)iter.second.y - 1]));
						openNodeCheck.insert(make_pair(make_pair(hNode.x, hNode.y), Vector2{ (float)iter.second.x,(float)iter.second.y }));
					}
					else
					{
						bool isTrue = false;
						for (auto _iter : tiles)
						{
							if (_iter.x == iter.second.x && _iter.y == iter.second.y)
							{
								isTrue = true;
								break;
							}
						}
						if (isTrue == false)
							tiles.push_back(iter.second);
					}
				}
			}
		}

		if (iter.second.regionsIds == searchNodeId)
		{
			unit->moveNodeStack.pop();
			map<pair<int, int>, Vector2>::iterator find = openNodeCheck.find(pair(iter.second.x, iter.second.y));
			moveStack2.push(Vector2{ (float)find->first.first ,(float)find->first.second });

			if (find->second.x == startX && find->second.y == startY)
			{
				break;
			}
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
	if (unit->moveNodeStack.empty() == false)
	{
		if (tiles.size() != 0)
		{
			GridManager::tileNum destTile = tiles[0];

			float dx = abs(destTile.x - tileEndPos.x);
			float dy = abs(destTile.y - tileEndPos.y);
			float e1 = abs(dx - dy);
			float e2 = min(dx, dy);
			float dest = e1 * 10 + e2 * 14;

			for (auto iter : tiles)
			{
				float dx2 = abs(iter.x - tileEndPos.x);
				float dy2 = abs(iter.y - tileEndPos.y);
				float	e12 = abs(dx2 - dy2);
				float	e22 = min(dx2, dy2);
				float dest2 = e12 * 10 + e22 * 14;
				if (dest > dest2)
				{
					destTile = iter;
					dest = dest2;
				}
			}
			map<pair<int, int>, Vector2>::iterator find = openNodeCheck.find(pair(destTile.x, destTile.y));
			moveStack2.push(Vector2{ (float)find->first.first ,(float)find->first.second });
			vectors.push_back(make_pair(Vector2{ (float)find->first.first ,(float)find->first.second }, Vector2{ find->second.x ,find->second.y }));

			if (find->second.x == startX && find->second.y == startY)
			{
				return;
			}
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
							vectors.push_back(make_pair(Vector2{ (float)find->first.first ,(float)find->first.second }, Vector2{ find->second.x ,find->second.y }));
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}

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
	//for (auto iter : openNodeCheck)
	//{
	//	IMAGEMANAGER->DrawRectRed({ float(iter.second.x) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x,float(iter.second.y) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y }, { float(iter.second.x) * 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER-//>GetCameraPosition().x,float(iter.second.y) * 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y });
	//
	//}
	//for (auto iter : vectors)
	//	IMAGEMANAGER->DrawLine({ iter.first.x * 1.5f * 8, iter.first.y * 1.5f * 8 }, { iter.second.x * 1.5f * 8, iter.second.y * 1.5f * 8 });

	//auto f = IMAGEMANAGER->GetMapReader()->mapRegions[GRIDMANAGER->regionsTile[(int)(obj->position.x / 1.5 / 8)][(int)(obj->position.y / 1.5 / 8)].regionsIds];
	//for (auto iter : IMAGEMANAGER->GetMapReader()->mapRegions[GRIDMANAGER->regionsTile[(int)(obj->position.x / 1.5 / 8)][(int)(obj->position.y / 1.5 / 8)].regionsIds]->nearRegionPosition)
	//{
	//	auto f2 = IMAGEMANAGER->GetMapReader()->mapRegions[iter.first];
	//		IMAGEMANAGER->DrawLine({ f->pos.x * 1.5f * 8, f->pos.y * 1.5f * 8 }, { f2->pos.x * 1.5f * 8, f2->pos.y * 1.5f * 8 });

	//}

}

void ObjectGrid::Release()
{
}
