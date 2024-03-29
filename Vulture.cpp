#include "stdafx.h"
#include "Vulture.h"
#include "Player.h"
#include "VultureBullet.h"
#include "Player2.h"
Vulture::Vulture()
{
}

Vulture::~Vulture()
{

}

void Vulture::Init()
{
	idleP[0] = IMAGEMANAGER->AddImageVectorCopy("vulture1P");
	idleP[1] = IMAGEMANAGER->AddImageVectorCopy("vulture2P");
	idleP[2] = IMAGEMANAGER->AddImageVectorCopy("vulture3P");
	idleP[3] = IMAGEMANAGER->AddImageVectorCopy("vulture4P");
	idleP[0]->Setting(0.1f, false);
	idleP[1]->Setting(0.1f, false);
	idleP[2]->Setting(0.1f, false);
	idleP[3]->Setting(0.1f, false);

	astarTimer = 0.1;
	player->m_suff += 1;
	m_maxHp = 80;
	m_hp = m_maxHp;

	range = 250;
	grid = GRIDMANAGER->AddGrid(this, 4, 4, 20, 20, -2, -1);
	grid->gridTag = rand() % 10000 + 100;
	player->AddUnit(this);
	m_attack = 14;
	img[8] = IMAGEMANAGER->FindImage("Vulture_1");
	img[7] = IMAGEMANAGER->FindImage("Vulture_2");
	img[6] = IMAGEMANAGER->FindImage("Vulture_3");
	img[5] = IMAGEMANAGER->FindImage("Vulture_4");
	img[4] = IMAGEMANAGER->FindImage("Vulture_5");
	img[3] = IMAGEMANAGER->FindImage("Vulture_6");
	img[2] = IMAGEMANAGER->FindImage("Vulture_7");
	img[1] = IMAGEMANAGER->FindImage("Vulture_8");
	img[0] = IMAGEMANAGER->FindImage("Vulture_9");
	m_isClick = false;
	lasthp = m_hp;
}

void Vulture::Update()
{
	if (isdeath == true)
	{
		SOUNDMANAGER->play("tscdth00", 0.5f);
		EFFECTMANAGER->AddEffect("bang2Effect", { position.x - 80,position.y - 100 }, 1.1, 0.07f);
		ObjectDestroyed();
	}
	if (KEYMANAGER->GetOnceKeyDown(VK_F11))
	{
		m_hp = 0;
	}
	if (m_hp <= 0 && isdeath == false)
	{
		isdeath = true;

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
	if (attackObject == nullptr)
	{
		for (auto iter : player->otherPlayer->m_units)
		{
			float dest = sqrt((iter->position.x - position.x) * (iter->position.x - position.x) + (iter->position.y - position.y) * (iter->position.y - position.y));
			if (range + 50 > dest)
			{
				attackObject = iter;
				break;
			}
		}
		if (attackObject == nullptr)
		{
			for (auto iter : player->otherPlayer->m_builds)
			{
				float dest = sqrt((iter->position.x - position.x) * (iter->position.x - position.x) + (iter->position.y - position.y) * (iter->position.y - position.y));
				if (range + 50 > dest)
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
	if (&player->m_builds == &OBJECTMANAGER->m_player->m_builds)
	{
		IMAGEMANAGER->FogUpdate(position, 30);
	}
	if (isDeath == true)
	{
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
		m_speed = 450;
		Move();
	}
	else
	{
		m_speed = 0;
	}
	CollisionUpdate();
}

void Vulture::Render()
{
	float rr = 8.f / 3.141592 * abs(imgRot);
	bool isR = false;
	if (imgRot < 0)
	{
		isR = true;
	}
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x,position.y + 10 }, 12, 8);
	}

	IMAGEMANAGER->CenterRenderBlendBlack(img[(int)rr], position, 1.5, 0, isR);
	m_isClick = false;
}

void Vulture::UIRender()
{
	m_isClick = true;
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

	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0002"]->wireImages[3][damageIndex[0]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0002"]->wireImages[1][damageIndex[1]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0002"]->wireImages[2][damageIndex[2]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);
	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->wires["wirefram0002"]->wireImages[0][damageIndex[3]], { 319 - 64.f * 1.5f / 2,680 - 64 * 1.5f / 2 }, 1.5f, 0);

	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 295,730 }, { 12,12 }, { 0,255,0,1 });

	IMAGEMANAGER->DirectDrawText(L"Sergeant", { 460,655 }, { 15,15 }, { 255,255,255,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran Vulture", { 430,625 }, { 15,15 });

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

void Vulture::Release()
{
	for (auto iter = player->m_selectUnits.begin(); iter < player->m_selectUnits.end(); iter++)
	{
		if ((*iter) == this)
		{
			player->m_selectUnits.erase(iter);
			break;
		}
	}
	player->m_suff -= 2;
}

void Vulture::Move()
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
				grid->Astar(4, 4);

			}
			if (moveNodeStack.empty() == false)
			{
				if (grid->moveStack2.empty() == true)
					grid->Astar(4, 4);
			}
		}
		else
		{
			if (grid->moveStack2.empty() == true)
				grid->Astar(4, 4);
			//여기선 다음 레기온과의 최단거리 
		}
	}

	if (grid->moveStack2.empty() == false)
	{
		d = Vector2{ (float)(grid->moveStack2.top().x * 8 * 1.5),(float)(grid->moveStack2.top().y * 8 * 1.5) };
		if (d.x != 0 && d.y != 0)
		{
			rot = atan2(d.x - position.x, d.y - position.y);
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

					astarTimer = 0.1f;
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
				imgRot = rot;
				position.x += moveDestX;
				position.y += moveDestY;
			}
		}
		else
		{
			m_speed = 0;
		}
	}
}

void Vulture::Attack()
{
	float dest = sqrt((attackObject->position.x - position.x) * (attackObject->position.x - position.x) + (attackObject->position.y - position.y) * (attackObject->position.y - position.y));
	fireImageDel2 += DELTA_TIME;
	if (range > dest)
	{
		if (fireImageDel2 > 1.25f)
		{
			attackObject->m_hp -= m_attack;
			fireImageDel2 = 0;
			VultureBullet* bull = new VultureBullet;
			bull->destPos = attackObject->position;
			OBJECTMANAGER->AddObject(bull, "Vulturebullet", position.x, position.y, 1);
		}
		m_dest = { 0,0 };
	}
	else
	{
		m_dest = attackObject->position;
		player->Astar(position, m_dest, this);
	}
}

void Vulture::CollisionUpdate()
{
	clickRect = { int(position.x - 20) , int(position.y - 20) , int(position.x + 20) , int(position.y + 20) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}
