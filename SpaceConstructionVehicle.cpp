#include "stdafx.h"
#include "SpaceConstructionVehicle.h"
#include "Barrack.h"
#include "CommandCenter.h"
#include "Build.h"
#include "Factory.h"
#include "Academy.h"
#include "EngineeringBay.h"
#include "Depot.h"
#include "Starport.h"
#include "ScienceFacility.h"
#include "Armory.h"
SpaceConstructionVehicle::SpaceConstructionVehicle()
{
}

SpaceConstructionVehicle::~SpaceConstructionVehicle()
{
	player->m_suff -= 1;
	if (player->m_selectUnit == this)
	{
		player->m_selectUnit = nullptr;
	}
	for (auto iter = player->m_selectUnits.begin(); iter < player->m_selectUnits.end(); iter++)
	{
		if ((*iter) == this)
		{
			player->m_selectUnits.erase(iter);
			break;
		}
	}
	for (auto iter = player->m_units.begin(); iter < player->m_units.end(); iter++)
	{
		if ((*iter) == this)
		{
			player->m_units.erase(iter);
			break;
		}
	}
}

void SpaceConstructionVehicle::Init()
{
	sparkImg = IMAGEMANAGER->AddImageVectorCopy("SCVEffect");
	sparkImg->Setting(0.1, true);
	m_maxHp = 60;
	m_hp = m_maxHp;
	BuildTimer = 3;
	buildDest = { 0,0 };
	player->m_suff += 1;
	astarTimer = 0.1;
	grid = GRIDMANAGER->AddGrid(this, 3, 3, 20, 20, -1, -1);
	grid->gridTag = rand() % 10000 + 100;
	m_isClick = false;
	m_spark = nullptr;
	m_nowBuild = nullptr;
	player->AddUnit(this);
	m_actionImage_1[0] = IMAGEMANAGER->FindImage("scv_action_1_1");
	m_actionImage_1[1] = IMAGEMANAGER->FindImage("scv_action_1_2");
	m_actionImage_1[2] = IMAGEMANAGER->FindImage("scv_action_1_3");
	m_actionImage_1[3] = IMAGEMANAGER->FindImage("scv_action_1_4");
	m_actionImage_1[4] = IMAGEMANAGER->FindImage("scv_action_1_5");
	m_actionImage_1[5] = IMAGEMANAGER->FindImage("scv_action_1_6");
	m_actionImage_1[6] = IMAGEMANAGER->FindImage("scv_action_1_7");
	m_actionImage_1[7] = IMAGEMANAGER->FindImage("scv_action_1_8");
	m_actionImage_1[8] = IMAGEMANAGER->FindImage("scv_action_1_9");

	m_actionImage_2[0] = IMAGEMANAGER->FindImage("scv_action_2_1");
	m_actionImage_2[1] = IMAGEMANAGER->FindImage("scv_action_2_2");
	m_actionImage_2[2] = IMAGEMANAGER->FindImage("scv_action_2_3");
	m_actionImage_2[3] = IMAGEMANAGER->FindImage("scv_action_2_4");
	m_actionImage_2[4] = IMAGEMANAGER->FindImage("scv_action_2_5");
	m_actionImage_2[5] = IMAGEMANAGER->FindImage("scv_action_2_6");
	m_actionImage_2[6] = IMAGEMANAGER->FindImage("scv_action_2_7");
	m_actionImage_2[7] = IMAGEMANAGER->FindImage("scv_action_2_8");
	m_actionImage_2[8] = IMAGEMANAGER->FindImage("scv_action_2_9");

	m_idleImage[8] = IMAGEMANAGER->FindImage("scv_idle_1");
	m_idleImage[7] = IMAGEMANAGER->FindImage("scv_idle_2");
	m_idleImage[6] = IMAGEMANAGER->FindImage("scv_idle_3");
	m_idleImage[5] = IMAGEMANAGER->FindImage("scv_idle_4");
	m_idleImage[4] = IMAGEMANAGER->FindImage("scv_idle_5");
	m_idleImage[3] = IMAGEMANAGER->FindImage("scv_idle_6");
	m_idleImage[2] = IMAGEMANAGER->FindImage("scv_idle_7");
	m_idleImage[1] = IMAGEMANAGER->FindImage("scv_idle_8");
	m_idleImage[0] = IMAGEMANAGER->FindImage("scv_idle_9");

	m_buttom = IMAGEMANAGER->FindImage("tcmdbtns0000");
	d = { 0,0 };
	randomMoveTime = 0;
	mineTimer = 0;
	isFA = true;

}

