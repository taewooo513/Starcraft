#include "stdafx.h"
#include "ScienceFacility.h"

ScienceFacility::ScienceFacility()
{
}

ScienceFacility::~ScienceFacility()
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

void ScienceFacility::Init()
{
	grid = GRIDMANAGER->AddGrid(this, 14, 8, 3, 2, 2, 1);
	grid->gridTag = 3;

	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("tbldlrg0000");
	m_buildImage[1] = IMAGEMANAGER->FindImage("tbldlrg0001");
	m_buildImage[2] = IMAGEMANAGER->FindImage("tbldlrg0002");
	m_buildImage[3] = IMAGEMANAGER->FindImage("research0001");

	m_maxCompleteTime = 10.5f;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1000;
	m_hp = 1;
}

void ScienceFacility::Update()
{
	IMAGEMANAGER->FogUpdate(position, 30);

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}

void ScienceFacility::Render()
{
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x  ,position.y }, 50, 30);
	}
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x  ,position.y }, 1.5, 0);
		}
		else
		{
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x  ,position.y }, 1.5, 0);
		}
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("research0000"), { position.x ,position.y }, 1.5, 0);
	}
	m_isClick = false;
}

void ScienceFacility::Release()
{
}

void ScienceFacility::UIRender()
{
	m_isClick = true;

	if (m_buildIndex < 4)
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
	}

	IMAGEMANAGER->DirectDrawText(L"Terran Science Facility", { 400,625 }, { 15,15 });

}
