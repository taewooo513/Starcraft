#include "stdafx.h"
#include "CommandCenter.h"

CommandCenter::CommandCenter()
{
}

CommandCenter::~CommandCenter()
{
	if (player != nullptr)
	{
		if (player->m_selectBuild == this)
		{
			player->m_selectBuild = nullptr;
		}
		for (auto iter = player->m_builds.begin(); iter != player->m_builds.end(); iter++)
		{
			if (*iter == this)
			{
				player->m_builds.erase(iter);
				break;
			}
		}
	}
}

void CommandCenter::Init()
{
	GRIDMANAGER->AddGrid(this, 4, 3, 3, 2, 0, 0);

	player->AddBuild(this);

	m_maxCompleteTime = 75.8f;
	m_completeTime = 0;
	m_costM = 400;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1500;
	m_hp = 1;

	idle = IMAGEMANAGER->FindImage("control0000");
	buildImage[0] = IMAGEMANAGER->FindImage("tbldlrg0000");
	buildImage[1] = IMAGEMANAGER->FindImage("tbldlrg0001");
	buildImage[2] = IMAGEMANAGER->FindImage("tbldlrg0002");
	buildImage[3] = IMAGEMANAGER->FindImage("control0001");

}

void CommandCenter::Update()
{
	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };

	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}

void CommandCenter::Render()
{
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
		{
			IMAGEMANAGER->RenderBlendBlack(buildImage[m_buildIndex], { position.x - 20 ,position.y - 20 }, 1.5, 0);
		}
		else
		{
			IMAGEMANAGER->RenderBlendBlack(buildImage[m_buildIndex], { position.x ,position.y - 50 }, 1.5, 0);
		}
	}
	else
	{
		IMAGEMANAGER->RenderBlendBlack(idle, { position.x + 8 ,position.y - 50 }, 1.5, 0);
	}
	IMAGEMANAGER->DrawRect({ (float)clickRect.left, (float)clickRect.top }, { (float)clickRect.right,(float)clickRect.bottom });
}

void CommandCenter::Release()
{
}

void CommandCenter::UIRender()
{
	if (KEYMANAGER->GetOnceKeyDown('S'))
	{

	}

	IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0286"), { UIPosition[5].x - 1 ,UIPosition[5].y - 2 }, 1.7, 0, 0);

	if (m_buildIndex < 4)
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
	}
	else
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0007"), { UIPosition[0].x - 1 ,UIPosition[0].y - 4 }, 1.7, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0282"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
	}
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("wirefram0106"), { 315,670 }, 1.4, 0, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 285,710 }, { 12,12 }, {0,255,0,1});

	IMAGEMANAGER->DirectDrawText(L"Terran Command Center", { 370,625 }, { 15,15 });
}