void SpaceConstructionVehicle::Move()
{
	Vector2 nowRegionPos;
	nowRegionPos = position / 1.5 / 8;
	int regionId = GRIDMANAGER->regionsTile[(int)nowRegionPos.x][(int)nowRegionPos.y].regionsIds;

	if (regionId != -1)
	{
		if (moveNodeStack.empty() == false)
		{
			if (moveNodeStack.top()->regionId == regionId)
			{
				moveNodeStack.pop();
				grid->Astar(4, 4);
			}
			if (moveNodeStack.empty() == false)
			{
				if (grid->moveStack2.empty() == true)
					grid->Astar(4, 4);
			}
		}
		else
		{
			if (grid->moveStack2.empty() == true)
				grid->Astar(4, 4);
			//여기선 다음 레기온과의 최단거리 
		}
	}

	if (grid->moveStack2.empty() == false)
	{
		d = Vector2{ (float)(grid->moveStack2.top().x * 8 * 1.5),(float)(grid->moveStack2.top().y * 8 * 1.5) };
		if (d.x != 0 && d.y != 0)
		{
			rot = atan2(d.x - position.x, d.y - position.y);
			if (m_speed < 300)
			{
				m_speed += 5;
			}
			float moveDestX = sin(rot) * DELTA_TIME * m_speed;
			float moveDestY = cos(rot) * DELTA_TIME * m_speed;
			float length = sqrt((d.x - position.x) * (d.x - position.x) + (d.y - position.y) * (d.y - position.y));

			if (length < DELTA_TIME * m_speed)
			{
				position.x = d.x;
				position.y = d.y;

				if (m_dest == position)
				{

				}
				else
				{
					grid->Astar(4, 4);
				}
				//astarTimer = 0.1f;
				astarTimer -= DELTA_TIME;
				if (grid->moveStack2.empty() == false)
				{
					grid->moveStack2.pop();

					if (grid->moveStack2.empty() && moveNodeStack.empty())
					{
						d = { 0,0 };
						m_dest = { 0,0 };
						m_speed = 0;
						if (buildIndex != 0)
							m_isBuild = true;
					}
				}
			}
			else
			{
				imgRot = rot;
				position.x += moveDestX;
				position.y += moveDestY;
			}
		}
		else
		{
			m_speed = 0;
		}
	}

	grid->Update(true);

}

