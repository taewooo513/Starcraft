#include "stdafx.h"
#include "Marine.h"
#include "Player.h"

void Marine::Init()
{
	range = 300;
	runImageTimeDelay = 0;
	astarTimer = 0.1;
	grid = GRIDMANAGER->AddGrid(this, 2, 2, 20, 20, 0, 0);
	grid->gridTag = rand() % 10000 + 100;
	player->AddUnit(this);
	deathImageTimeDelay = 0;
	player->m_suff += 1;

	idleImage[8] = IMAGEMANAGER->FindImage("marin_idle_1_1");
	idleImage[7] = IMAGEMANAGER->FindImage("marin_idle_1_2");
	idleImage[6] = IMAGEMANAGER->FindImage("marin_idle_1_3");
	idleImage[5] = IMAGEMANAGER->FindImage("marin_idle_1_4");
	idleImage[4] = IMAGEMANAGER->FindImage("marin_idle_1_5");
	idleImage[3] = IMAGEMANAGER->FindImage("marin_idle_1_6");
	idleImage[2] = IMAGEMANAGER->FindImage("marin_idle_1_7");
	idleImage[1] = IMAGEMANAGER->FindImage("marin_idle_1_8");
	idleImage[0] = IMAGEMANAGER->FindImage("marin_idle_1_9");

	run1Image[0] = IMAGEMANAGER->FindImage("marin_run_1_1");
	run1Image[1] = IMAGEMANAGER->FindImage("marin_run_1_2");
	run1Image[2] = IMAGEMANAGER->FindImage("marin_run_1_3");
	run1Image[3] = IMAGEMANAGER->FindImage("marin_run_1_4");
	run1Image[4] = IMAGEMANAGER->FindImage("marin_run_1_5");
	run1Image[5] = IMAGEMANAGER->FindImage("marin_run_1_6");
	run1Image[6] = IMAGEMANAGER->FindImage("marin_run_1_7");
	run1Image[7] = IMAGEMANAGER->FindImage("marin_run_1_8");
	run1Image[8] = IMAGEMANAGER->FindImage("marin_run_1_9");

	run2Image[0] = IMAGEMANAGER->FindImage("marin_run_2_1");
	run2Image[1] = IMAGEMANAGER->FindImage("marin_run_2_2");
	run2Image[2] = IMAGEMANAGER->FindImage("marin_run_2_3");
	run2Image[3] = IMAGEMANAGER->FindImage("marin_run_2_4");
	run2Image[4] = IMAGEMANAGER->FindImage("marin_run_2_5");
	run2Image[5] = IMAGEMANAGER->FindImage("marin_run_2_6");
	run2Image[6] = IMAGEMANAGER->FindImage("marin_run_2_7");
	run2Image[7] = IMAGEMANAGER->FindImage("marin_run_2_8");
	run2Image[8] = IMAGEMANAGER->FindImage("marin_run_2_9");

	run3Image[0] = IMAGEMANAGER->FindImage("marin_run_3_1");
	run3Image[1] = IMAGEMANAGER->FindImage("marin_run_3_2");
	run3Image[2] = IMAGEMANAGER->FindImage("marin_run_3_3");
	run3Image[3] = IMAGEMANAGER->FindImage("marin_run_3_4");
	run3Image[4] = IMAGEMANAGER->FindImage("marin_run_3_5");
	run3Image[5] = IMAGEMANAGER->FindImage("marin_run_3_6");
	run3Image[6] = IMAGEMANAGER->FindImage("marin_run_3_7");
	run3Image[7] = IMAGEMANAGER->FindImage("marin_run_3_8");
	run3Image[8] = IMAGEMANAGER->FindImage("marin_run_3_9");

	run4Image[0] = IMAGEMANAGER->FindImage("marin_run_4_1");
	run4Image[1] = IMAGEMANAGER->FindImage("marin_run_4_2");
	run4Image[2] = IMAGEMANAGER->FindImage("marin_run_4_3");
	run4Image[3] = IMAGEMANAGER->FindImage("marin_run_4_4");
	run4Image[4] = IMAGEMANAGER->FindImage("marin_run_4_5");
	run4Image[5] = IMAGEMANAGER->FindImage("marin_run_4_6");
	run4Image[6] = IMAGEMANAGER->FindImage("marin_run_4_7");
	run4Image[7] = IMAGEMANAGER->FindImage("marin_run_4_8");
	run4Image[8] = IMAGEMANAGER->FindImage("marin_run_4_9");

	run5Image[0] = IMAGEMANAGER->FindImage("marin_run_5_1");
	run5Image[1] = IMAGEMANAGER->FindImage("marin_run_5_2");
	run5Image[2] = IMAGEMANAGER->FindImage("marin_run_5_3");
	run5Image[3] = IMAGEMANAGER->FindImage("marin_run_5_4");
	run5Image[4] = IMAGEMANAGER->FindImage("marin_run_5_5");
	run5Image[5] = IMAGEMANAGER->FindImage("marin_run_5_6");
	run5Image[6] = IMAGEMANAGER->FindImage("marin_run_5_7");
	run5Image[7] = IMAGEMANAGER->FindImage("marin_run_5_8");
	run5Image[8] = IMAGEMANAGER->FindImage("marin_run_5_9");

	run6Image[0] = IMAGEMANAGER->FindImage("marin_run_6_1");
	run6Image[1] = IMAGEMANAGER->FindImage("marin_run_6_2");
	run6Image[2] = IMAGEMANAGER->FindImage("marin_run_6_3");
	run6Image[3] = IMAGEMANAGER->FindImage("marin_run_6_4");
	run6Image[4] = IMAGEMANAGER->FindImage("marin_run_6_5");
	run6Image[5] = IMAGEMANAGER->FindImage("marin_run_6_6");
	run6Image[6] = IMAGEMANAGER->FindImage("marin_run_6_7");
	run6Image[7] = IMAGEMANAGER->FindImage("marin_run_6_8");
	run6Image[8] = IMAGEMANAGER->FindImage("marin_run_6_9");

	run7Image[0] = IMAGEMANAGER->FindImage("marin_run_7_1");
	run7Image[1] = IMAGEMANAGER->FindImage("marin_run_7_2");
	run7Image[2] = IMAGEMANAGER->FindImage("marin_run_7_3");
	run7Image[3] = IMAGEMANAGER->FindImage("marin_run_7_4");
	run7Image[4] = IMAGEMANAGER->FindImage("marin_run_7_5");
	run7Image[5] = IMAGEMANAGER->FindImage("marin_run_7_6");
	run7Image[6] = IMAGEMANAGER->FindImage("marin_run_7_7");
	run7Image[7] = IMAGEMANAGER->FindImage("marin_run_7_8");
	run7Image[8] = IMAGEMANAGER->FindImage("marin_run_7_9");

	run8Image[0] = IMAGEMANAGER->FindImage("marin_run_8_1");
	run8Image[1] = IMAGEMANAGER->FindImage("marin_run_8_2");
	run8Image[2] = IMAGEMANAGER->FindImage("marin_run_8_3");
	run8Image[3] = IMAGEMANAGER->FindImage("marin_run_8_4");
	run8Image[4] = IMAGEMANAGER->FindImage("marin_run_8_5");
	run8Image[5] = IMAGEMANAGER->FindImage("marin_run_8_6");
	run8Image[6] = IMAGEMANAGER->FindImage("marin_run_8_7");
	run8Image[7] = IMAGEMANAGER->FindImage("marin_run_8_8");
	run8Image[8] = IMAGEMANAGER->FindImage("marin_run_8_9");

	deathImage[0] = IMAGEMANAGER->FindImage("marin_death_1");
	deathImage[1] = IMAGEMANAGER->FindImage("marin_death_2");
	deathImage[2] = IMAGEMANAGER->FindImage("marin_death_3");
	deathImage[3] = IMAGEMANAGER->FindImage("marin_death_4");
	deathImage[4] = IMAGEMANAGER->FindImage("marin_death_5");
	deathImage[5] = IMAGEMANAGER->FindImage("marin_death_6");
	deathImage[6] = IMAGEMANAGER->FindImage("marin_death_7");
	deathImage[7] = IMAGEMANAGER->FindImage("marin_death_8");

	fire1Image[8] = IMAGEMANAGER->FindImage("marin_attack_1_1");
	fire1Image[7] = IMAGEMANAGER->FindImage("marin_attack_1_2");
	fire1Image[6] = IMAGEMANAGER->FindImage("marin_attack_1_3");
	fire1Image[5] = IMAGEMANAGER->FindImage("marin_attack_1_4");
	fire1Image[4] = IMAGEMANAGER->FindImage("marin_attack_1_5");
	fire1Image[3] = IMAGEMANAGER->FindImage("marin_attack_1_6");
	fire1Image[2] = IMAGEMANAGER->FindImage("marin_attack_1_7");
	fire1Image[1] = IMAGEMANAGER->FindImage("marin_attack_1_8");
	fire1Image[0] = IMAGEMANAGER->FindImage("marin_attack_1_9");

	fire2Image[8] = IMAGEMANAGER->FindImage("marin_attack_2_1");
	fire2Image[7] = IMAGEMANAGER->FindImage("marin_attack_2_2");
	fire2Image[6] = IMAGEMANAGER->FindImage("marin_attack_2_3");
	fire2Image[5] = IMAGEMANAGER->FindImage("marin_attack_2_4");
	fire2Image[4] = IMAGEMANAGER->FindImage("marin_attack_2_5");
	fire2Image[3] = IMAGEMANAGER->FindImage("marin_attack_2_6");
	fire2Image[2] = IMAGEMANAGER->FindImage("marin_attack_2_7");
	fire2Image[1] = IMAGEMANAGER->FindImage("marin_attack_2_8");
	fire2Image[0] = IMAGEMANAGER->FindImage("marin_attack_2_9");

	//

	sidleImage[8] = IMAGEMANAGER->FindImage("marin_idle_1_1_s");
	sidleImage[7] = IMAGEMANAGER->FindImage("marin_idle_1_2_s");
	sidleImage[6] = IMAGEMANAGER->FindImage("marin_idle_1_3_s");
	sidleImage[5] = IMAGEMANAGER->FindImage("marin_idle_1_4_s");
	sidleImage[4] = IMAGEMANAGER->FindImage("marin_idle_1_5_s");
	sidleImage[3] = IMAGEMANAGER->FindImage("marin_idle_1_6_s");
	sidleImage[2] = IMAGEMANAGER->FindImage("marin_idle_1_7_s");
	sidleImage[1] = IMAGEMANAGER->FindImage("marin_idle_1_8_s");
	sidleImage[0] = IMAGEMANAGER->FindImage("marin_idle_1_9_s");
	
	srun1Image[0] = IMAGEMANAGER->FindImage("marin_run_1_1_s");
	srun1Image[1] = IMAGEMANAGER->FindImage("marin_run_1_2_s");
	srun1Image[2] = IMAGEMANAGER->FindImage("marin_run_1_3_s");
	srun1Image[3] = IMAGEMANAGER->FindImage("marin_run_1_4_s");
	srun1Image[4] = IMAGEMANAGER->FindImage("marin_run_1_5_s");
	srun1Image[5] = IMAGEMANAGER->FindImage("marin_run_1_6_s");
	srun1Image[6] = IMAGEMANAGER->FindImage("marin_run_1_7_s");
	srun1Image[7] = IMAGEMANAGER->FindImage("marin_run_1_8_s");
	srun1Image[8] = IMAGEMANAGER->FindImage("marin_run_1_9_s");
	
	srun2Image[0] = IMAGEMANAGER->FindImage("marin_run_2_1_s");
	srun2Image[1] = IMAGEMANAGER->FindImage("marin_run_2_2_s");
	srun2Image[2] = IMAGEMANAGER->FindImage("marin_run_2_3_s");
	srun2Image[3] = IMAGEMANAGER->FindImage("marin_run_2_4_s");
	srun2Image[4] = IMAGEMANAGER->FindImage("marin_run_2_5_s");
	srun2Image[5] = IMAGEMANAGER->FindImage("marin_run_2_6_s");
	srun2Image[6] = IMAGEMANAGER->FindImage("marin_run_2_7_s");
	srun2Image[7] = IMAGEMANAGER->FindImage("marin_run_2_8_s");
	srun2Image[8] = IMAGEMANAGER->FindImage("marin_run_2_9_s");
	
	srun3Image[0] = IMAGEMANAGER->FindImage("marin_run_3_1_s");
	srun3Image[1] = IMAGEMANAGER->FindImage("marin_run_3_2_s");
	srun3Image[2] = IMAGEMANAGER->FindImage("marin_run_3_3_s");
	srun3Image[3] = IMAGEMANAGER->FindImage("marin_run_3_4_s");
	srun3Image[4] = IMAGEMANAGER->FindImage("marin_run_3_5_s");
	srun3Image[5] = IMAGEMANAGER->FindImage("marin_run_3_6_s");
	srun3Image[6] = IMAGEMANAGER->FindImage("marin_run_3_7_s");
	srun3Image[7] = IMAGEMANAGER->FindImage("marin_run_3_8_s");
	srun3Image[8] = IMAGEMANAGER->FindImage("marin_run_3_9_s");
	
	srun4Image[0] = IMAGEMANAGER->FindImage("marin_run_4_1_s");
	srun4Image[1] = IMAGEMANAGER->FindImage("marin_run_4_2_s");
	srun4Image[2] = IMAGEMANAGER->FindImage("marin_run_4_3_s");
	srun4Image[3] = IMAGEMANAGER->FindImage("marin_run_4_4_s");
	srun4Image[4] = IMAGEMANAGER->FindImage("marin_run_4_5_s");
	srun4Image[5] = IMAGEMANAGER->FindImage("marin_run_4_6_s");
	srun4Image[6] = IMAGEMANAGER->FindImage("marin_run_4_7_s");
	srun4Image[7] = IMAGEMANAGER->FindImage("marin_run_4_8_s");
	srun4Image[8] = IMAGEMANAGER->FindImage("marin_run_4_9_s");
	
	srun5Image[0] = IMAGEMANAGER->FindImage("marin_run_5_1_s");
	srun5Image[1] = IMAGEMANAGER->FindImage("marin_run_5_2_s");
	srun5Image[2] = IMAGEMANAGER->FindImage("marin_run_5_3_s");
	srun5Image[3] = IMAGEMANAGER->FindImage("marin_run_5_4_s");
	srun5Image[4] = IMAGEMANAGER->FindImage("marin_run_5_5_s");
	srun5Image[5] = IMAGEMANAGER->FindImage("marin_run_5_6_s");
	srun5Image[6] = IMAGEMANAGER->FindImage("marin_run_5_7_s");
	srun5Image[7] = IMAGEMANAGER->FindImage("marin_run_5_8_s");
	srun5Image[8] = IMAGEMANAGER->FindImage("marin_run_5_9_s");
	
	srun6Image[0] = IMAGEMANAGER->FindImage("marin_run_6_1_s");
	srun6Image[1] = IMAGEMANAGER->FindImage("marin_run_6_2_s");
	srun6Image[2] = IMAGEMANAGER->FindImage("marin_run_6_3_s");
	srun6Image[3] = IMAGEMANAGER->FindImage("marin_run_6_4_s");
	srun6Image[4] = IMAGEMANAGER->FindImage("marin_run_6_5_s");
	srun6Image[5] = IMAGEMANAGER->FindImage("marin_run_6_6_s");
	srun6Image[6] = IMAGEMANAGER->FindImage("marin_run_6_7_s");
	srun6Image[7] = IMAGEMANAGER->FindImage("marin_run_6_8_s");
	srun6Image[8] = IMAGEMANAGER->FindImage("marin_run_6_9_s");
	
	srun7Image[0] = IMAGEMANAGER->FindImage("marin_run_7_1_s");
	srun7Image[1] = IMAGEMANAGER->FindImage("marin_run_7_2_s");
	srun7Image[2] = IMAGEMANAGER->FindImage("marin_run_7_3_s");
	srun7Image[3] = IMAGEMANAGER->FindImage("marin_run_7_4_s");
	srun7Image[4] = IMAGEMANAGER->FindImage("marin_run_7_5_s");
	srun7Image[5] = IMAGEMANAGER->FindImage("marin_run_7_6_s");
	srun7Image[6] = IMAGEMANAGER->FindImage("marin_run_7_7_s");
	srun7Image[7] = IMAGEMANAGER->FindImage("marin_run_7_8_s");
	srun7Image[8] = IMAGEMANAGER->FindImage("marin_run_7_9_s");
	
	srun8Image[0] = IMAGEMANAGER->FindImage("marin_run_8_1_s");
	srun8Image[1] = IMAGEMANAGER->FindImage("marin_run_8_2_s");
	srun8Image[2] = IMAGEMANAGER->FindImage("marin_run_8_3_s");
	srun8Image[3] = IMAGEMANAGER->FindImage("marin_run_8_4_s");
	srun8Image[4] = IMAGEMANAGER->FindImage("marin_run_8_5_s");
	srun8Image[5] = IMAGEMANAGER->FindImage("marin_run_8_6_s");
	srun8Image[6] = IMAGEMANAGER->FindImage("marin_run_8_7_s");
	srun8Image[7] = IMAGEMANAGER->FindImage("marin_run_8_8_s");
	srun8Image[8] = IMAGEMANAGER->FindImage("marin_run_8_9_s");
	
	sfire1Image[8] = IMAGEMANAGER->FindImage("marin_attack_1_1_s");
	sfire1Image[7] = IMAGEMANAGER->FindImage("marin_attack_1_2_s");
	sfire1Image[6] = IMAGEMANAGER->FindImage("marin_attack_1_3_s");
	sfire1Image[5] = IMAGEMANAGER->FindImage("marin_attack_1_4_s");
	sfire1Image[4] = IMAGEMANAGER->FindImage("marin_attack_1_5_s");
	sfire1Image[3] = IMAGEMANAGER->FindImage("marin_attack_1_6_s");
	sfire1Image[2] = IMAGEMANAGER->FindImage("marin_attack_1_7_s");
	sfire1Image[1] = IMAGEMANAGER->FindImage("marin_attack_1_8_s");
	sfire1Image[0] = IMAGEMANAGER->FindImage("marin_attack_1_9_s");
	
	sfire2Image[8] = IMAGEMANAGER->FindImage("marin_attack_2_1_s");
	sfire2Image[7] = IMAGEMANAGER->FindImage("marin_attack_2_2_s");
	sfire2Image[6] = IMAGEMANAGER->FindImage("marin_attack_2_3_s");
	sfire2Image[5] = IMAGEMANAGER->FindImage("marin_attack_2_4_s");
	sfire2Image[4] = IMAGEMANAGER->FindImage("marin_attack_2_5_s");
	sfire2Image[3] = IMAGEMANAGER->FindImage("marin_attack_2_6_s");
	sfire2Image[2] = IMAGEMANAGER->FindImage("marin_attack_2_7_s");
	sfire2Image[1] = IMAGEMANAGER->FindImage("marin_attack_2_8_s");
	sfire2Image[0] = IMAGEMANAGER->FindImage("marin_attack_2_9_s");

	m_attack = 5;
	m_isClick = false;
	m_maxHp = 40;
	m_hp = m_maxHp;
	lasthp = m_hp;
}

