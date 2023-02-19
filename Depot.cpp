#include "stdafx.h"
#include "Depot.h"

Depot::Depot()
{
}

Depot::~Depot()
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

void Depot::Init()
{
	idle = IMAGEMANAGER->AddImageVectorCopy("DepotIdle");
	idle->Setting(0.1f, true);
	grid = GRIDMANAGER->AddGrid(this, 12, 9, 3, 2, -6, -5);


	grid->gridTag = 3;

	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("tbldsml0000");
	m_buildImage[1] = IMAGEMANAGER->FindImage("tbldsml0001");
	m_buildImage[2] = IMAGEMANAGER->FindImage("tbldsml0002");
	m_buildImage[3] = IMAGEMANAGER->FindImage("depot0001");

	m_maxCompleteTime = 10.5f;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1000;
	m_hp = 1;
}

void Depot::Update()
{
	grid->Update();
	IMAGEMANAGER->FogUpdate(position, 30);

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y + 95;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y + 75;
}

void Depot::Render()
{
	IMAGEMANAGER->DrawRect({ (float)clickRect.left,(float)clickRect.top }, { (float)clickRect.right,(float)clickRect.bottom });

	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x  ,position.y }, 40, 20);
	}
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x,position.y }, 1.5, 0);
		else
		{
			IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("depotshad1"), { position.x ,position.y }, 1.5, 0);
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x  ,position.y }, 1.5, 0);
		}
		IMAGEMANAGER->DirectDrawText(L"Under Construction", { 420,660 }, { 15,15 }, { 200,200,200,0.8 });
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("depotshad"), { position.x  ,position.y }, 1.5, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("depot0000"), { position.x ,position.y }, 1.5, 0);
		idle->CenterRenderBlendBlack({ position.x - IMAGEMANAGER->FindImage("depot0000")->GetWidth() * 1.5f / 2 ,position.y - IMAGEMANAGER->FindImage("depot0000")->GetHeight() * 1.5f / 2 }, 1.5, 0, 0);
	}
	m_isClick = false;
}

void Depot::Release()
{
}

void Depot::UIRender()
{
	m_isClick = true;

	if (m_buildIndex < 4)
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
	}

	IMAGEMANAGER->DirectDrawText(L"Terran Supply Depot", { 400,625 }, { 15,15 });
	IMAGEMANAGER->DirectDrawText(L"Supplies Used: " + to_wstring(player->m_suff), { 400,675 }, { 12,12 });
	IMAGEMANAGER->DirectDrawText(L"Supplies Provided: 8", { 400,695 }, { 12,12 });
	IMAGEMANAGER->DirectDrawText(L"Total Supplies: " + to_wstring(player->m_maxSuff), { 400,715 }, { 12,12 });
	IMAGEMANAGER->DirectDrawText(L"Supplies Max: 200", { 400,735 }, { 12,12 });
}
