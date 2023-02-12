#include "stdafx.h"
#include "Vulture.h"
#include "Player.h"

Vulture::Vulture()
{
}

Vulture::~Vulture()
{
}

void Vulture::Init()
{
	astarTimer = 0.1;
	player->m_suff += 1;


	grid = GRIDMANAGER->AddGrid(this, 4, 4, 20, 20, -2, -1);
	grid->gridTag = rand() % 10000 + 100;
	player->AddUnit(this);

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
}

void Vulture::Update()
{
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
	float rr = 8.f / 3.141592 * abs(rot);
	cout << rot << endl;
	bool isR = false;
	if (rot < 0)
	{
		isR = true;
	}
	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawCircle({ position.x,position.y + 10 }, 9, 6);
	}

	IMAGEMANAGER->CenterRenderBlendBlack(img[(int)rr], position, 1.5, 0, isR);
	m_isClick = false;
}

void Vulture::UIRender()
{
	m_isClick = true;
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("wirefram0000"), { 319,680 }, 1.5, 0, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 300,730 }, { 12,12 }, { 0,255,0,1 });

	IMAGEMANAGER->DirectDrawText(L"Private", { 460,655 }, { 15,15 }, { 255,255,255,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran Marine", { 430,625 }, { 15,15 });

}

void Vulture::Release()
{
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

}

void Vulture::CollisionUpdate()
{
	clickRect = { int(position.x - 20) , int(position.y - 20) , int(position.x + 20) , int(position.y + 20) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}
