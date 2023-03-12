#include "stdafx.h"
#include "CommandCenter.h"
#include "SpaceConstructionVehicle.h"
CommandCenter::CommandCenter()
{
}

CommandCenter::~CommandCenter()
{
	player->m_maxSuff -= 10;

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
	grid->Release();

}

void CommandCenter::Init()
{
	player->m_maxSuff += 10;
	grid = GRIDMANAGER->AddGrid(this, 14, 11, 3, 2, -7, -5);
	grid->gridTag = 3;
	player->AddBuild(this);
	idleP[0] = IMAGEMANAGER->AddImageVectorCopy("Advisor1P");
	idleP[1] = IMAGEMANAGER->AddImageVectorCopy("Advisor2P");
	idleP[2] = IMAGEMANAGER->AddImageVectorCopy("Advisor3P");
	idleP[3] = IMAGEMANAGER->AddImageVectorCopy("Advisor4P");
	idleP[0]->Setting(0.1f, false);
	idleP[1]->Setting(0.1f, false);
	idleP[2]->Setting(0.1f, false);
	idleP[3]->Setting(0.1f, false);
	m_maxCompleteTime = 75;
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
	lasthp = m_hp;
}

void CommandCenter::Update()
{
	if (KEYMANAGER->GetStayKeyDown(VK_SPACE))
	{
		m_hp -= 10;
	}

	if (m_hp <= 0)
	{
		EFFECTMANAGER->AddEffect("f2", { position.x - 120,position.y - 90 }, 1, 0.1f);

		ObjectDestroyed();
		if (player != nullptr)
		{
			if (player->m_selectBuild == this)
			{
				player->m_selectUnit = nullptr;
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
	if (addUnitQueue.empty() == false)
	{
		if (addUnitQueue.front().timeNow < addUnitQueue.front().maxTime)
		{
			addUnitQueue.front().timeNow += DELTA_TIME;
		}
		else
		{
			if (player->m_suff + 1 <= player->m_maxSuff)
			{
				addUnitQueue.front().timeNow = addUnitQueue.front().maxTime;
				SpaceConstructionVehicle* scv = new SpaceConstructionVehicle;
				scv->SetPlayer(player);
				OBJECTMANAGER->AddObject(scv, "scv", position.x - 20, position.y + 100, 1);
				addUnitQueue.erase(addUnitQueue.begin());
				ac = 0;
			}
			else if (ac == 0)
			{
				ac++;
				SOUNDMANAGER->play("taderr02", 0.5f);
			}
		}
	}

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	if (&player->m_builds == &OBJECTMANAGER->m_player->m_builds)
	{
		IMAGEMANAGER->FogUpdate(position, 30);
	}
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x + 100;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x + 100;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y + 80;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y + 80;
	grid->Update();

	if ((lasthp - m_hp) >= m_maxHp / 16)
	{
		radf = (lasthp - m_hp);
		int fas = radf / (m_maxHp / 16);
		lasthp -= fas * (m_maxHp / 16);
		for (int i = 0; i < fas; )
		{
			int rrrrr = rand() % 4;
			if (damageIndex[rrrrr] < 3)
			{
				damageIndex[rrrrr]++;
				i++;
			}
			if (damageIndex[0] == 3 && damageIndex[1] == 3 && damageIndex[2] == 3 && damageIndex[3] == 3)
			{
				break;
			}
		}
	}
}

void CommandCenter::Render()
{
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x,position.y }, 50, 30);
	}
	if (m_buildIndex < 4)
	{
		if (m_buildIndex < 3)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(buildImage[m_buildIndex], { position.x ,position.y }, 1.5, 0);
		}
		else
		{


			IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tccshad0001"), { position.x  ,position.y }, 1.5, 0);
			IMAGEMANAGER->CenterRenderBlendBlack(buildImage[m_buildIndex], { position.x  ,position.y }, 1.5, 0);
		}
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tccshad0000"), { position.x  ,position.y }, 1.5, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(idle, { position.x  ,position.y }, 1.5, 0);
	}

	if (!addUnitQueue.empty())
	{
		workTimer += DELTA_TIME;

		if (workTimer > 0.08f)
		{
			isWork = !isWork;
			workTimer = 0;
		}

		if (isWork == true)
			IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("controlt"), { position.x  ,position.y }, 1.5, 0);
	}
	m_isClick = false;
}

void CommandCenter::Release()
{
}

void CommandCenter::UIRender()
{
	m_isClick = true;
	if (addUnitQueue.size() < 5)
	{
		if (KEYMANAGER->GetOnceKeyDown('S'))
		{
			if (player->m_mineral - 50 >= 0)
			{
				if (player->m_suff + 1 <= player->m_maxSuff)
				{
					player->m_mineral -= 50;
					addUnitQueue.push_back({ 1,0,12.6 });
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
	}
	if (addUnitQueue.size() == 0)
	{
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0286"), { UIPosition[5].x - 1 ,UIPosition[5].y - 2 }, 1.7, 0, 0);
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
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0007"), { UIPosition[0].x - 1 ,UIPosition[0].y - 4 }, 1.7, 0, 0);
		if (addUnitQueue.size() == 0)
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0282"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
		}
		else
		{
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
			IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
			if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
			{
				player->m_mineral += 50;
				addUnitQueue.pop_back();
			}
		}
	}
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0106"]->wireImages[3][damageIndex[0]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0106"]->wireImages[1][damageIndex[1]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0106"]->wireImages[2][damageIndex[2]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0106"]->wireImages[0][damageIndex[3]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);

	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 295,730 }, { 12,12 }, { 0,255,0,1 });

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

	IMAGEMANAGER->DirectDrawText(L"Terran Command Center", { 400,625 }, { 15,15 });

	idleP[randImgaeP]->UIRenderBlendBlack({ 660,655 }, 1.5, 0, 0);
	if (idleP[randImgaeP]->GetIsEnd())
	{
		if (rand() % 5 != 0)
		{
			randImgaeP = 0;
		}
		else
		{
			randImgaeP = rand() % 3 + 1;
		}
		idleP[randImgaeP]->Reset();
	}
}
