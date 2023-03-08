#include "stdafx.h"
#include "Factory.h"
#include "Vulture.h"
#include "Machines.h"
#include "Tank.h"

Factory::Factory()
{
}

Factory::~Factory()
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

void Factory::Init()
{
	idleAnimation = IMAGEMANAGER->AddImageVectorCopy("FactoryIdle");
	idleAnimation->Setting(0.2, true);
	grid = GRIDMANAGER->AddGrid(this, 14, 11, 3, 2, -7, -5);

	grid->gridTag = 3;

	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("tbldlrg0000");
	m_buildImage[1] = IMAGEMANAGER->FindImage("tbldlrg0001");
	m_buildImage[2] = IMAGEMANAGER->FindImage("tbldlrg0002");
	m_buildImage[3] = IMAGEMANAGER->FindImage("factory0001");

	m_maxCompleteTime = 1;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1000;
	m_hp = 1;
}

void Factory::Update()
{
	grid->Update();
	if (mac != nullptr)
	{
		if (mac->isComplete == false)
		{
			mac->AddBuild(nullptr);
		}
	}
	if (addUnitQueue.empty() == false)
	{
		if (addUnitQueue.front().timeNow < addUnitQueue.front().maxTime)
		{
			addUnitQueue.front().timeNow += DELTA_TIME;
		}
		else
		{
			if (addUnitQueue.front().unit == 1)
			{
				if (player->m_suff + 2 <= player->m_maxSuff)
				{
					Vulture* vulture = new Vulture;
					vulture->SetPlayer(player);
					OBJECTMANAGER->AddObject(vulture, "fireBat", position.x, position.y + 100, 1);
					addUnitQueue.erase(addUnitQueue.begin());
				}
				else
				{
					if (asdf == false)
					{
						SOUNDMANAGER->play("taderr02", 0.5f);
						asdf = true;
					}
				}
			}
			else if (addUnitQueue.front().unit == 2)
			{
				if (player->m_suff + 2 <= player->m_maxSuff)
				{
					Tank* vulture = new Tank;
					vulture->SetPlayer(player);
					OBJECTMANAGER->AddObject(vulture, "fireBat", position.x, position.y + 100, 1);
					addUnitQueue.erase(addUnitQueue.begin());
				}
				else
				{
					if (asdf == false)
					{
						SOUNDMANAGER->play("taderr02", 0.5f);
						asdf = true;
					}
				}
			}
		}
	}
	IMAGEMANAGER->FogUpdate(position, 30);

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x + 85;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y + 75;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y + 75;
}

void Factory::Render()
{
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x,position.y }, 50, 30);
	}
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x  ,position.y }, 1.5, 0);
		else
		{
			IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tfashad0001"), position, 1.5, 0, 0);
			IMAGEMANAGER->CenterRenderBlendBlack(m_buildImage[m_buildIndex], { position.x   ,position.y }, 1.5, 0);
		}
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tfashad0000"), position, 1.5, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("factory0000"), { position.x ,position.y }, 1.5, 0);
	}
	cout << idleAnimation->GetImages()[0]->GetWidth();
	if (!addUnitQueue.empty())
	{
		idleAnimation->CenterRenderBlendBlack({ position.x - (float)(128.f * 1.5 / 2.f),position.y - float(160.f * 1.5 / 2.f) }, 1.5, 0, 0);
	}
	m_isClick = false;
}

void Factory::Release()
{
}

void Factory::UIRender()
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
		if (addUnitQueue.size() < 5)
		{
			if (KEYMANAGER->GetOnceKeyDown('V'))
			{
				if (player->m_mineral - 75 >= 0)
				{
					if (player->m_suff + 2 <= player->m_maxSuff)
					{
						player->m_mineral -= 75;
						addUnitQueue.push_back({ 1,0,19 });
					}
					else
					{
						SOUNDMANAGER->play("taderr02", 0.5f);
					}
				}
				else
				{
					SOUNDMANAGER->play("taderr00", 0.5f);
				}
			}
			if (KEYMANAGER->GetOnceKeyDown('T'))
			{
				if (player->m_mineral - 150 >= 0)
				{
					if (player->m_suff + 2 <= player->m_maxSuff)
					{
						player->m_mineral -= 150;
						addUnitQueue.push_back({ 2,0,31 });
					}
					else
					{
						SOUNDMANAGER->play("taderr02", 0.5f);
					}
				}
				else
				{
					SOUNDMANAGER->play("taderr00", 0.5f);
				}
			}
			if (KEYMANAGER->GetOnceKeyDown('G'))
			{
				//addUnitQueue.push_back({ 2,0,12.6 });
			}
			if (mac == nullptr)
			{
				if (KEYMANAGER->GetOnceKeyDown('C'))
				{
					mac = new Machines;
					mac->fac = this;
					mac->SetPlayer(this->player);
					OBJECTMANAGER->AddObject(mac, "Build", position.x + 140, position.y + 60, 1);
				}
			}
		}

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0002"), { UIPosition[0].x - 1 ,UIPosition[0].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
		if (mac != nullptr)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0023"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);
		}
		else
		{
			IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0023"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);
		}
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);

		if (player->buildList[Player::BuildList::eArmory])
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0003"), { UIPosition[2].x - 1 ,UIPosition[2].y - 2 }, 1.7, 0, 0);
		}
		else
		{
			IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0003"), { UIPosition[2].x - 1 ,UIPosition[2].y - 2 }, 1.7, 0, 0);
		}

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		if (addUnitQueue.size() != 0)
		{
		}
		else
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0282"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
		}

		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0120"), { UIPosition[6].x - 1 ,UIPosition[6].y - 5 }, 1.7, 0, 0);
	}



	if (addUnitQueue.size() != 0)
	{
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
	}
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0113"]->wireImages[3][damageIndex[0]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0113"]->wireImages[1][damageIndex[1]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0113"]->wireImages[2][damageIndex[2]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0113"]->wireImages[0][damageIndex[3]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);

	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 295,730 }, { 12,12 }, { 0,255,0,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran Factory", { 400,625 }, { 15,15 });
}
