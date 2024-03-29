#include "stdafx.h"
#include "Player.h"
#include "SpaceConstructionVehicle.h"
#include "CommandCenter.h"
#include "Marine.h"
#include "FireBat.h"
#include "Barrack.h"
#include "EngineeringBay.h"
#include "Academy.h"
#include "Factory.h"
#include "ScienceFacility.h"
#include "Tank.h"
#include "Armory.h"
#include "Starport.h"
#include "Gas.h"
#include "Depot.h"
#include "Machines.h"
#include "Vulture.h"
void Player::Astar(Vector2 startPos, Vector2 endPos, Unit* unit)
{
	Vector2 tileStartPos, tileEndPos;
	tileStartPos = startPos / 1.5 / 8;
	tileEndPos = endPos / 1.5 / 8;
	int nowTileRegionId = IMAGEMANAGER->GetMapReader()->region->regionsIds[(int)tileStartPos.y][(int)tileStartPos.x].regionsIds;
	while (!unit->moveNodeStack.empty())
	{
		unit->moveNodeStack.pop();
	}
	if (nowTileRegionId == -1)
	{
		return;
	}
	if (nowTileRegionId == IMAGEMANAGER->GetMapReader()->region->regionsIds[(int)tileEndPos.y][(int)tileEndPos.x].regionsIds)
	{
		return;
	}
	vector<pair<pair<float, float>, MapRegions*>> openNode;
	// cost or node
	//priority_queue <> regionQueue;
	priority_queue < pair<pair<float, float>, MapRegions*>, vector<pair<pair<float, float>, MapRegions*>>, comp> regionQueue;
	openNode.push_back(make_pair(make_pair(0, 0), IMAGEMANAGER->GetMapReader()->mapRegions[nowTileRegionId]));
	regionQueue.push(make_pair(make_pair(0, 0), openNode[0].second));
	int startRegionId = openNode[0].second->regionId;
	int c = 0;
	bool isFind = false;

	for (auto iter : IMAGEMANAGER->GetMapReader()->mapRegions)
	{
		iter->openNode = false;
	}
	while (regionQueue.empty() != true)
	{
		auto iter = regionQueue.top();
		regionQueue.pop();

		MapRegions* nextNode = nullptr;

		for (auto& nextRegion : iter.second->nearRegions)
		{
			if (nextRegion.second->openNode == false)
			{
				nextRegion.second->openNode = true;

				float dx = abs(nextRegion.second->pos.x - (int)tileEndPos.x);
				float dy = abs(nextRegion.second->pos.y - (int)tileEndPos.y);
				float e1 = abs(dx - dy);
				float e2 = min(dx, dy);
				float dest = e1 + e2 + nextRegion.first + iter.first.first;

				nextRegion.second->whereRegionId = iter.second->regionId;
				regionQueue.push(make_pair(make_pair(dest, dest), nextRegion.second));
			}
			if (nextRegion.second->regionId == IMAGEMANAGER->GetMapReader()->region->regionsIds[(int)tileEndPos.y][(int)tileEndPos.x].regionsIds)
			{
				bool isOut = false;
				nextRegion.second->whereRegionId = iter.second->regionId;

				auto __iter = nextRegion;

				while (true)
				{
					for (auto ___iter : __iter.second->nearRegions)
					{
						if (__iter.second->regionId == startRegionId)
						{
							isOut = true;
							break;
						}
						if (___iter.second->regionId == __iter.second->whereRegionId)
						{
							if (__iter.second->regionId == startRegionId)
							{
								isOut = true;
								break;
							}
							else
							{
								testDraw.push_back(make_pair(___iter.second->pos, __iter.second->pos));

								unit->moveNodeStack.push(new MoveNode{ ___iter.second->pos,__iter.second->regionId });
								__iter = ___iter;
								break;
							}
						}
					}
					if (isOut == true)
					{
						break;
					}
				}
				isFind = true;
				break;
			}
		}
		if (isFind == true || (iter.second->pos == tileEndPos))
		{
			break;
		}
	}
}

