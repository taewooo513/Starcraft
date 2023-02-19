#include "stdafx.h"
#include "FireBat.h"
#include "Player.h"
void FireBat::Init()
{
	runImageTimeDelay = 0;
	astarTimer = 0.1;
	player->m_suff += 1;

	grid = GRIDMANAGER->AddGrid(this, 2, 2, 20, 20, 0, 0);
	grid->gridTag = rand() % 10000 + 100;
	player->AddUnit(this);
	deathImageTimeDelay = 0;
	idleImage[8] = IMAGEMANAGER->FindImage("firebat_idle_1");
	idleImage[7] = IMAGEMANAGER->FindImage("firebat_idle_2");
	idleImage[6] = IMAGEMANAGER->FindImage("firebat_idle_3");
	idleImage[5] = IMAGEMANAGER->FindImage("firebat_idle_4");
	idleImage[4] = IMAGEMANAGER->FindImage("firebat_idle_5");
	idleImage[3] = IMAGEMANAGER->FindImage("firebat_idle_6");
	idleImage[2] = IMAGEMANAGER->FindImage("firebat_idle_7");
	idleImage[1] = IMAGEMANAGER->FindImage("firebat_idle_8");
	idleImage[0] = IMAGEMANAGER->FindImage("firebat_idle_9");

	run1Image[0] = IMAGEMANAGER->FindImage("firebat_run_1_1");
	run1Image[1] = IMAGEMANAGER->FindImage("firebat_run_1_2");
	run1Image[2] = IMAGEMANAGER->FindImage("firebat_run_1_3");
	run1Image[3] = IMAGEMANAGER->FindImage("firebat_run_1_4");
	run1Image[4] = IMAGEMANAGER->FindImage("firebat_run_1_5");
	run1Image[5] = IMAGEMANAGER->FindImage("firebat_run_1_6");
	run1Image[6] = IMAGEMANAGER->FindImage("firebat_run_1_7");
	run1Image[7] = IMAGEMANAGER->FindImage("firebat_run_1_8");
	run1Image[8] = IMAGEMANAGER->FindImage("firebat_run_1_9");

	run2Image[0] = IMAGEMANAGER->FindImage("firebat_run_2_1");
	run2Image[1] = IMAGEMANAGER->FindImage("firebat_run_2_2");
	run2Image[2] = IMAGEMANAGER->FindImage("firebat_run_2_3");
	run2Image[3] = IMAGEMANAGER->FindImage("firebat_run_2_4");
	run2Image[4] = IMAGEMANAGER->FindImage("firebat_run_2_5");
	run2Image[5] = IMAGEMANAGER->FindImage("firebat_run_2_6");
	run2Image[6] = IMAGEMANAGER->FindImage("firebat_run_2_7");
	run2Image[7] = IMAGEMANAGER->FindImage("firebat_run_2_8");
	run2Image[8] = IMAGEMANAGER->FindImage("firebat_run_2_9");

	run3Image[0] = IMAGEMANAGER->FindImage("firebat_run_3_1");
	run3Image[1] = IMAGEMANAGER->FindImage("firebat_run_3_2");
	run3Image[2] = IMAGEMANAGER->FindImage("firebat_run_3_3");
	run3Image[3] = IMAGEMANAGER->FindImage("firebat_run_3_4");
	run3Image[4] = IMAGEMANAGER->FindImage("firebat_run_3_5");
	run3Image[5] = IMAGEMANAGER->FindImage("firebat_run_3_6");
	run3Image[6] = IMAGEMANAGER->FindImage("firebat_run_3_7");
	run3Image[7] = IMAGEMANAGER->FindImage("firebat_run_3_8");
	run3Image[8] = IMAGEMANAGER->FindImage("firebat_run_3_9");

	run4Image[0] = IMAGEMANAGER->FindImage("firebat_run_4_1");
	run4Image[1] = IMAGEMANAGER->FindImage("firebat_run_4_2");
	run4Image[2] = IMAGEMANAGER->FindImage("firebat_run_4_3");
	run4Image[3] = IMAGEMANAGER->FindImage("firebat_run_4_4");
	run4Image[4] = IMAGEMANAGER->FindImage("firebat_run_4_5");
	run4Image[5] = IMAGEMANAGER->FindImage("firebat_run_4_6");
	run4Image[6] = IMAGEMANAGER->FindImage("firebat_run_4_7");
	run4Image[7] = IMAGEMANAGER->FindImage("firebat_run_4_8");
	run4Image[8] = IMAGEMANAGER->FindImage("firebat_run_4_9");

	run5Image[0] = IMAGEMANAGER->FindImage("firebat_run_5_1");
	run5Image[1] = IMAGEMANAGER->FindImage("firebat_run_5_2");
	run5Image[2] = IMAGEMANAGER->FindImage("firebat_run_5_3");
	run5Image[3] = IMAGEMANAGER->FindImage("firebat_run_5_4");
	run5Image[4] = IMAGEMANAGER->FindImage("firebat_run_5_5");
	run5Image[5] = IMAGEMANAGER->FindImage("firebat_run_5_6");
	run5Image[6] = IMAGEMANAGER->FindImage("firebat_run_5_7");
	run5Image[7] = IMAGEMANAGER->FindImage("firebat_run_5_8");
	run5Image[8] = IMAGEMANAGER->FindImage("firebat_run_5_9");

	run6Image[0] = IMAGEMANAGER->FindImage("firebat_run_6_1");
	run6Image[1] = IMAGEMANAGER->FindImage("firebat_run_6_2");
	run6Image[2] = IMAGEMANAGER->FindImage("firebat_run_6_3");
	run6Image[3] = IMAGEMANAGER->FindImage("firebat_run_6_4");
	run6Image[4] = IMAGEMANAGER->FindImage("firebat_run_6_5");
	run6Image[5] = IMAGEMANAGER->FindImage("firebat_run_6_6");
	run6Image[6] = IMAGEMANAGER->FindImage("firebat_run_6_7");
	run6Image[7] = IMAGEMANAGER->FindImage("firebat_run_6_8");
	run6Image[8] = IMAGEMANAGER->FindImage("firebat_run_6_9");

	run7Image[0] = IMAGEMANAGER->FindImage("firebat_run_7_1");
	run7Image[1] = IMAGEMANAGER->FindImage("firebat_run_7_2");
	run7Image[2] = IMAGEMANAGER->FindImage("firebat_run_7_3");
	run7Image[3] = IMAGEMANAGER->FindImage("firebat_run_7_4");
	run7Image[4] = IMAGEMANAGER->FindImage("firebat_run_7_5");
	run7Image[5] = IMAGEMANAGER->FindImage("firebat_run_7_6");
	run7Image[6] = IMAGEMANAGER->FindImage("firebat_run_7_7");
	run7Image[7] = IMAGEMANAGER->FindImage("firebat_run_7_8");
	run7Image[8] = IMAGEMANAGER->FindImage("firebat_run_7_9");



	m_isClick = false;
	m_maxHp = 40;
	m_hp = m_maxHp;
}

