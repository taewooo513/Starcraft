#include "stdafx.h"
#include "Academy.h"

Academy::Academy()
{
}

Academy::~Academy()
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

void Academy::Init()
{
	grid = GRIDMANAGER->AddGrid(this, 12, 10, 3, 2, -5, -5);
	idleP[0] = IMAGEMANAGER->AddImageVectorCopy("Advisor1P");
	idleP[1] = IMAGEMANAGER->AddImageVectorCopy("Advisor2P");
	idleP[2] = IMAGEMANAGER->AddImageVectorCopy("Advisor3P");
	idleP[3] = IMAGEMANAGER->AddImageVectorCopy("Advisor4P");
	idleP[0]->Setting(0.1f, false);
	idleP[1]->Setting(0.1f, false);
	idleP[2]->Setting(0.1f, false);
	idleP[3]->Setting(0.1f, false);
	grid->gridTag = 3;

	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("tbldmed0000");
	m_buildImage[1] = IMAGEMANAGER->FindImage("tbldmed0001");
	m_buildImage[2] = IMAGEMANAGER->FindImage("tbldmed0002");
	m_buildImage[3] = IMAGEMANAGER->FindImage("academy0001");
	m_shadImage[0] = IMAGEMANAGER->FindImage("tb3shad0000");
	m_shadImage[1] = IMAGEMANAGER->FindImage("tb3shad0001");
	m_shadImage[2] = IMAGEMANAGER->FindImage("tb3shad0002");

	m_maxCompleteTime = 50.f;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1000;
	m_hp = 1;
}

void Academy::Update()
{
	grid->Update();
	if (addUnitQueue.empty() == false)
	{

	}

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y + 75;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y + 75;
}

void Academy::Render()
{
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x  ,position.y }, 50, 30);
	}
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
		{

			IMAGEMANAGER->RenderBlendBlack(m_shadImage[m_buildIndex], { position.x - 72 ,position.y - 100 }, 1.5, 0);
			IMAGEMANAGER->RenderBlendBlack(m_buildImage[m_buildIndex], { position.x - 72 ,position.y - 100 }, 1.5, 0);
		}
		else
		{
			IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tacshad0001"), { position.x - 72,position.y - 100 }, 1.5, 0);
			IMAGEMANAGER->RenderBlendBlack(m_buildImage[m_buildIndex], { position.x - 72 ,position.y - 100 }, 1.5, 0);
		}
	}
	else
	{
		IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tacshad0000"), { position.x - 72,position.y - 100 }, 1.5, 0);
		IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("academy0000"), { position.x - 72,position.y - 100 }, 1.5, 0);
	}
	m_isClick = false;
}

void Academy::Release()
{
}

void Academy::UIRender()
{
	IMAGEMANAGER->DrawCircle({ position.x ,position.y }, 10, 10);

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
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[3][damageIndex[0]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[1][damageIndex[1]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[2][damageIndex[2]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0112"]->wireImages[0][damageIndex[3]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);

	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 295,730 }, { 12,12 }, { 0,255,0,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran Academy", { 400,625 }, { 15,15 });
}