void SpaceConstructionVehicle::Update()
{
	grid->Update();

	if (isdeath == true)
	{
		ObjectDestroyed();
	}
	if (m_hp <= 0 && isdeath == false)
	{
		SOUNDMANAGER->play("tscdth00", 0.5f);
		isdeath = true;
		EFFECTMANAGER->AddEffect("bang2Effect", { position.x - 80,position.y - 100 }, 1.1, 0.07f);
	}

	IMAGEMANAGER->FogUpdate(position, 30);
	if (m_nowBuild == nullptr)
	{
		if (GRIDMANAGER->regionsTile[(int)(position.x / 1.5f / 8.f)][(int)(position.y / 1.5f / 8.f)].isBuildTag != 0)
		{
			if (GRIDMANAGER->regionsTile[(int)(position.x / 1.5f / 8.f)][(int)(position.y / 1.5f / 8.f)].isBuildTag != grid->gridTag)
			{
				position.x += cos(5) * DELTA_TIME * 100;
				position.y += sin(5) * DELTA_TIME * 100;
				imgRot = 5;
			}
		}
	}
	else
	{
		if (isFA == true)
		{
			BuildTimer += DELTA_TIME;

			if (BuildTimer > 3)
			{
				isFA = false;
			}
			else
			{
				float rrr = atan2(m_nowBuild->position.x - position.x, m_nowBuild->position.y - position.y);
				imgRot = rrr > 0 ? 3.141592 - rrr : -3.141592 - rrr;
			}
		}
		else
		{
			if (buildDest.x == 0 && buildDest.y == 0)
			{
				float rX = rand() % 200 - 100 + m_nowBuild->position.x;
				float rY = rand() % 200 - 100 + m_nowBuild->position.y;
				buildDest.x = rX;
				buildDest.y = rY;
			}

			float rrr = atan2(buildDest.y - position.y, buildDest.x - position.x);
			if (m_speed2 < 300)
			{
				m_speed2 += 5;
			}
			float moveDestX = cos(rrr) * DELTA_TIME * m_speed2;
			float moveDestY = sin(rrr) * DELTA_TIME * m_speed2;
			float length = sqrt((buildDest.x - position.x) * (buildDest.x - position.x) + (buildDest.y - position.y) * (buildDest.y - position.y));

			if (length < DELTA_TIME* m_speed2)
			{
				float rrr = atan2(buildDest.x - position.x, buildDest.y - position.y);
				imgRot = rrr > 0 ? 3.141592 - rrr : -3.141592 - rrr;
				position.x = buildDest.x;
				position.y = buildDest.y;

				BuildTimer = 0;
				buildDest = { 0,0 };
				m_speed2 = 0;
				isFA = true;
			}
			else
			{
				float rrr = atan2(buildDest.x - position.x, buildDest.y - position.y);
				imgRot = rrr > 0 ? 3.141592 - rrr : -3.141592 - rrr;
				position.x += moveDestX;
				position.y += moveDestY;
			}
		}
	}

	if (m_isClick == false)
	{
		if (m_isBuild == false)
		{
			buildIndex = 0;
		}
		page = 0;
	}

	if (m_dest.x != 0 && m_dest.y != 0)
	{
		if (m_speed < 300)
		{
			m_speed += 300 * DELTA_TIME;
		}
		Move();
	}
	else
	{
		m_speed = 0;
	}
	isa = false;

	if (me != nullptr)
	{
		if (m_dest.x == 0 && m_dest.y == 0)
		{
			if (isMine == false)
			{
				if (me->nowMineUnit == nullptr)
				{
					me->nowMineUnit = this;
				}
				else if (me->nowMineUnit != this)
				{
					float fDest = 10000;
					auto lastMe = me;
					for (auto iter : player->resrouces)
					{
						float dest = sqrt((iter->position.x - position.x) * (iter->position.x - position.x)
							+ (iter->position.y - position.y) * (iter->position.y - position.y));
						if (fDest > dest && dest < 150)
						{
							if (typeid(*iter).name() == typeid(Mineral).name())
							{
								auto a = dynamic_cast<Mineral*>(iter);
								if (a->nowMineUnit == nullptr)
								{
									fDest = dest;
									me = a;
								}
							}
						}
					}
					if (lastMe != me)
					{
						m_dest = me->position;
						player->Astar(position, m_dest, this);
					}
				}
			}
			else if (isMine == true)
			{
				mineResertTimer += DELTA_TIME;
				imgRot = atan2(m_command->position.x - position.x, m_command->position.y - position.y);


				isa = true;

				if (mineResertTimer > 0.5f)
				{
					player->m_mineral += 8;
					m_dest = me->position;
					isMine = false;
					mineResertTimer = 0;
					player->Astar(position, m_dest, this);
				}
			}
		}

		if (me->nowMineUnit == this)
		{
			imgRot = atan2(me->position.x - position.x, me->position.y - position.y);

			isa = true;
			mineTimer += DELTA_TIME;
			if (mineTimer > 3)
			{
				me->MineMineral();
				isMine = true;
				mineTimer = 0;
				for (auto iter : player->m_builds)
				{
					if (typeid(*iter).name() == typeid(CommandCenter).name())
					{
						me->nowMineUnit = nullptr;
						m_command = iter;
						m_dest = iter->position;
						player->Astar(position, m_dest, this);
					}
				}
			}
		}
	}


	BuildObject();
	CollisionUpdate();
}