void FireBat::Move()
{
	Vector2 nowRegionPos;
	nowRegionPos = position / 1.5 / 8;
	int regionId = GRIDMANAGER->regionsTile[(int)nowRegionPos.x][(int)nowRegionPos.y].regionsIds;

	if (regionId != -1)
	{
		if (moveNodeStack.empty() == false)
		{
			if (moveNodeStack.top()->regionId == regionId)
			{
				moveNodeStack.pop();
				grid->Astar(2, 2);

			}
			if (moveNodeStack.empty() == false)
			{
				if (grid->moveStack2.empty() == true)
					grid->Astar(2, 2);
			}
		}
		else
		{
			if (grid->moveStack2.empty() == true)
				grid->Astar(2, 2);
			//여기선 다음 레기온과의 최단거리 
		}
	}

	if (grid->moveStack2.empty() == false)
	{
		d = Vector2{ (float)(grid->moveStack2.top().x * 8 * 1.5),(float)(grid->moveStack2.top().y * 8 * 1.5) };
		if (d.x != 0 && d.y != 0)
		{
			if (!grid->moveStack2.empty())
			{
				rot = atan2(d.x - position.x, d.y - position.y);
			}

			if (m_speed < 300)
			{
				m_speed += 5;
			}
			float moveDestX = sin(rot) * DELTA_TIME * m_speed;
			float moveDestY = cos(rot) * DELTA_TIME * m_speed;
			float length = sqrt((d.x - position.x) * (d.x - position.x) + (d.y - position.y) * (d.y - position.y));

			if (length < DELTA_TIME * m_speed)
			{
				position.x = d.x;
				position.y = d.y;
				if (astarTimer < 0)
				{
					grid->Astar(2, 2);

					astarTimer = 0.05f;
				}
				astarTimer -= DELTA_TIME;
				if (grid->moveStack2.empty() == false)
				{
					grid->moveStack2.pop();

					if (grid->moveStack2.empty() && moveNodeStack.empty())
					{
						d = { 0,0 };
						m_dest = { 0,0 };
						m_speed = 0;
					}
				}
			}
			else
			{
				position.x += moveDestX;
				position.y += moveDestY;
			}
		}
		else
		{
			runFrame = 0;
			m_speed = 0;
		}
	}
	grid->Update();

}