void Marine::Move()
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

					astarTimer = 0.25f;
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

void Marine::Attack()
{
	float dest = sqrt((attackObject->position.x - position.x) * (attackObject->position.x - position.x) + (attackObject->position.y - position.y) * (attackObject->position.y - position.y));
	if (range > dest)
	{
		fireImageDel2 += DELTA_TIME;
		if (fireImageDel2 < 0.5f && onceFire == true)
		{
			attackObject->m_hp -= m_attack;
			onceFire = false;
			SOUNDMANAGER->play("tmafir00", 0.5f);
		}
		m_dest = { 0,0 };
	}
	else
	{
		m_dest = attackObject->position;
		player->Astar(position, m_dest, this);
	}
}

void Marine::Update()
{
	if (isdeath == true)
	{
		ObjectDestroyed();
	}
	if (attackObject == nullptr)
	{
		for (auto iter : player->otherPlayer->m_units)
		{
			float dest = sqrt(
				(iter->position.x - position.x) * (iter->position.x - position.x) +
				(iter->position.y - position.y) * (iter->position.y - position.y));
			if (range > dest)
			{
				attackObject = iter;
				break;
			}
		}
		if (attackObject == nullptr)
		{
			for (auto iter : player->otherPlayer->m_builds)
			{
				float dest = sqrt(
					(iter->position.x - position.x) * (iter->position.x - position.x) +
					(iter->position.y - position.y) * (iter->position.y - position.y));
				if (range > dest)
				{
					attackObject = iter;
					break;
				}
			}
		}
	}
	else
	{
		if (attackObject->isdeath == true)
		{
			attackObject = nullptr;
		}
	}
	if (attackObject != nullptr)
	{
		Attack();
	}


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

void Marine::Render()
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
			if (attackObject == nullptr)
			{
				IMAGEMANAGER->CenterRenderBlendBlack(sidleImage[(int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(idleImage[(int)rr], position, 1.5f, 0, isR);
				nowFire = false;
				onceFire = true;
			}
			else
			{
				if (fireImageDel2 > 0.5)
				{
					fireImageDel += DELTA_TIME;
					if (fireImageDel > 0.05f)
					{
						nowFire = !nowFire;
						fireImageDel = 0;
						fireCount++;
					}
					if (fireCount == 4)
					{
						fireImageDel2 = 0;
						fireCount = 0;
						onceFire = true;
					}
				}

				rot = atan2(attackObject->position.x - position.x, attackObject->position.y - position.y);
				rr = 8.f / 3.141592 * abs(rot);

				if (nowFire == false)
				{
					IMAGEMANAGER->CenterRenderBlendBlack(sfire1Image[(int)rr], position, 1.5f, 0, isR);
					IMAGEMANAGER->CenterRenderBlendBlack(fire1Image[(int)rr], position, 1.5f, 0, isR);
				}
				else
				{
					IMAGEMANAGER->CenterRenderBlendBlack(sfire2Image[(int)rr], position, 1.5f, 0, isR);
					IMAGEMANAGER->CenterRenderBlendBlack(fire2Image[(int)rr], position, 1.5f, 0, isR);
				}
			}
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
				IMAGEMANAGER->CenterRenderBlendBlack(srun1Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run1Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 1:
				IMAGEMANAGER->CenterRenderBlendBlack(srun2Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run2Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 2:
				IMAGEMANAGER->CenterRenderBlendBlack(srun3Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run3Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 3:
				IMAGEMANAGER->CenterRenderBlendBlack(srun4Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run4Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 4:
				IMAGEMANAGER->CenterRenderBlendBlack(srun5Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run5Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 5:
				IMAGEMANAGER->CenterRenderBlendBlack(srun6Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run6Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 6:
				IMAGEMANAGER->CenterRenderBlendBlack(srun7Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run7Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			case 7:
				IMAGEMANAGER->CenterRenderBlendBlack(srun8Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run8Image[8 - (int)rr], position, 1.5f, 0, isR);
				break;
			default:
				IMAGEMANAGER->CenterRenderBlendBlack(srun8Image[8 - (int)rr], position, 1.5f, 0, isR);
				IMAGEMANAGER->CenterRenderBlendBlack(run8Image[8 - (int)rr], position, 1.5f, 0, isR);
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
			if (deathFrame == 0)
			{
				if (rand() % 2 == 0)
					SOUNDMANAGER->play("tmadth01", 0.5f);
				else
					SOUNDMANAGER->play("tmadth00", 0.5f);
			}
			deathImageTimeDelay = 0;
			deathFrame++;
		}
		if (deathFrame < 8)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(deathImage[deathFrame], position, 1.5f, 0, isR);
		}
		else
		{
			if (isdeath == false)
			{
				isdeath = true;
			}

		}
	}
	IMAGEMANAGER->FogUpdate(position, 30);
	m_isClick = false;

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

void Marine::UIRender()
{
	m_isClick = true;

	IMAGEMANAGER->DirectDrawText(L"Private", { 460,655 }, { 15,15 }, { 255,255,255,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran Marine", { 430,625 }, { 15,15 });

	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0292"), { 453,722 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0012"), { 480,750 }, 1.7, 0, 0);

	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0288"), { 523 ,722 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0012"), { 550,750 }, 1.7, 0, 0);


	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0228"), { UIPosition[0].x  ,UIPosition[0].y }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0229"), { UIPosition[1].x - 2 ,UIPosition[1].y }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0230"), { UIPosition[2].x ,UIPosition[2].y }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0254"), { UIPosition[3].x ,UIPosition[3].y + 3 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[4].x + 25,UIPosition[4].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0255"), { UIPosition[4].x ,UIPosition[4].y + 3 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0237"), { UIPosition[6].x ,UIPosition[6].y + 3 }, 1.7, 0, 0);

	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0000"]->wireImages[3][damageIndex[0]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0000"]->wireImages[1][damageIndex[1]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0000"]->wireImages[2][damageIndex[2]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0000"]->wireImages[0][damageIndex[3]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);

	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 295,730 }, { 12,12 }, { 0,255,0,1 });

	if (KEYMANAGER->GetOnceKeyDown(VK_F5))
	{
		m_hp -= 1;
	}

	m_attack = 5;
}

void Marine::Release()
{
	for (auto iter = player->m_selectUnits.begin(); iter < player->m_selectUnits.end(); iter++)
	{
		if ((*iter) == this)
		{
			player->m_selectUnits.erase(iter);
			break;
		}
	}
	player->m_suff -= 1;
}

void Marine::CollisionUpdate()
{
	clickRect = { int(position.x - 20) , int(position.y - 20) , int(position.x + 20) , int(position.y + 20) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}
