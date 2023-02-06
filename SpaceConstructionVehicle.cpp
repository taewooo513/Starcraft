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
	grid = GRIDMANAGER->AddGrid(this, 0, 0, 10, 10, -1, -2);

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
}

void SpaceConstructionVehicle::Move()
{
}

void SpaceConstructionVehicle::Update()
{
	if (grid->moveStack2.empty() == false)
	{
		d = Vector2{ (float)(grid->moveStack2.top().x * 8 * 1.5),(float)(grid->moveStack2.top().y * 8 * 1.5) };
	}
	else
	{
		if (moveNodeStack.empty() == false)
		{
			grid->Astar();
			if (moveNodeStack.empty() == false)
				if (grid->nowTileRegionId == GRIDMANAGER->regionsTile[(int)this->moveNodeStack.top()->pos.x][(int)this->moveNodeStack.top()->pos.y].regionsIds)
				{
					this->moveNodeStack.pop();
				}
		}
	}
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

			if (grid->moveStack2.empty() == false)
			{
				grid->moveStack2.pop();
			}
			d = { 0,0 };
		}
		else
		{
			position.x += moveDestX;
			position.y += moveDestY;
		}
	}

	//else
	//{
	//	m_speed = 0;
		/*if (m_dest.x != 0 && m_dest.y != 0)
		{
			rot = atan2(m_dest.x - position.x, m_dest.y - position.y);
			if (m_speed < 300)
			{
				m_speed += 5;
			}
			float moveDestX = sin(rot) * DELTA_TIME * m_speed;

			float moveDestY = cos(rot) * DELTA_TIME * m_speed;
			float length = sqrt((m_dest.x - position.x) * (m_dest.x - position.x) + (m_dest.y - position.y) * (m_dest.y - position.y));
			if (length > DELTA_TIME * m_speed)
			{
				position.x += moveDestX;
				position.y += moveDestY;
			}
			else
			{
				m_speed = 0;
			}
		}*/
		//}
	if (m_nowBuild != nullptr)
	{
		if (m_nowBuild->GetIsObjectDestroyed() == true)
		{
			m_nowBuild = nullptr;
		}
		else
		{
			m_nowBuild->AddBuild();
			if (m_spark == nullptr)
			{
				m_spark = EFFECTMANAGER->AddEffect("SCVEffect", { position.x - 35 ,position.y - 40 }, 1.7, 0.1);
			}
			else if (m_spark->m_isEnd == true)
			{
				m_spark->m_isDestroy = true;
				m_spark = nullptr;
			}
			if (m_nowBuild->isComplete == true)
			{
				m_nowBuild = nullptr;
			}
		}
	}
	else
	{
		if (m_spark != nullptr)
		{
			m_spark->m_isDestroy = true;
			m_spark = nullptr;
		}
	}
	clickRect = { int(position.x - 20) , int(position.y - 20) , int(position.x + 20) , int(position.y + 20) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;

	if (m_isBuild == true)
	{
		page = 0;

		if (moveNodeStack.empty() == true)
		{
			switch (buildIndex)
			{
			case eCommandCenter:
				m_nowBuild = new CommandCenter;
				m_nowBuild->SetPlayer(player);
				m_isBuild = false;
				buildIndex = 0;
				OBJECTMANAGER->AddObject(m_nowBuild, "Barrack", (float)((int)position.x / (int)(32.f * 1.5f) * (32.f * 1.5)), (float)((int)position.y / (int)(32.f * 1.5f) * (32.f * 1.5)), 0);
				break;
			case eBarrack:
				m_nowBuild = new Barrack;
				m_isBuild = false;
				m_nowBuild->SetPlayer(player);
				OBJECTMANAGER->AddObject(m_nowBuild, "Barrack", (float)((int)position.x / (int)(32.f * 1.5f) * (32.f * 1.5)), (float)((int)position.y / (int)(32.f * 1.5f) * (32.f * 1.5)), 0);
				buildIndex = 0;
				break;
			}
		}
	}
}

void SpaceConstructionVehicle::Render()
{
	bool isR = false;
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle(position, 12, 9);
	}

	if (rot <= -3.141592 / 9.f * 0.5f && -3.141592 + 3.141592 / 9.f * 0.5f)
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
	/*
		AddImage("cmdicons0106", L"./Resources/Icon/cmdicons0106.bmp"); // 커멘드센터
		AddImage("cmdicons0109", L"./Resources/Icon/cmdicons0109.bmp"); // 서플라이 디포트
		AddImage("cmdicons0110", L"./Resources/Icon/cmdicons0110.bmp"); // 가스
		AddImage("cmdicons0112", L"./Resources/Icon/cmdicons0112.bmp"); // 아카데미
		AddImage("cmdicons0113", L"./Resources/Icon/cmdicons0113.bmp"); // 펙토리
		AddImage("cmdicons0114", L"./Resources/Icon/cmdicons0114.bmp"); // 스타포트
		AddImage("cmdicons0111", L"./Resources/Icon/cmdicons0111.bmp"); // 아이콘
			AddImage("cmdicons0122", L"./Resources/Icon/cmdicons0122.bmp"); // 엔지니어링
	AddImage("cmdicons0124", L"./Resources/Icon/cmdicons0124.bmp"); // 터렛
	*/
	m_isClick = true;
	if (m_nowBuild != nullptr)
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			m_nowBuild = nullptr;
		}
	}
	if (page == 1)
	{
		if (buildIndex == 0)
		{
			if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
			{
				page = 0;
			}
			if (KEYMANAGER->GetOnceKeyDown('B'))
			{
				buildIndex = eBarrack;
				//m_nowBuild = new Barrack;
				//OBJECTMANAGER->AddObject(m_nowBuild, "Barrack", position.x, position.y, 0);
				page = 0;
			}
			if (KEYMANAGER->GetOnceKeyDown('C'))
			{
				buildIndex = eCommandCenter;
				//m_nowBuild = new CommandCenter;
				//OBJECTMANAGER->AddObject(m_nowBuild, "Barrack", position.x, position.y, 0);
				page = 0;
			}

		}
		else
		{
			if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
			{
				buildIndex = 0;
			}
		}
	}

	switch (buildIndex)
	{
	case eCommandCenter:
		IMAGEMANAGER->DrawRect({ (float)(_ptMouse.x / (int)(32.f * 1.5f) * (32.f * 1.5)), (float)(_ptMouse.y / (int)(32.f * 1.5f) * (32.f * 1.5)) }, {
				(float)(_ptMouse.x / (int)(32.f * 1.5f) * (32.f * 1.5)) + (float)IMAGEMANAGER->FindImage("control0000")->GetWidth() * 1.5f,
				(float)(_ptMouse.y / (int)(32.f * 1.5f) * (32.f * 1.5)) + (float)IMAGEMANAGER->FindImage("control0000")->GetHeight()
			});
		IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("control0000"), { (float)(_ptMouse.x / (int)(32.f * 1.5f) * (32.f * 1.5)), (float)(_ptMouse.y / (int)(32.f * 1.5f) * (32.f * 1.5)) - 50 }, 1.5, 0);
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

	if (m_nowBuild == nullptr)
	{
		if (page == 0)
		{
			if (KEYMANAGER->GetOnceKeyDown('B'))
			{
				page = 1;
			}
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
		else
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
	}
	else
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
	}

}

void SpaceConstructionVehicle::Attack()
{
}

void SpaceConstructionVehicle::BuildingConstruction()
{

}
