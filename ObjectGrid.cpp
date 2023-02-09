#include "stdafx.h"
#include "ObjectGrid.h"
#include "Object.h"
#include "Unit.h"
#include "MapReader.h"

void ObjectGrid::Astar()
{
	auto unit = dynamic_cast<Unit*>(obj);

	Vector2 tileStartPos;
	tileStartPos = obj->position / 1.5 / 8;
	Vector2 tileEndPos = unit->m_dest / 1.5 / 8;
	if (unit->moveNodeStack.empty() == true)
	{
		cout << "노드스택비었음" << endl;
		return;
	}
	int nextRegionId = unit->moveNodeStack.top()->regionId;
	nowTileRegionId = GRIDMANAGER->regionsTile[(int)tileStartPos.x][(int)tileStartPos.y].regionsIds;
	tileNum tile;
	if (unit->moveNodeStack.empty() == false)
	{
		float dest = 10000;
		auto find = IMAGEMANAGER->GetMapReader()->mapRegions[nowTileRegionId]->nearRegionPosition.find(nextRegionId);
		if (find == IMAGEMANAGER->GetMapReader()->mapRegions[nowTileRegionId]->nearRegionPosition.end())
		{
			for (auto _iter : IMAGEMANAGER->GetMapReader()->mapRegions[nowTileRegionId]->nearRegionPosition)
			{
				cout << "현재" << nowTileRegionId << "인접 :" << _iter.first << endl;
			}
			cout << "인접 Region을 찾지 못함" << endl;
			return;
		}
		for (auto iter : find->second)
		{
			float dx = abs((int)tileStartPos.x - (int)iter.x);
			float dy = abs((int)tileStartPos.y - (int)iter.y);
			float e1 = abs(dx - dy);
			float e2 = min(dx, dy);
			if (dest > e1 * 10 + e2 * 14)
			{
				tileEndPos = iter;
				dest = e1 * 10 + e2 * 14;
			}
		}
	}

	if (tileStartPos == tileEndPos)
	{
		cout << "시작점과 끝점이 같음";
		return;
	}

	map<pair<float, float>, Vector2> distmap;// 다음,지금
	// priority_queue < pair<pair<float, float>, MapRegions*>, vector<pair<pair<float, float>, MapRegions*>>, comp> regionQueue;

	priority_queue <pair<Vector2, pair<float, float>>, vector<pair<Vector2, pair<float, float>>>, comp> regionQueue;
	regionQueue.push(make_pair(tileStartPos, make_pair(0, 0)));

	while (regionQueue.empty() == false)
	{
		auto iter = regionQueue.top();
		regionQueue.pop();
		if (GRIDMANAGER->regionsTile[(int)iter.first.x][(int)iter.first.y].regionsIds == nextRegionId)
		{
			auto find = distmap.find(make_pair(iter.first.x, iter.first.y));
			while (true)
			{
				if (find != distmap.end())
				{
					moveStack2.push({ find->first.first,find->first.second });
					if (find->first.first == tileStartPos.x && find->first.second == tileStartPos.y)
					{
						break;
					}
					find = distmap.find(make_pair(find->second.x, find->second.y));
				}
				else
				{
					cout << "찾 못";
					break;
				}
			}
		}

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				int searchX = (int)iter.first.x + x - 2;
				int searchY = (int)iter.first.y + y - 2;
				if (y == 1 && x == 1)
				{
					continue;
				}
				if (searchX < 0 || searchX > 512 || searchY < 0 || searchY >512)
				{
					continue;
				}

				auto find2 = distmap.find(make_pair(searchX, searchY));
				if (find2 == distmap.end())
				{
					if (GRIDMANAGER->regionsTile[(int)searchX][(int)searchY].isBuildTag == 0)
					{
						int cost = 10 + iter.second.first;
						if ((x + y) % 2 == 0)
						{
							cost += 4;
						}

						float dx = abs((int)searchX - (int)tileEndPos.x);
						float dy = abs((int)searchY - (int)tileEndPos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float d = e1 * 10 + e2 * 14;

						regionQueue.push(make_pair(Vector2{ (float)searchX, (float)searchY }, make_pair(cost, d)));
						distmap.insert(make_pair(make_pair((int)searchX, (int)searchY), iter.first));
					}
				}
			}
		}
	}


	if (lastX != 0 && lastY != 0) // Astar를 위해 현재 서있는 그리드를 지워준다
	{
		for (int i = lastX; i < lastX + m_collisionGridSize.x * 4; i++)
		{
			for (int j = lastY; j < lastY + m_collisionGridSize.y * 4; j++)
			{
				if (GRIDMANAGER->regionsTile[i][j].isBuildTag == gridTag)
					GRIDMANAGER->regionsTile[i][j].isBuildTag = 0;
			}
		}
	}

	int fx = (int)(obj->position.x / 8 / 1.5) + x;
	int fy = (int)(obj->position.y / 8 / 1.5) + y;

	for (int i = fx; i < fx + m_collisionGridSize.x * 4; i++)
	{
		for (int j = fy; j < fy + m_collisionGridSize.y * 4; j++)
		{
			if (GRIDMANAGER->regionsTile[i][j].isBuildTag == 0)
			{
				GRIDMANAGER->regionsTile[i][j].isBuildTag = gridTag;
			}
		}
	}
	lastX = fx;
	lastY = fy;
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
				if (GRIDMANAGER->regionsTile[i][j].isBuildTag == gridTag)
					GRIDMANAGER->regionsTile[i][j].isBuildTag = 0;
			}
		}
	}
	int fx = (int)(obj->position.x / 8 / 1.5) + x;
	int fy = (int)(obj->position.y / 8 / 1.5) + y;

	for (int i = fx; i < fx + m_collisionGridSize.x * 4; i++)
	{
		for (int j = fy; j < fy + m_collisionGridSize.y * 4; j++)
		{
			GRIDMANAGER->regionsTile[i][j].isBuildTag = gridTag;
		}
	}
	lastX = fx;
	lastY = fy;
}