void Player::Init()
{
	m_clickRad = 0;
	OBJECTMANAGER->m_player = this;
	sizeClick = 1.7;
	m_isCameraClick = true;
	m_cursorImage = IMAGEMANAGER->AddImageVectorCopy("arrow0000");
	m_cursorImage->Setting(0.15, true);
	string unitsStr;
	unitsStr = FILEMANAGER->GetFileData("Player1", "Batch");
	vector<ObjectData*> objs;
	MY_UTILITY::ConvertStructureString2Vec(&objs, unitsStr);

	for (auto iter : objs)
	{
		switch (iter->key)
		{
		case 0:
		{
			Object* me = new Mineral;
			OBJECTMANAGER->AddObject(me, "mimeral", iter->x, iter->y, 0);
			resrouces.push_back(me);
		}
		break;
		case 1:
		{
			Build* commandCenter = new CommandCenter;
			commandCenter->SetPlayer(this);
			IMAGEMANAGER->CameraSetPosition({ float(iter->x - WINSIZE_X / 2.f), float(iter->y - WINSIZE_Y / 2.f) + 80.f });
			OBJECTMANAGER->AddObject(commandCenter, "Build", iter->x, iter->y, 0);
			commandCenter->AddComplete();
		}
		break;
		case 6:
		{
			Marine* marine = new Marine;
			marine->SetPlayer(this);
			OBJECTMANAGER->AddObject(marine, "marine", iter->x, iter->y - 200, 0);
		}
		break;
		case 7:
		{
			Unit* scv = new SpaceConstructionVehicle;
			scv->SetPlayer(this);
			OBJECTMANAGER->AddObject(scv, "Unit", iter->x, iter->y, 1);
		}
		break;
		case 10:
		{
			Object* me = new Gas;
			OBJECTMANAGER->AddObject(me, "mimeral", iter->x, iter->y, 0);
			resrouces.push_back(me);
		}
		break;
		}
	}

	m_selectBuild = nullptr;
	m_selectUnit = nullptr;

	mapRect = {
		11,
		560,
		11 + int(4096 * 0.0495f),
		560 + int(4096 * 0.0495f)
	};
}

