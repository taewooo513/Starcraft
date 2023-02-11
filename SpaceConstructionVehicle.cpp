#include "stdafx.h"
#include "SpaceConstructionVehicle.h"
#include "Barrack.h"
#include "CommandCenter.h"
SpaceConstructionVehicle::SpaceConstructionVehicle()
{
}

SpaceConstructionVehicle::~SpaceConstructionVehicle()
{
}

void SpaceConstructionVehicle::Init()
{
	astarTimer = 0.1;
	grid = GRIDMANAGER->AddGrid(this, 1, 1, 20, 20, -1, -2);
	grid->gridTag = rand() % 100 + 100;
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

	m_idleImage[0] = IMAGEMANAGER->FindImage("scv_idle_1");
	m_idleImage[1] = IMAGEMANAGER->FindImage("scv_idle_2");
	m_idleImage[2] = IMAGEMANAGER->FindImage("scv_idle_3");
	m_idleImage[3] = IMAGEMANAGER->FindImage("scv_idle_4");
	m_idleImage[4] = IMAGEMANAGER->FindImage("scv_idle_5");
	m_idleImage[5] = IMAGEMANAGER->FindImage("scv_idle_6");
	m_idleImage[6] = IMAGEMANAGER->FindImage("scv_idle_7");
	m_idleImage[7] = IMAGEMANAGER->FindImage("scv_idle_8");
	m_idleImage[8] = IMAGEMANAGER->FindImage("scv_idle_9");

	m_buttom = IMAGEMANAGER->FindImage("tcmdbtns0000");
	d = { 0,0 };
	randomMoveTime = 0;
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
				grid->Astar();
			}
			if (moveNodeStack.empty() == false)
			{
				if (grid->moveStack2.empty() == true)
					grid->Astar();
			}
		}
		else
		{
			if (grid->moveStack2.empty() == true)
				grid->Astar(); //여기선 다음 레기온과의 최단거리 
		}
	}

	if (grid->moveStack2.empty() == false)
	{
		d = Vector2{ (float)(grid->moveStack2.top().x * 8 * 1.5),(float)(grid->moveStack2.top().y * 8 * 1.5) };
		rot = atan2(d.x - position.x, d.y - position.y);
		if (d.x != 0 && d.y != 0)
		{
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
				if (astarTimer < 0)
				{
					grid->Astar();
					astarTimer = 0.1f;
				}
				astarTimer -= DELTA_TIME;
				if (grid->moveStack2.empty() == false)
				{
					grid->moveStack2.pop();

					if (grid->moveStack2.empty() && moveNodeStack.empty())
					{
						d = { 0,0 };
						m_dest = { 0,0 };
						m_speed = 0;
						m_isBuild = true;
					}
				}
			}
			else
			{
				position.x += moveDestX;
				position.y += moveDestY;

			}
		}
		else
		{
			m_speed = 0;
		}
	}
}

void SpaceConstructionVehicle::Update()
{
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
	BuildObject();
	CollisionUpdate();
}

void SpaceConstructionVehicle::Render()
{
	bool isR = false;
	ImageDirection();

	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle(position, 12, 9);
	}

	if (rot <= 0)
	{
		isR = true;
	}

	if (m_nowBuild == nullptr)
	{
		IMAGEMANAGER->CenterRenderBlendBlack(m_idleImage[(int)m_dir], position, 1.5f, 0, isR);
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(m_actionImage_1[(int)m_dir], position, 1.5f, 0, isR);
	}

	m_isClick = false;

}

void SpaceConstructionVehicle::UIRender()
{
	Command();
	m_isClick = true;

	if (page == 1)
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			page = 0;
		}
		if (KEYMANAGER->GetOnceKeyDown('B'))
		{
			buildIndex = eBarrack;
			page = 2;
		}
		if (KEYMANAGER->GetOnceKeyDown('C'))
		{
			buildIndex = eCommandCenter;
			page = 2;
		}
	}
	else if (page == 3)
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
				page = 1;
			}
		}
	}

	BuildingConstruction();
	BuildCommandUI();
}

void SpaceConstructionVehicle::Attack()
{
}

