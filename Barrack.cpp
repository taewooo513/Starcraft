#include "stdafx.h"
#include "Barrack.h"
#include "Marine.h"
#include "FireBat.h"

Barrack::Barrack()
{
}

Barrack::~Barrack()
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

void Barrack::Init()
{
	workImage = IMAGEMANAGER->AddImageVectorCopy("Barrack_Work");
	workImage->Setting(0.1, true);
	grid = GRIDMANAGER->AddGrid(this, 14, 11, 3, 2, -7, -5);

	grid->gridTag = 3;

	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("tbldlrg0000");
	m_buildImage[1] = IMAGEMANAGER->FindImage("tbldlrg0001");
	m_buildImage[2] = IMAGEMANAGER->FindImage("tbldlrg0002");
	m_buildImage[3] = IMAGEMANAGER->FindImage("tbarrack0001");

	m_maxCompleteTime = 1000.5f;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1000;
	m_hp = 1;
}

void Barrack::Update()
{
	grid->Update();
	IMAGEMANAGER->FogUpdate(position, 30);

	if (addUnitQueue.empty() == false)
	{
		if (addUnitQueue.front().timeNow < addUnitQueue.front().maxTime)
		{
			addUnitQueue.front().timeNow += DELTA_TIME;
		}
		else
		{
			addUnitQueue.front().timeNow = addUnitQueue.front().maxTime;
			if (addUnitQueue.front().unit == 1)
			{
				Marine* marine = new Marine;
				marine->SetPlayer(player);
				OBJECTMANAGER->AddObject(marine, "marine", position.x, position.y + 100, 1);
				addUnitQueue.erase(addUnitQueue.begin());
			}
			else if (addUnitQueue.front().unit == 2)
			{
				FireBat* fireBat = new FireBat;
				fireBat->SetPlayer(player);
				OBJECTMANAGER->AddObject(fireBat, "fireBat", position.x, position.y + 100, 1);
				addUnitQueue.erase(addUnitQueue.begin());
			}
		}
	}

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x + 95;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x + 95;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y + 80;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y + 80;
}

void Barrack::Render()
{
	IMAGEMANAGER->DrawRect({ (float)clickRect.left,(float)clickRect.top }, { (float)clickRect.right,(float)clickRect.bottom });
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
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tbrshad0000"), { position.x ,position.y }, 1.5, 0, 0.5f);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"), { position.x ,position.y }, 1.5, 0);
	}
	if (!addUnitQueue.empty())
	{
		workImage->CenterRenderBlendBlack({
			position.x - (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetWidth() * 1.5f / 2.f,
			position.y - (float)IMAGEMANAGER->FindImage("tbarrack0000")->GetHeight() * 1.5f / 2.f }, 1.5, 0, 0);
	}
	m_isClick = false;
}

void Barrack::Release()
{
}

void Barrack::UIRender()
{
	IMAGEMANAGER->DrawCircle({ position.x ,position.y }, 10, 10);

	m_isClick = true;
	if (addUnitQueue.size() < 5)
	{
		if (KEYMANAGER->GetOnceKeyDown('M'))
		{
			addUnitQueue.push_back({ 1,0,1 });
		}
		if (KEYMANAGER->GetOnceKeyDown('F'))
		{
			addUnitQueue.push_back({ 2,0,12.6 });
		}
	}


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
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0000"), { UIPosition[0].x - 1 ,UIPosition[0].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
		if (player->buildList[Player::BuildList::eAcademy] == true)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0010"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);
		}
		else
		{
			IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0010"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);
		}
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0001"), { UIPosition[2].x - 1 ,UIPosition[2].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0282"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
	}

	if (addUnitQueue.size() != 0)
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0003"), { 445,700 }, 1.5, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0007"), { 421,673 }, 1.5, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0011"), { 465,755 }, 1.5, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0004"), { 445,762 }, 1.5, 0, 0);
		if (addUnitQueue.size() > 1)
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0007"), { 421,735 }, 1.5, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0004"), { 505,762 }, 1.5, 0, 0);
		if (addUnitQueue.size() > 2)
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0007"), { 481,735 }, 1.5, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0004"), { 565,762 }, 1.5, 0, 0);
		if (addUnitQueue.size() > 3)
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0007"), { 541,735 }, 1.5, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0003"), { 625,762 }, 1.5, 0, 0);

		if (addUnitQueue.size() > 4)
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0007"), { 601,735 }, 1.5, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("coolTimeBar"), { 535,694 }, 0.8, 0, 0);

		for (int i = 0; i < 41; i++)
		{
			if (addUnitQueue[0].maxTime / 41 * i < addUnitQueue[0].timeNow)
			{
				IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("Coll"), { float(535 + i * 4),694 }, 0.8, 0, 0);
			}
		}

		IMAGEMANAGER->DirectDrawText(L"Building", { 500,660 }, { 15,15 }, { 255,255,255,1 });

		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			addUnitQueue.pop_back();
		}
	}
	else
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0286"), { UIPosition[5].x - 1 ,UIPosition[5].y - 2 }, 1.7, 0, 0);

	}
	IMAGEMANAGER->DirectDrawText(L"Terran Barracks", { 400,625 }, { 15,15 });
}