void Player::Update()
{
	for (int i = 0; i < 10; i++)
	{
		buildList[i] = false;
	}
	for (auto iter : m_builds)
	{
		if (typeid(*iter).name() == typeid(Barrack).name())
		{
			buildList[eBarrack] = true;
		}
		if (typeid(*iter).name() == typeid(CommandCenter).name())
		{
			buildList[eCommandCenter] = true;
		}
		if (typeid(*iter).name() == typeid(EngineeringBay).name())
		{
			buildList[eEngine] = true;
		}
		if (typeid(*iter).name() == typeid(Academy).name())
		{
			buildList[eAcademy] = true;
		}
		if (typeid(*iter).name() == typeid(Armory).name())
		{
			buildList[eArmory] = true;
		}
		if (typeid(*iter).name() == typeid(Factory).name())
		{
			buildList[eFactory] = true;
		}
		if (typeid(*iter).name() == typeid(Starport).name())
		{
			buildList[eStarport] = true;
		}
	}


	updateTimer += DELTA_TIME;
	if (m_selectBuild != nullptr)
	{
		m_selectBuild->SelectBuild();
	}

	if (KEYMANAGER->GetOnceKeyDown(VK_RBUTTON))
	{
		m_clickRad = 0;
		m_rClickPos.x = _ptMouse.x + IMAGEMANAGER->GetCameraPosition().x;
		m_rClickPos.y = _ptMouse.y + IMAGEMANAGER->GetCameraPosition().y;
		if (m_selectUnit != nullptr)
		{
			m_selectUnit->ResetParam();
			Vector2 p = m_rClickPos;
			for (auto iter : resrouces)
			{
				m_selectUnit->SetDestPosition(m_rClickPos);
				if (PtInRect(&iter->clickRect, _ptMouse))
				{
					if (typeid(*m_selectUnit).name() == typeid(SpaceConstructionVehicle).name())
					{
						SpaceConstructionVehicle* scv = dynamic_cast<SpaceConstructionVehicle*>(m_selectUnit);
						if (typeid(*iter).name() == typeid(Mineral).name())
						{
							scv->me = dynamic_cast<Mineral*>(iter);
						}
					}
					p = iter->position;
					m_selectUnit->SetDestPosition(iter->position);
				}
			}
			Astar(m_selectUnit->GetPosition(), { p }, m_selectUnit);
		}
		for (auto iter : m_selectUnits)
		{
			iter->ResetParam();

			Vector2 p = m_rClickPos;
			for (auto _iter : resrouces)
			{
				iter->SetDestPosition(m_rClickPos);
				if (PtInRect(&_iter->clickRect, _ptMouse))
				{
					if (typeid(*iter).name() == typeid(SpaceConstructionVehicle).name())
					{
						SpaceConstructionVehicle* scv = dynamic_cast<SpaceConstructionVehicle*>(iter);
						if (typeid(*_iter).name() == typeid(Mineral).name())
						{
							scv->me = dynamic_cast<Mineral*>(_iter);
						}
					}
					p = _iter->position;
					iter->SetDestPosition(_iter->position);
				}
			}
			Astar(iter->GetPosition(), { p }, iter);
		}
	}

	if (KEYMANAGER->GetOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&mapRect, _ptMouse))
		{
			m_isCameraClick = true;
		}
		else if (_ptMouse.y < 600)
		{
			m_isClick = true;
			m_clickStartX = _ptMouse.x;
			m_clickStartY = _ptMouse.y;

			m_clickEndX = _ptMouse.x;
			m_clickEndY = _ptMouse.y;
		}
		if (m_selectUnit != nullptr)
		{
			if (typeid(*m_selectUnit).name() == typeid(SpaceConstructionVehicle).name())
			{
				m_rClickPos.x = _ptMouse.x + IMAGEMANAGER->GetCameraPosition().x;
				m_rClickPos.y = _ptMouse.y + IMAGEMANAGER->GetCameraPosition().y;

				m_rClickPos.x = _ptMouse.x + IMAGEMANAGER->GetCameraPosition().x;
				m_rClickPos.y = _ptMouse.y + IMAGEMANAGER->GetCameraPosition().y;

				SpaceConstructionVehicle* scv = dynamic_cast<SpaceConstructionVehicle*>(m_selectUnit);
				if (scv->buildIndex != 0)
				{
					float width = 0;
					float height = 0;
					/*
						enum eBuildClass
						{
							eBegin,
							eCommandCenter,
							eFactory,
							eBarrack,
							eAcademy,
							eEngin,
							eDepot,
							eStarport,
							eArmory,
							eScience
						};
					*/

					switch (scv->buildIndex)
					{
					case SpaceConstructionVehicle::eBuildClass::eCommandCenter:
					case SpaceConstructionVehicle::eBuildClass::eFactory:
					case SpaceConstructionVehicle::eBuildClass::eBarrack:
					case SpaceConstructionVehicle::eBuildClass::eEngin:
					case SpaceConstructionVehicle::eBuildClass::eStarport:
						width = 32.f * 1.5 * 4.f / 2.f;
						height = 32.f * 1.5 * 3.f / 2.f;
						break;
					case SpaceConstructionVehicle::eBuildClass::eDepot:
					case SpaceConstructionVehicle::eBuildClass::eAcademy:
					case SpaceConstructionVehicle::eBuildClass::eArmory:
					case SpaceConstructionVehicle::eBuildClass::eScience:
						width = 32.f * 1.5 * 3.f / 2.f;
						height = 32.f * 1.5 * 2.f / 2.f;
						break;
					case SpaceConstructionVehicle::eBuildClass::eGas:
						width = 32.f * 1.5 * 3.f / 2.f;
						height = 32.f * 1.5 * 2.f / 2.f;
						break;
					}


					if ((scv->buildIndex != 10 && scv->isBuildAble_ == true) || (scv->buildIndex == scv->eGas && scv->isBuildAble_ == true))
					{
						scv->m_dest = { (float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) + width
							, (float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) + height };
						Astar(scv->position, scv->m_dest, scv);
						scv->buildPos = scv->m_dest;
						scv->m_isBuild = true;
					}
				}
			}
		}
	}
	else if (KEYMANAGER->GetStayKeyDown(VK_LBUTTON))
	{
		if (m_isClick == true)
		{
			m_clickEndX = _ptMouse.x;
			if (_ptMouse.y < 600)
			{
				m_clickEndY = _ptMouse.y;
			}
			else
			{
				m_clickEndY = 600;
			}
		}
		else if (m_isCameraClick)
		{
			int mapWidth = mapRect.right - mapRect.left;
			int mapHeight = mapRect.bottom - mapRect.top;

			float xx = (mapWidth / 4096.f * WINSIZE_X / 2);
			float yy = (mapWidth / 4296.f * WINSIZE_Y / 2);
			float mouseMapX = _ptMouse.x - xx - mapRect.left > 0 ? _ptMouse.x - xx - mapRect.left : 0;
			float mouseMapY = _ptMouse.y - yy - mapRect.top > 0 ? _ptMouse.y - yy - mapRect.top : 0;
			mapWidth -= xx * 3;
			mapHeight -= yy * 3;

			float camPosX = mouseMapX * ((4096.f) / mapWidth) < 4096 + WINSIZE_X - 20 ? mouseMapX * ((4096.f) / mapWidth) : 4096 + WINSIZE_X - 20;
			float camPosY = mouseMapY * ((4296.f / mapHeight)) < 4296.f + WINSIZE_Y - 20 ? mouseMapY * ((4296.f) / mapHeight) : 4296.f + WINSIZE_Y - 20;
			IMAGEMANAGER->CameraSetPosition({ camPosX ,camPosY });
		}
	}
	if (m_isClick == true && KEYMANAGER->GetOnceKeyUp(VK_LBUTTON))
	{
		RECT pRt;
		if (m_clickStartX > m_clickEndX)
		{
			float swap = m_clickEndX;
			m_clickEndX = m_clickStartX;
			m_clickStartX = swap;
		}
		if (m_clickStartY > m_clickEndY)
		{
			float swap = m_clickEndY;
			m_clickEndY = m_clickStartY;
			m_clickStartY = swap;
		}

		for (auto iter : m_builds)
		{

			RECT rtMouse = { m_clickStartX  ,
			m_clickStartY ,
				m_clickEndX ,
				m_clickEndY
			};
			if (IntersectRect(&pRt, &rtMouse, &iter->GetClickRect()))
			{
				m_selectBuild = iter;
				m_selectUnit = nullptr;
				m_selectUnits.clear();

				if (typeid(*m_selectBuild) == typeid(Academy))
				{
					SOUNDMANAGER->play("tacwht00", 0.5f);
				}
				else if (typeid(*m_selectBuild) == typeid(EngineeringBay))
				{
					SOUNDMANAGER->play("twpwht00", 0.5f);
				}
				else if (typeid(*m_selectBuild) == typeid(Depot))
				{
					SOUNDMANAGER->play("tnswht00", 0.5f);
				}
				else if (typeid(*m_selectBuild) == typeid(Armory))
				{
					SOUNDMANAGER->play("tclwht00", 0.5f);
				}
				else if (typeid(*m_selectBuild) == typeid(Machines))
				{
					SOUNDMANAGER->play("tmswht00", 0.5f);
				}

				break;
			}
		}

		m_isClick = false;
		m_isCameraClick = false;
		int count = 0;
		for (auto iter : m_units)
		{
			RECT rtMouse = { m_clickStartX  ,
			m_clickStartY ,
				m_clickEndX ,
				m_clickEndY
			};

			RECT pRt;

			if (abs(m_clickEndX - m_clickStartX) > 0.1f)
			{
				if (IntersectRect(&pRt, &rtMouse, &iter->GetClickRect()))
				{
					if (count == 0)
					{
						m_selectUnits.clear();
						m_selectUnit = nullptr;
						m_selectBuild = nullptr;
						if (typeid(*iter) == typeid(SpaceConstructionVehicle))
						{
							if (rand() % 4 == 0)
							{
								SOUNDMANAGER->play("tscwht00", 0.5f);
							}
							else if (rand() % 4 == 1)
							{
								SOUNDMANAGER->play("tscwht01", 0.5f);
							}
							else if (rand() % 4 == 2)
							{
								SOUNDMANAGER->play("tscwht02", 0.5f);
							}
							else
							{
								SOUNDMANAGER->play("tscwht03", 0.5f);
							}
						}
						else if (typeid(*iter) == typeid(Marine))
						{
							if (rand() % 4 == 0)
							{
								SOUNDMANAGER->play("tmawht00", 0.5f);
							}
							else if (rand() % 4 == 1)
							{
								SOUNDMANAGER->play("tmawht01", 0.5f);
							}
							else if (rand() % 4 == 2)
							{
								SOUNDMANAGER->play("tmawht02", 0.5f);
							}
							else
							{
								SOUNDMANAGER->play("tmawht03", 0.5f);
							}
						}
						else if (typeid(*iter) == typeid(FireBat))
						{
							if (rand() % 4 == 0)
							{
								SOUNDMANAGER->play("tfbwht00", 0.5f);
							}
							else if (rand() % 4 == 1)
							{
								SOUNDMANAGER->play("tfbwht01", 0.5f);
							}
							else if (rand() % 4 == 2)
							{
								SOUNDMANAGER->play("tfbwht02", 0.5f);
							}
							else
							{
								SOUNDMANAGER->play("tfbwht03", 0.5f);
							}
						}
						else if (typeid(*iter) == typeid(Vulture))
						{
							if (rand() % 4 == 0)
							{
								SOUNDMANAGER->play("tvuwht00", 0.5f);
							}
							else if (rand() % 4 == 1)
							{
								SOUNDMANAGER->play("tvuwht01", 0.5f);
							}
							else if (rand() % 4 == 2)
							{
								SOUNDMANAGER->play("tvuwht02", 0.5f);
							}
							else
							{
								SOUNDMANAGER->play("tvuwht03", 0.5f);
							}
						}
						else if (typeid(*iter) == typeid(Tank))
						{
							if (rand() % 4 == 0)
							{
								SOUNDMANAGER->play("ttawht00", 0.5f);
							}
							else if (rand() % 4 == 1)
							{
								SOUNDMANAGER->play("ttawht01", 0.5f);
							}
							else if (rand() % 4 == 2)
							{
								SOUNDMANAGER->play("ttawht02", 0.5f);
							}
							else
							{
								SOUNDMANAGER->play("ttawht03", 0.5f);
							}
						}
					}
					m_selectUnits.push_back(iter);
					count++;
				}
			}
			else
			{
				if (PtInRect(&iter->GetClickRect(), _ptMouse))
				{
					if (m_selectUnits.size() != 0)
						m_selectUnits.clear();
					m_selectUnit = iter;

					if (typeid(*m_selectUnit) == typeid(SpaceConstructionVehicle))
					{
						if (rand() % 4 == 0)
						{
							SOUNDMANAGER->play("tscwht00", 0.5f);
						}
						else if (rand() % 4 == 1)
						{
							SOUNDMANAGER->play("tscwht01", 0.5f);
						}
						else if (rand() % 4 == 2)
						{
							SOUNDMANAGER->play("tscwht02", 0.5f);
						}
						else
						{
							SOUNDMANAGER->play("tscwht03", 0.5f);
						}
					}
					else if (typeid(*m_selectUnit) == typeid(Marine))
					{
						if (rand() % 4 == 0)
						{
							SOUNDMANAGER->play("tmawht00", 0.5f);
						}
						else if (rand() % 4 == 1)
						{
							SOUNDMANAGER->play("tmawht01", 0.5f);
						}
						else if (rand() % 4 == 2)
						{
							SOUNDMANAGER->play("tmawht02", 0.5f);
						}
						else
						{
							SOUNDMANAGER->play("tmawht03", 0.5f);
						}
					}
					else if (typeid(*m_selectUnit) == typeid(FireBat))
					{
						if (rand() % 4 == 0)
						{
							SOUNDMANAGER->play("tfbwht00", 0.5f);
						}
						else if (rand() % 4 == 1)
						{
							SOUNDMANAGER->play("tfbwht01", 0.5f);
						}
						else if (rand() % 4 == 2)
						{
							SOUNDMANAGER->play("tfbwht02", 0.5f);
						}
						else
						{
							SOUNDMANAGER->play("tfbwht03", 0.5f);
						}
					}
					else if (typeid(*m_selectUnit) == typeid(Vulture))
					{
						if (rand() % 4 == 0)
						{
							SOUNDMANAGER->play("tvuwht00", 0.5f);
						}
						else if (rand() % 4 == 1)
						{
							SOUNDMANAGER->play("tvuwht01", 0.5f);
						}
						else if (rand() % 4 == 2)
						{
							SOUNDMANAGER->play("tvuwht02", 0.5f);
						}
						else
						{
							SOUNDMANAGER->play("tvuwht03", 0.5f);
						}
					}
					else if (typeid(*m_selectUnit) == typeid(Tank))
					{
						if (rand() % 4 == 0)
						{
							SOUNDMANAGER->play("ttawht00", 0.5f);
						}
						else if (rand() % 4 == 1)
						{
							SOUNDMANAGER->play("ttawht01", 0.5f);
						}
						else if (rand() % 4 == 2)
						{
							SOUNDMANAGER->play("ttawht02", 0.5f);
						}
						else
						{
							SOUNDMANAGER->play("ttawht03", 0.5f);
						}
					}

					m_selectBuild = nullptr;
					break;
				}
			}
		}
		if (m_selectUnits.size() == 1)
		{
			m_selectUnit = m_selectUnits[0];
			m_selectUnits.clear();
		}
	}

	if (m_isClick == false)
	{
		if (KEYMANAGER->GetStayKeyDown(VK_LEFT))
		{
			if (IMAGEMANAGER->GetCameraPosition().x > 5)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x - DELTA_TIME * 800, IMAGEMANAGER->GetCameraPosition().y });
			}
		}
		if (KEYMANAGER->GetStayKeyDown(VK_RIGHT))
		{
			if (IMAGEMANAGER->GetCameraPosition().x < 128 * 32 * 1.5f - WINSIZE_X)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x + DELTA_TIME * 800, IMAGEMANAGER->GetCameraPosition().y });
			}
		}
		if (KEYMANAGER->GetStayKeyDown(VK_DOWN))
		{
			if (IMAGEMANAGER->GetCameraPosition().y < 128 * 32 * 1.5f - WINSIZE_Y)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x , IMAGEMANAGER->GetCameraPosition().y + DELTA_TIME * 800 });
			}
		}
		if (KEYMANAGER->GetStayKeyDown(VK_UP))
		{
			if (IMAGEMANAGER->GetCameraPosition().y > 5)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x , IMAGEMANAGER->GetCameraPosition().y - DELTA_TIME * 800 });
			}
		}
	}

}

