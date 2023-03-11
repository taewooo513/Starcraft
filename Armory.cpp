#include "stdafx.h"
#include "Armory.h"

Armory::Armory()
{
}

Armory::~Armory()
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

void Armory::Init()
{
	idleP[0] = IMAGEMANAGER->AddImageVectorCopy("Advisor1P");
	idleP[1] = IMAGEMANAGER->AddImageVectorCopy("Advisor2P");
	idleP[2] = IMAGEMANAGER->AddImageVectorCopy("Advisor3P");
	idleP[3] = IMAGEMANAGER->AddImageVectorCopy("Advisor4P");
	idleP[0]->Setting(0.1f, false);
	idleP[1]->Setting(0.1f, false);
	idleP[2]->Setting(0.1f, false);
	idleP[3]->Setting(0.1f, false);
	grid = GRIDMANAGER->AddGrid(this, 14, 8, 3, 2, 2, 1);
	grid->gridTag = 3;

	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("tbldmed0000");
	m_buildImage[1] = IMAGEMANAGER->FindImage("tbldmed0001");
	m_buildImage[2] = IMAGEMANAGER->FindImage("tbldmed0002");
	m_buildImage[3] = IMAGEMANAGER->FindImage("chemlab0001");

	m_maxCompleteTime = 10.5f;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1000;
	m_hp = 1;
}

void Armory::Update()
{
	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}

void Armory::Render()
{
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x  ,position.y }, 50, 30);
	}
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x  ,position.y }, 1.5, 0);
		else
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x  ,position.y }, 1.5, 0);
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("chemlab0000"), { position.x ,position.y }, 1.5, 0);
	}
	m_isClick = false;
}

void Armory::Release()
{
}

void Armory::UIRender()
{
	m_isClick = true;

	if (m_buildIndex < 4)
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->DirectDrawText(L"Under Construction", { 420,660 }, { 15,15 }, { 200,200,200,0.8 });

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("coolTimeBar"), { 505,694 }, 0.8, 0, 0);
		for (int i = 0; i < 41; i++)
		{
			if (m_maxCompleteTime / 41 * i < m_completeTime)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("Coll"), { float(505 + i * 4),694 }, 0.8, 0, 0);
			}
		}
	}

	IMAGEMANAGER->DirectDrawText(L"Terran Armory", { 400,625 }, { 15,15 });
}