void SpaceConstructionVehicle::BuildingConstruction()
{
	if (page == 2)
	{
		switch (buildIndex)
		{
		case eCommandCenter:
			IMAGEMANAGER->DrawRect({
				(float)((_ptMouse.x) / (int)(32.f * 1.5f) * (32.f * 1.5)),
				(float)((_ptMouse.y) / (int)(32.f * 1.5f) * (32.f * 1.5)) }, {
				(float)((_ptMouse.x) / (int)(32.f * 1.5f) * (32.f * 1.5)) + (float)IMAGEMANAGER->FindImage("control0000")->GetWidth() * 1.5f ,
				(float)((_ptMouse.y) / (int)(32.f * 1.5f) * (32.f * 1.5)) + (float)IMAGEMANAGER->FindImage("control0000")->GetHeight() });
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("control0000"), { (float)(_ptMouse.x / (int)(32.f * 1.5f) * (32.f * 1.5)) + IMAGEMANAGER->GetCameraPosition().x, (float)(_ptMouse.y / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 + IMAGEMANAGER->GetCameraPosition().y }, 1.5, 0);
			break;
		case eBarrack:
			IMAGEMANAGER->DrawRect({ (float)(_ptMouse.x / (int)(32.f * 1.5f) * (32.f * 1.5)),(float)(_ptMouse.y / (int)(32.f * 1.5f) * (32.f * 1.5)) },
			{
				(float)(_ptMouse.x / (int)(32.f * 1.5f) * (32.f * 1.5)) + (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetWidth() ,
				(float)(_ptMouse.y / (int)(32.f * 1.5f) * (32.f * 1.5)) + (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetHeight()
			});
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"), { (float)(_ptMouse.x / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50,(float)(_ptMouse.y / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
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
		page = 0;
		if (moveNodeStack.empty() == true && grid->moveStack2.empty())
		{
			switch (buildIndex)
			{
			case eCommandCenter:
				m_nowBuild = new CommandCenter;
				m_nowBuild->SetPlayer(player);
				m_isBuild = false;
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "CommandCenter", (float)((int)position.x / (int)(32.f * 1.5f) * (32.f * 1.5)) + 32, (float)((int)position.y / (int)(32.f * 1.5f) * (32.f * 1.5)), 0);
				break;
			case eBarrack:
				m_nowBuild = new Barrack;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				OBJECTMANAGER->AddObject(m_nowBuild, "Barrack", (float)((int)position.x / (int)(32.f * 1.5f) * (32.f * 1.5)) + 32, (float)((int)position.y / (int)(32.f * 1.5f) * (32.f * 1.5)), 0);
				buildIndex = 0;
				break;
			}
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
			if (KEYMANAGER->GetOnceKeyDown('C'))// 특수 건물
			{
				//page = 2; 
			}
		}
		else if (page == 1)
		{
			if (KEYMANAGER->GetOnceKeyDown('B')) // 베럭
			{
				buildIndex = eBarrack;
				page = 3;
			}
			if (KEYMANAGER->GetOnceKeyDown('C')) //커멘드 센터
			{
				buildIndex = eCommandCenter;
				page = 3;
			}
		}
	}
	else
	{

	}
}

void SpaceConstructionVehicle::BuildActive()
{
	Vector2 mousePos = { _ptMouse.x,_ptMouse.y };
	mousePos = mousePos + IMAGEMANAGER->GetCameraPosition();
	mousePos.x = (int)mousePos.x;
	mousePos.y = (int)mousePos.y;

	switch (buildIndex)
	{
	case eCommandCenter:
		IMAGEMANAGER->DrawRect({
			(float)((int)(mousePos.x) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x),
			(float)((int)(mousePos.y) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) }, {
			(float)((int)(mousePos.x) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x) + (float)IMAGEMANAGER->FindImage("control0000")->GetWidth() * 1.5f ,
			(float)((int)(mousePos.y) / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) + (float)IMAGEMANAGER->FindImage("control0000")->GetHeight() });
		IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("control0000"),
			{
				(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5)),
				(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50
			}, 1.5, 0);
		break;
	case eBarrack:
		IMAGEMANAGER->DrawRect({
			(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x),
			(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) }, {
			(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().x) + (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetWidth() ,
			(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5) - IMAGEMANAGER->GetCameraPosition().y) + (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetHeight()
			});
		IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"),
			{
				(float)((int)mousePos.x / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50,
				(float)((int)mousePos.y / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50
			}, 1.5, 0);
		break;
	}
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
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0109"), { UIPosition[1].x - 4 ,UIPosition[1].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0110"), { UIPosition[2].x - 4 ,UIPosition[2].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0111"), { UIPosition[3].x - 4 ,UIPosition[3].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[4].x + 25,UIPosition[4].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0122"), { UIPosition[4].x - 4 ,UIPosition[4].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0124"), { UIPosition[5].x - 4 ,UIPosition[5].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0112"), { UIPosition[6].x - 4 ,UIPosition[6].y - 7 }, 1.7, 0, 0);

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[7].x + 25,UIPosition[7].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0125"), { UIPosition[7].x - 2 ,UIPosition[7].y }, 1.7, 0, 0);
		}
		else if (page == 3)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
			BuildActive();
		}
	}
	else
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
	}
}