void SpaceConstructionVehicle::Render()
{
	float rr = 8.f / 3.141592 * abs(imgRot);
	bool isR = false;

	ImageDirection();

	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle(position, 12, 9);
	}

	if (isa == true)
	{
		if (imgRot <= 0)
		{
			isR = true;
		}
	}
	else
	{
		if (imgRot <= 0)
		{
			isR = true;
		}
	}

	if (m_nowBuild == nullptr || m_speed2 != 0)
	{
		IMAGEMANAGER->CenterRenderBlendBlack(m_idleImage[(int)rr], position, 1.5f, 0, isR);
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(m_actionImage_1[(int)rr], position, 1.5f, 0, isR);
		sparkImg->CenterRenderBlendBlack({ position.x - 45 + cosf(imgRot - 3.141592 / 2) * 50,position.y - 50 + sinf(imgRot - 3.141592 / 2) * 50 }, 1.5f, 0, false);
	}

	m_isClick = false;
}

void SpaceConstructionVehicle::UIRender()
{
	Command();
	m_isClick = true;

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("wirefram0106"), { 319,680 }, 1.5, 0, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 285,730 }, { 12,12 }, { 0,255,0,1 });


	if (page == 1)
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			page = 0;
		}
		if (KEYMANAGER->GetOnceKeyDown('A') && player->buildList[Player::BuildList::eBarrack] == true)
		{
			if (player->m_mineral - 150 >= 0)
			{
				buildIndex = eAcademy;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
		if (KEYMANAGER->GetOnceKeyDown('B') && player->buildList[Player::BuildList::eCommandCenter] == true)
		{
			if (player->m_mineral - 150 >= 0)
			{
				buildIndex = eBarrack;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
		if (KEYMANAGER->GetOnceKeyDown('C'))
		{
			if (player->m_mineral - 400 >= 0)
			{
				buildIndex = eCommandCenter;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
		if (KEYMANAGER->GetOnceKeyDown('E') && player->buildList[Player::BuildList::eBarrack] == true)
		{
			if (player->m_mineral - 125 >= 0)
			{
				buildIndex = eEngin;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
		if (KEYMANAGER->GetOnceKeyDown('S') && player->buildList[Player::BuildList::eCommandCenter] == true)
		{
			if (player->m_mineral - 100 >= 0)
			{
				buildIndex = eDepot;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
	}
	else if (page == 2)
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			page = 0;
		}
		if (KEYMANAGER->GetOnceKeyDown('F') && player->buildList[Player::BuildList::eBarrack] == true)
		{
			if (player->m_mineral - 200 >= 0)
			{
				buildIndex = eFactory;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
		if (KEYMANAGER->GetOnceKeyDown('S') && player->buildList[Player::BuildList::eFactory] == true)
		{
			if (player->m_mineral - 150 >= 0)
			{
				buildIndex = eStarport;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
		if (KEYMANAGER->GetOnceKeyDown('A') && player->buildList[Player::BuildList::eFactory] == true)
		{
			if (player->m_mineral - 100 >= 0)
			{
				buildIndex = eArmory;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
		if (KEYMANAGER->GetOnceKeyDown('I') && player->buildList[Player::BuildList::eArmory] == true)
		{
			if (player->m_mineral - 100 >= 0)
			{
				buildIndex = eScience;
				page = 4;
			}
			else
			{
				SOUNDMANAGER->play("taderr00", 0.5f);
			}
		}
	}
	else if (page == 4)
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			buildIndex = 0;
			if (m_nowBuild != nullptr)
			{
				page = 0;
				m_nowBuild = nullptr;
			}
			else
			{
				page = 0;
			}
		}
	}

	BuildingConstruction();
	BuildCommandUI();
}

void SpaceConstructionVehicle::Attack()
{
}

void SpaceConstructionVehicle::ResetParam()
{
	if (me != nullptr)
	{
		me->nowMineUnit = nullptr;
	}
	if (m_nowBuild == nullptr)
	{
		buildIndex = 0;
		m_isBuild = false;

	}
}

void SpaceConstructionVehicle::BuildingConstruction()
{
	if (page == 4)
	{
		switch (buildIndex)
		{
		case eCommandCenter:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("control0000"), {
				(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
				(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(4, 3);
			break;
		case eBarrack:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x - 50) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(4, 3);
			break;
		case eFactory:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("factory0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(4, 3);
			break;
		case eAcademy:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("academy0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(3, 2);
			break;
		case eEngin:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("weaponpl0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(4, 3);
			break;
		case eDepot:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("depot0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(3, 2);
			break;
		case eStarport:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("starport0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(4, 3);
			break;
		case eArmory:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("chemlab0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(3, 2);
			break;
		case eScience:
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("research0000"), {
			(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) ,
			(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
			RectSzie(3, 2);

			break;
		}
	}
}

void SpaceConstructionVehicle::CollisionUpdate()
{
	clickRect = { int(position.x - 20) , int(position.y - 20) , int(position.x + 20) , int(position.y + 20) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}

void SpaceConstructionVehicle::BuildObject()
{
	if (m_isBuild == true)
	{
		if (moveNodeStack.empty() == true && grid->moveStack2.empty())
		{
			switch (buildIndex)
			{
			case eCommandCenter:
				player->m_mineral -= 400;
				m_nowBuild = new CommandCenter;
				m_nowBuild->SetPlayer(player);
				m_isBuild = false;
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eBarrack:
				player->m_mineral -= 150;
				m_nowBuild = new Barrack;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eFactory:
				player->m_mineral -= 150;
				m_nowBuild = new Factory;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eAcademy:
				player->m_mineral -= 150;
				m_nowBuild = new Academy;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eEngin:
				player->m_mineral -= 125;
				m_nowBuild = new EngineeringBay;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eDepot:
				player->m_mineral -= 100;
				m_nowBuild = new Depot;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eStarport:
				player->m_mineral -= 150;
				m_nowBuild = new Starport;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eArmory:
				player->m_mineral -= 150;
				m_nowBuild = new Armory;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			case eScience:
				player->m_mineral -= 150;
				m_nowBuild = new ScienceFacility;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter",
					buildPos.x + 1,
					buildPos.y
					, 0);
				break;
			}
		}
	}

	if (m_nowBuild != nullptr)
	{
		m_nowBuild->AddBuild(this);
		if (m_nowBuild->isComplete == true)
		{
			m_nowBuild = nullptr;
		}
	}

}

void SpaceConstructionVehicle::ImageDirection()
{
}

void SpaceConstructionVehicle::Command()
{
	if (m_nowBuild == nullptr)
	{
		if (page == 0)
		{
			if (KEYMANAGER->GetOnceKeyDown('B')) // 일반 건물
			{
				page = 1;
			}
			if (KEYMANAGER->GetOnceKeyDown('V'))// 특수 건물
			{
				page = 2;
			}
		}
		else if (page == 1)
		{

		}
	}
	else
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			m_nowBuild = nullptr;
			m_isBuild = false;
			page = 0;
		}
	}
}

void SpaceConstructionVehicle::BuildActive()
{
	Vector2 mousePos = { _ptMouse.x,_ptMouse.y };
	mousePos = mousePos + IMAGEMANAGER->GetCameraPosition();
	mousePos.x = (int)mousePos.x;
	mousePos.y = (int)mousePos.y;

	//switch (buildIndex)
	//{
	//case eCommandCenter:
	//	IMAGEMANAGER->DrawRect({
	//		(float)((int)(mousePos.x) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x),
	//		(float)((int)(mousePos.y) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) }, {
	//		(float)((int)(mousePos.x) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x) + (float)IMAGEMANAGER->FindImage("control0000")->GetWidth() * 1.5f ,
	//		(float)((int)(mousePos.y) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) + (float)IMAGEMANAGER->FindImage("control0000")->GetHeight() }
	//		, { 0,1,0,0.5 }, 1);
	//	IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("control0000"),
	//		{
	//			(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5)),
	//			(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50
	//		}, 1.5, 0);
	//	break;
	//case eBarrack:
	//	IMAGEMANAGER->DrawRect({
	//		(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x),
	//		(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) }, {
	//		(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x) + (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetWidth() ,
	//		(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) + (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetHeight()
	//		}, { 0,1,0,0.5 }, 1);
	//	IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"),
	//		{
	//			(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50,
	//			(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50
	//		}, 1.5, 0);
	//	break;
	//}
}

void SpaceConstructionVehicle::BuildCommandUI()
{
	if (m_nowBuild == nullptr)
	{
		if (page == 0)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0230"), { UIPosition[2].x - 1 ,UIPosition[2].y - 2 }, 1.7, 0, 0);


			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0229"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0228"), { UIPosition[0].x - 1 ,UIPosition[0].y - 2 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[4].x + 25,UIPosition[4].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0231"), { UIPosition[4].x  ,UIPosition[4].y + 4 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0232"), { UIPosition[3].x - 2 ,UIPosition[3].y - 2 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0234"), { UIPosition[6].x + 3,UIPosition[6].y + 5 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[7].x + 25,UIPosition[7].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0235"), { UIPosition[7].x + 3,UIPosition[7].y + 5 }, 1.7, 0, 0);
		}
		else if (page == 1)
		{

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0106"), { UIPosition[0].x - 4 ,UIPosition[0].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0110"), { UIPosition[2].x - 4 ,UIPosition[2].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
			if (player->buildList[Player::BuildList::eCommandCenter] == true)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0111"), { UIPosition[3].x - 4 ,UIPosition[3].y - 7 }, 1.7, 0, 0);
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0109"), { UIPosition[1].x - 4 ,UIPosition[1].y - 7 }, 1.7, 0, 0);

			}
			else
			{
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0109"), { UIPosition[1].x - 4 ,UIPosition[1].y - 7 }, 1.7, 0, 0);
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0111"), { UIPosition[3].x - 4 ,UIPosition[3].y - 7 }, 1.7, 0, 0);
			}


			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[4].x + 25,UIPosition[4].y + 25 }, 1.7, 0, 0);
			if (player->buildList[Player::BuildList::eBarrack] == true)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0122"), { UIPosition[4].x - 4 ,UIPosition[4].y - 7 }, 1.7, 0, 0);
			}
			else
			{
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0122"), { UIPosition[4].x - 4 ,UIPosition[4].y - 7 }, 1.7, 0, 0);
			}

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0124"), { UIPosition[5].x - 4 ,UIPosition[5].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[7].x + 25,UIPosition[7].y + 25 }, 1.7, 0, 0);

			if (player->buildList[Player::BuildList::eBarrack] == true)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0112"), { UIPosition[6].x - 4 ,UIPosition[6].y - 7 }, 1.7, 0, 0);
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0125"), { UIPosition[7].x - 2 ,UIPosition[7].y }, 1.7, 0, 0);

			}
			else
			{
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0112"), { UIPosition[6].x - 4 ,UIPosition[6].y - 7 }, 1.7, 0, 0);
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0125"), { UIPosition[7].x - 2 ,UIPosition[7].y }, 1.7, 0, 0);
			}

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
		}
		else if (page == 2)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
			if (player->buildList[Player::BuildList::eBarrack] == true)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0113"), { UIPosition[0].x - 4 ,UIPosition[0].y - 7 }, 1.7, 0, 0);
			}
			else
			{
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0113"), { UIPosition[0].x - 4 ,UIPosition[0].y - 7 }, 1.7, 0, 0);
			}
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);

			if (player->buildList[Player::BuildList::eFactory] == true)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0114"), { UIPosition[1].x - 4 ,UIPosition[1].y - 7 }, 1.7, 0, 0);
			}
			else
			{
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0114"), { UIPosition[1].x - 4 ,UIPosition[1].y - 7 }, 1.7, 0, 0);
			}
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);

			if (player->buildList[Player::BuildList::eFactory] == true)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0116"), { UIPosition[2].x - 4 ,UIPosition[2].y - 7 }, 1.7, 0, 0);
			}
			else
			{
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0116"), { UIPosition[2].x - 4 ,UIPosition[2].y - 7 }, 1.7, 0, 0);
			}
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
			if (player->buildList[Player::BuildList::eFactory] == true)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0123"), { UIPosition[3].x - 4 ,UIPosition[3].y - 7 }, 1.7, 0, 0);
			}
			else
			{
				IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0123"), { UIPosition[3].x - 4 ,UIPosition[3].y - 7 }, 1.7, 0, 0);
			}
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
		}
		else if (page == 3)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
		}
	}
	else
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
	}
}

void SpaceConstructionVehicle::RectSzie(int sizeX, int sizeY)
{
	isBuildAble_ = true;
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
		{
			int searchX = (int)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) + i * 32 * 1.5f;
			int searchY = (int)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) + j * 32 * 1.5f;
			searchX = searchX / 1.5f / 8;
			searchY = searchY / 1.5f / 8;
			bool isBuildAble = false;
			for (int x2 = searchX; x2 < searchX + 4; x2++)
			{
				for (int y2 = searchY; y2 < searchY + 4; y2++)
				{
					if (GRIDMANAGER->regionsTile[x2][y2].isBuildTag != 0)
					{
						isBuildAble = true;
						isBuildAble_ = false;
						break;
					}
				}
				if (isBuildAble == true)
					break;
			}
			if (isBuildAble == false)
			{
				IMAGEMANAGER->DrawRect({
					(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) + i * 32 * 1.5f - IMAGEMANAGER->GetCameraPosition().x,
					(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) + j * 32 * 1.5f - IMAGEMANAGER->GetCameraPosition().y }, {
					(float)((int)(_ptMouse.x + IMAGEMANAGER->GetCameraPosition().x) / (int)(32.f * 1.5f) * (32.f * 1.5)) + i * 32 * 1.5f + float(32 * 1.5) - IMAGEMANAGER->GetCameraPosition().x ,
					(float)((int)(_ptMouse.y + IMAGEMANAGER->GetCameraPosition().y) / (int)(32.f * 1.5f) * (32.f * 1.5)) + j * 32 * 1.5f + float(32 * 1.5) - IMAGEMANAGER->GetCameraPosition().y
					}, { 0,1,0,0.5 }, 1);
			}
			else
			{
				IMAGEMANAGER->DrawRect({
					(float)((_ptMouse.x + (int)IMAGEMANAGER->GetCameraPosition().x % (int)(32 * 1.5f)) / (int)(32.f * 1.5f) * (32.f * 1.5)) + i * 32 * 1.5f ,
					(float)((_ptMouse.y + (int)IMAGEMANAGER->GetCameraPosition().y % (int)(32 * 1.5f)) / (int)(32.f * 1.5f) * (32.f * 1.5)) + j * 32 * 1.5f }, {
					(float)((_ptMouse.x + (int)IMAGEMANAGER->GetCameraPosition().x % (int)(32 * 1.5f)) / (int)(32.f * 1.5f) * (32.f * 1.5)) + float(i * 32 * 1.5f + 32 * 1.5) ,
					(float)((_ptMouse.y + (int)IMAGEMANAGER->GetCameraPosition().y % (int)(32 * 1.5f)) / (int)(32.f * 1.5f) * (32.f * 1.5)) + float(j * 32 * 1.5f + 32 * 1.5)
					}, { 1,0,0,0.5 }, 1);
			}
		}
	}
}
