#include "stdafx.h"
#include "Machines.h"
#include "Vulture.h"
#include "Factory.h"
Machines::Machines()
{
}

Machines::~Machines()
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

void Machines::Init()
{
	idleshadAnimation = IMAGEMANAGER->AddImageVectorCopy("tmsshad0000");
	idleshadAnimation->Setting(0.2, true);
	idleAnimation = IMAGEMANAGER->AddImageVectorCopy("machines");
	idleAnimation->Setting(0.2, true);
	grid = GRIDMANAGER->AddGrid(this, 10, 4, 4, 2, -7, -5);

	grid->gridTag = 3;

	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("tbldsml0000");
	m_buildImage[1] = IMAGEMANAGER->FindImage("tbldsml0001");
	m_buildImage[2] = IMAGEMANAGER->FindImage("tbldsml0002");
	m_buildImage[3] = IMAGEMANAGER->FindImage("machines0004");
	vi = IMAGEMANAGER->AddImageVectorCopy("machinec0000");
	vi->Setting(0.2, false);
	m_maxCompleteTime = 25;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 750;
	m_hp = 1;
}

void Machines::Update()
{
	IMAGEMANAGER->FogUpdate(position, 30);

	grid->Update();
	if (addUnitQueue.empty() == false)
	{
		if (addUnitQueue.front().timeNow < addUnitQueue.front().maxTime)
		{
			addUnitQueue.front().timeNow += DELTA_TIME;
		}
		else
		{
			if (addUnitQueue.front().unit == 0)
			{
				player->isFastVulture = true;
				addUnitQueue.erase(addUnitQueue.begin());
			}
			else if (addUnitQueue.front().unit == 1)
			{
				player->isSModeUp = true;
				addUnitQueue.erase(addUnitQueue.begin());
			}
		}
	}

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y + 75;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y + 75;
}

void Machines::Render()
{
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x,position.y }, 50, 30);
	}
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
			IMAGEMANAGER->RenderBlendBlack(m_buildImage[m_buildIndex], { position.x - 66,position.y - 110 }, 1.5, 0);
		else
		{
			IMAGEMANAGER->RenderBlendBlack(m_buildImage[m_buildIndex], { position.x - 96  ,position.y - 130 }, 1.5, 0);
			IMAGEMANAGER->RenderBlendBlack(m_buildImage[m_buildIndex], { position.x - 96  ,position.y - 130 }, 1.5, 0);
		}
	}
	else
	{
		vi->CenterRenderBlendBlack({ position.x - 96,position.y - 130 }, 1.5, 0, 0);
		IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tmsshad0000"), { position.x - 96,position.y - 127 }, 1.5, 0);
		IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("machines"), { position.x - 96,position.y - 127 }, 1.5, 0);

	}
	if (!addUnitQueue.empty())
	{
		idleshadAnimation->CenterRenderBlendBlack({ position.x - 96  ,position.y - 130 }, 1.5, 0, 0);
		idleAnimation->CenterRenderBlendBlack({ position.x - 96  ,position.y - 130 }, 1.5, 0, 0);
	}
	m_isClick = false;
}

void Machines::Release()
{
}

void Machines::UIRender()
{
	m_isClick = true;



	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0286"), { UIPosition[5].x - 1 ,UIPosition[5].y - 2 }, 1.7, 0, 0);

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
	else
	{
		if (KEYMANAGER->GetOnceKeyDown('T'))
		{
			addUnitQueue.push_back({ 0,0,60 });
		}
		if (KEYMANAGER->GetOnceKeyDown('S'))
		{
			addUnitQueue.push_back({ 1,0,60 });
		}
		if (KEYMANAGER->GetOnceKeyDown('M'))
		{

		}
		if (player->isFastVulture == false)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0287"), { UIPosition[0].x - 5  ,UIPosition[0].y - 7 }, 1.7, 0, 0);
		}
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0243"), { UIPosition[1].x + 8 ,UIPosition[1].y + 8 }, 1.7, 0, 0);

		if (player->isSModeUp == false)
		{

			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0245"), { UIPosition[2].x - 5 ,UIPosition[2].y - 2 }, 1.7, 0, 0);
		}

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0282"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);


		if (addUnitQueue.size() != 0)
		{
			for (int i = 0; i < 41; i++)
			{
				if (addUnitQueue[0].maxTime / 41 * i < addUnitQueue[0].timeNow)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("Coll"), { float(535 + i * 4),694 }, 0.8, 0, 0);
				}
			}
			IMAGEMANAGER->DirectDrawText(L"Building", { 500,660 }, { 15,15 }, { 255,255,255,1 });
		}
	}

	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0120"]->wireImages[3][damageIndex[0]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0120"]->wireImages[1][damageIndex[1]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0120"]->wireImages[2][damageIndex[2]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0120"]->wireImages[0][damageIndex[3]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);

	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 295,730 }, { 12,12 }, { 0,255,0,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran Machine Shop", { 400,625 }, { 15,15 });
}
