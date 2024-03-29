#include "stdafx.h"
#include "ObjectGrid.h"
#include "Object.h"
#include "Unit.h"
#include "MapReader.h"
void ObjectGrid::Astar(float searchSizeX, float searchSizeY, bool isMine)
{
	//OBJECTMANAGER->astarQueue.push([&]() {Astar2(); });
	this->searchSizeX = searchSizeX;
	this->searchSizeY = searchSizeY;
	Astar2(isMine);
}

void ObjectGrid::Astar2(bool isMine)
{
	if (isMine == false)
		if (lastX != 0 && lastY != 0) // Astar를 위해 현재 서있는 그리드를 지워준다
		{
			for (int i = lastX; i < lastX + m_collisionGridSize.x; i++)
			{
				for (int j = lastY; j < lastY + m_collisionGridSize.y; j++)
				{
					if (GRIDMANAGER->regionsTile[i][j].isBuildTag == gridTag)
						GRIDMANAGER->regionsTile[i][j].isBuildTag = 0;
				}
			}
		}

	auto unit = dynamic_cast<Unit*>(obj);

	while (moveStack2.empty() == false)
	{
		moveStack2.pop();
	}
	int c = 0;
	Vector2 tileStartPos;
	tileStartPos = obj->position / 1.5 / 8;
	Vector2 tileEndPos = unit->m_dest / 1.5 / 8;
	if (IMAGEMANAGER->GetMapReader()->region->regionsIds[(int)tileStartPos.y][(int)tileStartPos.x].regionsIds == -1)
	{
		return;
	}
	if (unit->moveNodeStack.empty() == true)
	{
		if (IMAGEMANAGER->GetMapReader()->region->regionsIds[(int)tileStartPos.y][(int)tileStartPos.x].regionsIds != IMAGEMANAGER->GetMapReader()->region->regionsIds[(int)tileEndPos.y][(int)tileEndPos.x].regionsIds)
		{
			return;
		}
	}
	else
	{
		int nextRegionId = unit->moveNodeStack.top()->regionId;
		nowTileRegionId = GRIDMANAGER->regionsTile[(int)tileStartPos.x][(int)tileStartPos.y].regionsIds;
		if (unit->moveNodeStack.empty() == false)
		{
			float dest = 10000;
			auto find = IMAGEMANAGER->GetMapReader()->mapRegions[nextRegionId]->nearRegionPosition.find(nowTileRegionId);
			if (find == IMAGEMANAGER->GetMapReader()->mapRegions[nextRegionId]->nearRegionPosition.end())
			{
				return;
			}
			for (auto iter : find->second)
			{
				float dx = abs((int)tileStartPos.x - (int)iter.x);
				float dy = abs((int)tileStartPos.y - (int)iter.y);
				float e2 = sqrt(dx * dx + dy * dy);
				if (dest > e2)
				{
					tileEndPos = iter;
					dest = e2;
				}
			}
		}
	}

	bool isF = false;
	for (int i = tileEndPos.x + this->x; i < tileEndPos.x + m_collisionGridSize.x + this->x; i++)
	{
		for (int j = tileEndPos.y + this->y; j < tileEndPos.y + m_collisionGridSize.y + this->y; j++)
		{
			if (GRIDMANAGER->regionsTile[(int)i][(int)j].isBuildTag != 0)
			{
				isF = true;
				break;
			}
		}
		if (isF == true)
		{
			break;
		}
	}
	if (isF == true)
	{
		priority_queue <pair< pair<float, float>, Vector2>, vector<pair<pair<float, float>, Vector2>>, comp2> regionQueue;
		c++;

		vector<Vector2> openNode;
		tileEndPos.x = (int)tileEndPos.x;
		tileEndPos.y = (int)tileEndPos.y;

		regionQueue.push(make_pair(make_pair(0, 0), tileEndPos));
		openNode.push_back(tileEndPos);
		while (regionQueue.empty() == false)
		{
			auto iter = regionQueue.top();
			regionQueue.pop();
			bool isF = false;
			if (GRIDMANAGER->regionsTile[(int)iter.second.x][(int)iter.second.y].isBuildTag == 0)
			{
				for (int i = iter.second.x + this->x; i < iter.second.x + m_collisionGridSize.x + this->x; i++)
				{
					for (int j = iter.second.y + this->y; j < iter.second.y + m_collisionGridSize.y + this->y; j++)
					{
						if (i < 0 || j < 0 || i > 511 || j >511)
						{
							continue;
						}
						if (GRIDMANAGER->regionsTile[(int)i][(int)j].isBuildTag != 0 || GRIDMANAGER->regionsTile[(int)i][(int)j].regionsIds != GRIDMANAGER->regionsTile[(int)tileEndPos.x][(int)tileEndPos.y].regionsIds)
						{
							isF = true;
							break;
						}
					}
					if (isF == true)
					{
						break;
					}
				}
				if (isF == false)
				{
					tileEndPos = iter.second;
					break;
				}
			}

			for (int y = -1; y < 2; y++)
			{
				for (int x = -1; x < 2; x++)
				{
					float searchX = iter.second.x + x;
					float searchY = iter.second.y + y;
					if (y == 0 && x == 0)
					{
					}
					else
					{
						if ((int)searchX < 0 || (int)searchX > 511 || (int)searchY < 0 || (int)searchY >511)
						{

						}
						else
						{
							Vector2 find2 = { 0,0 };
							for (auto iter : openNode)
							{
								if (iter == Vector2{ (float)searchX, (float)searchY })
								{
									find2 = iter;
									break;
								}
							}
							if (find2 == Vector2{ 0,0 })
							{
								int cost = 10 + iter.first.first;
								if ((x + y) % 2 == 0)
								{
									cost += 4;
								}

								float dx = abs(searchX - tileStartPos.x);
								float dy = abs(searchY - tileStartPos.y);
								float e1 = abs(dx - dy);
								float e2 = min(dx, dy);
								float d = e1 * 10 + e2 * 14;

								regionQueue.push(make_pair(make_pair(cost, d), Vector2{ searchX, searchY }));
								openNode.push_back({ searchX, searchY });
							}
						}
					}
				}
			}

		}

	}

	map<pair<float, float>, Vector2> distmap; // 다음,지금
	priority_queue <pair<Vector2, pair<float, float>>, vector<pair<Vector2, pair<float, float>>>, comp> regionQueue;
	regionQueue.push(make_pair(tileStartPos, make_pair(0, 0)));
	distmap.insert(make_pair(make_pair(tileStartPos.x, tileStartPos.y), Vector2{ 0.f,0.f }));

	while (regionQueue.empty() == false)
	{
		if (c > 10)
		{
			break;
		}
		auto iter = regionQueue.top();
		regionQueue.pop();
		if ((int)iter.first.x == (int)tileEndPos.x && (int)iter.first.y == (int)tileEndPos.y)
		{
			auto find = distmap.find(make_pair(iter.first.x, iter.first.y));
			while (true)
			{
				if (find != distmap.end())
				{
					moveStack2.push({ find->first.first,find->first.second });

					if ((int)find->first.first == (int)tileStartPos.x && (int)find->first.second == (int)tileStartPos.y)
					{
						break;
					}
					find = distmap.find(make_pair(find->second.x, find->second.y));
				}
				else
				{
					break;
				}
			}
			break;
		}
		for (int y = -1; y < 2; y++)
		{
			for (int x = -1; x < 2; x++)
			{
				float searchX = iter.first.x + x;
				float searchY = iter.first.y + y;
				if (y == 0 && x == 0)
				{
				}
				else
				{
					if ((int)searchX < 0 || (int)searchX > 511 || (int)searchY < 0 || (int)searchY >511)
					{

					}
					else
					{
						auto find2 = distmap.find(make_pair(searchX, searchY));
						if (find2 == distmap.end())
						{
							bool isF = false;
							for (int i = (int)searchX + this->x; i < (int)searchX + m_collisionGridSize.x + this->x; i++)
							{
								for (int j = (int)searchY + this->y; j < (int)searchY + m_collisionGridSize.y + this->y; j++)
								{
									if (i < 0 || j < 0 || i > 511 || j >511)
									{

									}
									else if (GRIDMANAGER->regionsTile[(int)i][(int)j].isBuildTag != 0)
									{
										isF = true;
										break;
									}
								}
								if (isF == true)
								{
									break;
								}
							}
							if (isF == false)
							{
								int cost = 10 + iter.second.first;
								if ((x + y) % 2 == 0)
								{
									cost += 4;
								}

								float dx = abs(searchX - tileEndPos.x);
								float dy = abs(searchY - tileEndPos.y);
								float e1 = abs(dx - dy);
								float e2 = min(dx, dy);
								float d = e1 * 10 + e2 * 14;

								regionQueue.push(make_pair(Vector2{ searchX, searchY }, make_pair(cost, d)));
								distmap.insert(make_pair(make_pair(searchX, searchY), iter.first));
							}
						}
					}
				}
			}
		}
	}
	distmap.clear();

	int fx = (int)(obj->position.x / 8 / 1.5) + x;
	int fy = (int)(obj->position.y / 8 / 1.5) + y;
	if (isMine == false)
		for (int i = fx; i < fx + m_collisionGridSize.x; i++)
		{
			for (int j = fy; j < fy + m_collisionGridSize.y; j++)
			{
				if (GRIDMANAGER->regionsTile[i][j].isBuildTag == 0)
				{
					GRIDMANAGER->regionsTile[i][j].isBuildTag = gridTag;
				}
			}
		}
	lastX = fx;
	lastY = fy;

	if (moveStack2.empty() == false)
		moveStack2.pop();

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

void ObjectGrid::Update(bool isA)
{


	if (lastX != 0 && lastY != 0)
	{
		for (int i = lastX; i < lastX + m_collisionGridSize.x; i++)
		{
			for (int j = lastY; j < lastY + m_collisionGridSize.y; j++)
			{
				if (i > 0 && j > 0)
					if (GRIDMANAGER->regionsTile[i][j].isBuildTag == gridTag)
						GRIDMANAGER->regionsTile[i][j].isBuildTag = 0;
			}
		}
	}
	int fx = (int)(obj->position.x / 8 / 1.5) + x;
	int fy = (int)(obj->position.y / 8 / 1.5) + y;
	for (int i = fx; i < fx + m_collisionGridSize.x; i++)
	{
		for (int j = fy; j < fy + m_collisionGridSize.y; j++)
		{
			if (fx > 0 && fy > 0)
				if (GRIDMANAGER->regionsTile[i][j].isBuildTag == 0)
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
		for (int i = lastX; i < lastX + m_collisionGridSize.x; i++)
		{
			for (int j = lastY; j < lastY + m_collisionGridSize.y; j++)
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