void ObjectGrid::Render()
{
	for (auto iter : openNodeCheck)
	{
		IMAGEMANAGER->DrawRectRed(
			{
				float(iter.second.x) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x,
				float(iter.second.y) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y },
			{ float(iter.second.x) * 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x
			,float(iter.second.y) * 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y
			});
	}
	//for (auto iter : vectors)
	//	IMAGEMANAGER->DrawLine({ iter.first.x * 1.5f * 8, iter.first.y * 1.5f * 8 }, { iter.second.x * 1.5f * 8, iter.second.y * 1.5f * 8 });

	//auto f = IMAGEMANAGER->GetMapReader()->mapRegions[GRIDMANAGER->regionsTile[(int)(obj->position.x / 1.5 / 8)][(int)(obj->position.y / 1.5 / 8)].regionsIds];
	//for (auto iter : IMAGEMANAGER->GetMapReader()->mapRegions[GRIDMANAGER->regionsTile[(int)(obj->position.x / 1.5 / 8)][(int)(obj->position.y / 1.5 / 8)].regionsIds]->nearRegionPosition)
	//{
	//	auto f2 = IMAGEMANAGER->GetMapReader()->mapRegions[iter.first];
	//		IMAGEMANAGER->DrawLine({ f->pos.x * 1.5f * 8, f->pos.y * 1.5f * 8 }, { f2->pos.x * 1.5f * 8, f2->pos.y * 1.5f * 8 });

	//}

	for (auto iter : vectors)
		IMAGEMANAGER->DrawLine({ iter.first.x * 1.5f * 8, iter.first.y * 1.5f * 8 }, { iter.second.x * 1.5f * 8, iter.second.y * 1.5f * 8 });
}

void ObjectGrid::Release()
{
	if (lastX != 0 && lastY != 0)
	{
		for (int i = lastX; i < lastX + m_collisionGridSize.x * 4; i++)
		{
			for (int j = lastY; j < lastY + m_collisionGridSize.y * 4; j++)
			{
				if (GRIDMANAGER->regionsTile[i][j].isBuildTag == gridTag)
				{
					GRIDMANAGER->regionsTile[i][j].isBuildTag = 0;
				}
			}
		}
	}
	isDestory = true;
}
