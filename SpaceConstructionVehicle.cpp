#include "stdafx.h"
#include "SpaceConstructionVehicle.h"

SpaceConstructionVehicle::SpaceConstructionVehicle()
{
}

SpaceConstructionVehicle::~SpaceConstructionVehicle()
{
}

void SpaceConstructionVehicle::Init()
{
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
}

void SpaceConstructionVehicle::Move()
{
}

void SpaceConstructionVehicle::Render()
{
	float rot = atan2(position.x - _ptMouse.x, position.y - _ptMouse.y) * -1;
	bool isR = false;

	if (rot <= -3.141592 / 9.f * 0.5f && -3.141592 + 3.141592 / 9.f * 0.5f)
	{
		isR = true;
	}

	if (abs(rot) >= 0 && abs(rot) < 3.141592 / 9.f * 0.5f)
	{
		m_dir = 0;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f)
	{
		m_dir = 1;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f + 3.141592 / 9.f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 2.f)
	{
		m_dir = 2;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 2.f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 3.f)
	{
		m_dir = 3;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 3.f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 4.f)
	{
		m_dir = 4;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 4.f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 5.f)
	{
		m_dir = 5;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 5.f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 6.f)
	{
		m_dir = 6;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 6.f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 7.f)
	{
		m_dir = 7;
	}
	else if (abs(rot) >= 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 7.f && abs(rot) < 3.141592 / 9.f * 0.5f + 3.141592 / 9.f * 8.f)
	{
		m_dir = 8;
	}

	IMAGEMANAGER->CenterRenderBlendBlack(m_idleImage[(int)m_dir], position, 1.5f, 0, isR);
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

	if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
	{
		page = 0;
	}

	if (page == 0)
	{
		if (KEYMANAGER->GetOnceKeyDown('B'))
		{
			page = 1;
		}

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0230"), { UIPosition[2].x - 1 ,UIPosition[2].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0229"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0228"), { UIPosition[0].x - 1 ,UIPosition[0].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[4].x + 25,UIPosition[4].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0231"), { UIPosition[4].x  ,UIPosition[4].y + 4 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0232"), { UIPosition[3].x - 2 ,UIPosition[3].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0234"), { UIPosition[6].x + 3,UIPosition[6].y + 5 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[7].x + 25,UIPosition[7].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0235"), { UIPosition[7].x + 3,UIPosition[7].y + 5 }, 1.7, 0, 0);
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0106"), { UIPosition[0].x - 4 ,UIPosition[0].y - 7 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0109"), { UIPosition[1].x - 4 ,UIPosition[1].y - 7 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0110"), { UIPosition[2].x - 4 ,UIPosition[2].y - 7 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0111"), { UIPosition[3].x - 4 ,UIPosition[3].y - 7 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[4].x + 25,UIPosition[4].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0122"), { UIPosition[4].x - 4 ,UIPosition[4].y - 7 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0124"), { UIPosition[5].x - 4 ,UIPosition[5].y - 7 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0112"), { UIPosition[6].x - 4 ,UIPosition[6].y - 7 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[7].x + 25,UIPosition[7].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0125"), { UIPosition[7].x - 2 ,UIPosition[7].y  }, 1.7, 0, 0);
	}
	//IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), UIPosition[8], 1.7, 0, 0);
}

void SpaceConstructionVehicle::Attack()
{
}