void Player::Render()
{
	if (KEYMANAGER->GetOnceKeyDown(VK_F5))
	{
		m_mineral += 10000;
	}
	IMAGEMANAGER->MapRender();
}

void Player::UIRender()
{
	IMAGEMANAGER->FogRender();
	updateTimer += DELTA_TIME;
	if (updateTimer > 0.5)
	{
		for (int x = 0; x < 512; x++)
		{
			for (int y = 0; y < 512; y++)
			{
				if (GRIDMANAGER->regionsTile[x][y].fogTag > 0.5f)
				{
					GRIDMANAGER->regionsTile[x][y].fogTag -= DELTA_TIME * 15;
				}
			}
		}
		updateTimer = 0;
	}

	if (m_clickRad < 3.141592)
	{
		m_clickRad += DELTA_TIME * 30;
	}
	else
	{
		m_rClickPos = { -100,-100 };
	}

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("targg0001"), { (float)m_rClickPos.x - IMAGEMANAGER->GetCameraPosition().x,(float)m_rClickPos.y - IMAGEMANAGER->GetCameraPosition().y }, sizeClick + sin(m_clickRad) * 0.6, 0, 0);

	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawRect({ m_clickStartX,m_clickStartY }, { m_clickEndX, m_clickEndY });
	}

	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("playerUI"), { 0,0 }, 1.6f, 0);
	IMAGEMANAGER->UIMapRender();
	if (m_selectUnit != nullptr)
	{
		m_selectUnit->UIRender();
	}

	if (m_selectBuild != nullptr)
	{
		m_selectBuild->UIRender();
	}
	GRIDMANAGER->Render();
	if (m_isClick == false)
	{
		m_cursorImage->CenterRenderBlendBlack(Vector2{ (float)_ptMouse.x - 110,(float)_ptMouse.y - 110 } + IMAGEMANAGER->GetCameraPosition(), 1.7f, 0, 0);
	}
	else
	{
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("drag0000"), { (float)_ptMouse.x - 110,(float)_ptMouse.y - 110 }, 1.7f, 0);
	}

	int count = 0;
	for (auto iter : m_selectUnits)
	{
		if (count >= 12)
		{
			break;
		}
		iter->m_isClick = true;
		if (typeid(*iter).name() == typeid(SpaceConstructionVehicle).name())
		{
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0018"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.8, 0);

			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0007"]->wireImages[3][iter->damageIndex[0]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0007"]->wireImages[1][iter->damageIndex[1]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0007"]->wireImages[2][iter->damageIndex[2]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0007"]->wireImages[0][iter->damageIndex[3]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);

		}
		if (typeid(*iter).name() == typeid(Marine).name())
		{
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0018"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.8, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0000"]->wireImages[3][iter->damageIndex[0]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0000"]->wireImages[1][iter->damageIndex[1]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0000"]->wireImages[2][iter->damageIndex[2]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0000"]->wireImages[0][iter->damageIndex[3]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);

		}
		if (typeid(*iter).name() == typeid(FireBat).name())
		{
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0018"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.8, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[3][iter->damageIndex[0]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[1][iter->damageIndex[1]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[2][iter->damageIndex[2]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[0][iter->damageIndex[3]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);

		}
		if (typeid(*iter).name() == typeid(Vulture).name())
		{
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0018"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.8, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0002"]->wireImages[3][iter->damageIndex[0]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0002"]->wireImages[1][iter->damageIndex[1]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0002"]->wireImages[2][iter->damageIndex[2]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0002"]->wireImages[0][iter->damageIndex[3]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
		}
		if (typeid(*iter).name() == typeid(Tank).name())
		{
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0018"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.8, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0005"]->wireImages[3][iter->damageIndex[0]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0005"]->wireImages[1][iter->damageIndex[1]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0005"]->wireImages[2][iter->damageIndex[2]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
			IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["grpwire0005"]->wireImages[0][iter->damageIndex[3]], { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.85f, 0);
		}
		count++;
	}
	float mapWidth = mapRect.right - mapRect.left;
	float mapHeight = mapRect.bottom - mapRect.top;

	float xx = (mapWidth / (4096.f + WINSIZE_X * 2.f) * IMAGEMANAGER->GetCameraPosition().x);
	float yy = (mapHeight / (4096.f + WINSIZE_Y * 2.5f) * IMAGEMANAGER->GetCameraPosition().y);
	IMAGEMANAGER->DrawRect({ 10 + xx ,560 + yy }, { 10 + xx + float(mapWidth / (4096.f + WINSIZE_X * 2.f) * WINSIZE_X), 560 + yy + float(mapHeight / (4096.f + WINSIZE_Y * 2.5f) * WINSIZE_Y) });


	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("min"), { 700,10 }, 1.5f, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring(m_mineral), { 725,13 }, { 17,17 }, { 0,1,0,1 });

	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("gas"), { 800,10 }, 1.5f, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring(m_gas), { 825,13 }, { 17,17 }, { 0,1,0,1 });

	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("suf"), { 900,10 }, 1.5f, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring(m_suff) + L"/" + to_wstring(m_maxSuff), { 925,13 }, { 17,17 }, { 0,1,0,1 });
}

void Player::Release()
{
	SAFE_DELETE(m_cursorImage);
}