void FireBat::Attack()
{
}

void FireBat::Update()
{
	IMAGEMANAGER->FogUpdate(position, 30);

	if (KEYMANAGER->GetOnceKeyDown(VK_F2))
	{
		m_hp = 0;
	}

	if (m_hp <= 0)
	{
		isDeath = true;
		deathImageTimeDelay += DELTA_TIME;
		if (player != nullptr)
		{
			if (player->m_selectUnit == this)
			{
				player->m_selectUnit = nullptr;
			}
			for (auto iter = player->m_selectUnits.begin(); iter != player->m_selectUnits.end(); iter++)
			{
				if (*iter == this)
				{
					player->m_selectUnits.erase(iter);
					break;
				}
			}

			for (auto iter = player->m_units.begin(); iter != player->m_units.end(); iter++)
			{
				if (*iter == this)
				{
					player->m_units.erase(iter);
					break;
				}
			}
		}
	}

	if (m_dest.x != 0 && m_dest.y != 0)
	{
		m_speed = 200;
		Move();
	}
	else
	{
		m_speed = 0;
	}
	CollisionUpdate();
}

void FireBat::Render()
{
	float rr = 8.f / 3.141592 * abs(rot);
	bool isR = false;
	if (isDeath == false)
	{
		if (rot < 0)
		{
			isR = true;
		}
		if (m_isClick == true)
		{
			IMAGEMANAGER->DrawCircle({ position.x,position.y + 10 }, 9, 6);
		}

		if (m_speed == 0)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(idleImage[(int)rr], position, 1.5f, 0, isR);
		}
		else
		{
			runImageTimeDelay += DELTA_TIME;
			if (runImageTimeDelay > 0.07f)
			{
				runFrame++;
				runImageTimeDelay = 0;
			}
			switch (runFrame)
			{
			case 0:
				IMAGEMANAGER->CenterRenderBlendBlack(run1Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 1:
				IMAGEMANAGER->CenterRenderBlendBlack(run2Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 2:
				IMAGEMANAGER->CenterRenderBlendBlack(run3Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 3:
				IMAGEMANAGER->CenterRenderBlendBlack(run4Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 4:
				IMAGEMANAGER->CenterRenderBlendBlack(run5Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 5:
				IMAGEMANAGER->CenterRenderBlendBlack(run6Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 6:
				IMAGEMANAGER->CenterRenderBlendBlack(run7Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			default:
				IMAGEMANAGER->CenterRenderBlendBlack(run7Image[8 - (int)rr], position, 1.5f, 0, isR);
				runFrame = 0;
				break;
			}
		}
	}
	else
	{
		deathImageTimeDelay += DELTA_TIME;
		if (deathImageTimeDelay > 0.2f)
		{
			deathImageTimeDelay = 0;
			deathFrame++;
		}
		if (deathFrame < 8)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(deathImage[deathFrame], position, 1.5f, 0, isR);
		}
	}
	m_isClick = false;
}

void FireBat::UIRender()
{
	m_isClick = true;
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("wirefram0010"), { 319,680 }, 1.5, 0, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 300,730 }, { 12,12 }, { 0,255,0,1 });

	IMAGEMANAGER->DirectDrawText(L"Corporal", { 460,655 }, { 15,15 }, { 255,255,255,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran FireBat", { 430,625 }, { 15,15 });

}

void FireBat::Release()
{
}

void FireBat::CollisionUpdate()
{
	clickRect = { int(position.x - 20) , int(position.y - 20) , int(position.x + 20) , int(position.y + 20) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}
