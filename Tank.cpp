#include "stdafx.h"
#include "Tank.h"
#include "Player.h"

Tank::Tank()
{
}

Tank::~Tank()
{
}

void Tank::Init()
{
	sMode = IMAGEMANAGER->AddImageVectorCopy("sTank");
	sModeT = IMAGEMANAGER->AddImageVectorCopy("sTankT");

	range = 250;

	sMode->Setting(0.2f, false);
	sModeT->Setting(0.0125f, false);

	m_attack = 30;

	astarTimer = 0.1;
	player->m_suff += 1;
	m_maxHp = 80;
	m_hp = m_maxHp;

	grid = GRIDMANAGER->AddGrid(this, 4, 4, 20, 20, -2, -1);
	grid->gridTag = rand() % 10000 + 100;
	player->AddUnit(this);

	img[8] = IMAGEMANAGER->FindImage("tank0000");
	img[7] = IMAGEMANAGER->FindImage("tank0002");
	img[6] = IMAGEMANAGER->FindImage("tank0004");
	img[5] = IMAGEMANAGER->FindImage("tank0006");
	img[4] = IMAGEMANAGER->FindImage("tank0008");
	img[3] = IMAGEMANAGER->FindImage("tank0010");
	img[2] = IMAGEMANAGER->FindImage("tank0012");
	img[1] = IMAGEMANAGER->FindImage("tank0014");
	img[0] = IMAGEMANAGER->FindImage("tank0016");

	img1[8] = IMAGEMANAGER->FindImage("tank0018");
	img1[7] = IMAGEMANAGER->FindImage("tank0020");
	img1[6] = IMAGEMANAGER->FindImage("tank0022");
	img1[5] = IMAGEMANAGER->FindImage("tank0024");
	img1[4] = IMAGEMANAGER->FindImage("tank0026");
	img1[3] = IMAGEMANAGER->FindImage("tank0028");
	img1[2] = IMAGEMANAGER->FindImage("tank0030");
	img1[1] = IMAGEMANAGER->FindImage("tank0032");
	img1[0] = IMAGEMANAGER->FindImage("tank0033");

	img2[8] = IMAGEMANAGER->FindImage("tank0035");
	img2[7] = IMAGEMANAGER->FindImage("tank0037");
	img2[6] = IMAGEMANAGER->FindImage("tank0039");
	img2[5] = IMAGEMANAGER->FindImage("tank0041");
	img2[4] = IMAGEMANAGER->FindImage("tank0043");
	img2[3] = IMAGEMANAGER->FindImage("tank0045");
	img2[2] = IMAGEMANAGER->FindImage("tank0047");
	img2[1] = IMAGEMANAGER->FindImage("tank0049");
	img2[0] = IMAGEMANAGER->FindImage("tank0050");

	imgT[8] = IMAGEMANAGER->FindImage("tankt0000");
	imgT[7] = IMAGEMANAGER->FindImage("tankt0002");
	imgT[6] = IMAGEMANAGER->FindImage("tankt0004");
	imgT[5] = IMAGEMANAGER->FindImage("tankt0006");
	imgT[4] = IMAGEMANAGER->FindImage("tankt0008");
	imgT[3] = IMAGEMANAGER->FindImage("tankt0010");
	imgT[2] = IMAGEMANAGER->FindImage("tankt0012");
	imgT[1] = IMAGEMANAGER->FindImage("tankt0014");
	imgT[0] = IMAGEMANAGER->FindImage("tankt0016");

	imgsT[8] = IMAGEMANAGER->FindImage("stankt0000");
	imgsT[7] = IMAGEMANAGER->FindImage("stankt0002");
	imgsT[6] = IMAGEMANAGER->FindImage("stankt0004");
	imgsT[5] = IMAGEMANAGER->FindImage("stankt0006");
	imgsT[4] = IMAGEMANAGER->FindImage("stankt0008");
	imgsT[3] = IMAGEMANAGER->FindImage("stankt0010");
	imgsT[2] = IMAGEMANAGER->FindImage("stankt0012");
	imgsT[1] = IMAGEMANAGER->FindImage("stankt0014");
	imgsT[0] = IMAGEMANAGER->FindImage("stankt0016");
	m_isClick = false;
}

void Tank::Update()
{
	if (isSMode == true)
	{
		range = 500;
	}
	else
	{
		range = 250;
	}
	if (attackObject == nullptr)
	{
		for (auto iter : player->otherPlayer->m_units)
		{
			float dest = sqrt((iter->position.x - position.x) * (iter->position.x - position.x) + (iter->position.y - position.y) * (iter->position.y - position.y));
			if (range > dest)
			{
				attackObject = iter;
				break;
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
	IMAGEMANAGER->FogUpdate(position, 30);

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
	if (isSMode == false)
	{
		if (m_dest.x != 0 && m_dest.y != 0)
		{
			m_speed = 150;
			moveFrameTime += DELTA_TIME;

			if (moveFrameTime > 0.1)
			{
				if (moveFrame < 2)
				{
					moveFrame++;
				}
				else
				{
					moveFrame = 0;
				}
				moveFrameTime = 0;
			}

			Move();
		}
		else
		{
			m_speed = 0;
		}

	}
	CollisionUpdate();
}

void Tank::Render()
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
	if (isSMode == false)
	{
		if (moveFrame == 0)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(img[(int)rr], position, 1.5, 0, isR);
		}
		else if (moveFrame == 1)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(img1[(int)rr], position, 1.5, 0, isR);
		}
		else if (moveFrame == 2)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(img2[(int)rr], position, 1.5, 0, isR);
		}
		IMAGEMANAGER->CenterRenderBlendBlack(imgT[(int)rr], position, 1.5, 0, isR);
	}
	else
	{
		if (sMode->GetIsEnd() == true)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("sTank"), position, 1.5, 0, isR);

		}
		else
		{
			sMode->CenterRenderBlendBlack({ position.x - 100,position.y - 100 }, 1.5, 0, 0);
		}

		if (sModeT->GetIsEnd() == true)
		{
			IMAGEMANAGER->CenterRenderBlendBlack(imgsT[(int)rr], position, 1.5, 0, isR);
		}
		else
		{
			sModeT->CenterRenderBlendBlack({ position.x - 100,position.y - 100 }, 1.5, 0, 0);
		}
	}
	m_isClick = false;
}

void Tank::UIRender()
{
	m_isClick = true;
	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0230"), { UIPosition[2].x - 1 ,UIPosition[2].y - 2 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0229"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0228"), { UIPosition[0].x - 1 ,UIPosition[0].y - 2 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[4].x + 25,UIPosition[4].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0231"), { UIPosition[4].x  ,UIPosition[4].y + 4 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[3].x + 25,UIPosition[3].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0232"), { UIPosition[3].x - 2 ,UIPosition[3].y - 2 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[6].x + 25,UIPosition[6].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0234"), { UIPosition[6].x + 3,UIPosition[6].y + 5 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[7].x + 25,UIPosition[7].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->DrawUI2(IMAGEMANAGER->FindImage("cmdicons0235"), { UIPosition[7].x + 3,UIPosition[7].y + 5 }, 1.7, 0, 0);

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("wirefram0002"), { 319,680 }, 1.5, 0, 0);
	IMAGEMANAGER->DirectDrawText(to_wstring((int)m_hp) + L"/" + to_wstring((int)m_maxHp), { 300,730 }, { 12,12 }, { 0,255,0,1 });

	IMAGEMANAGER->DirectDrawText(L"Sergeant", { 460,655 }, { 15,15 }, { 255,255,255,1 });

	IMAGEMANAGER->DirectDrawText(L"Terran Tank", { 430,625 }, { 15,15 });

	if (KEYMANAGER->GetOnceKeyDown('S') && (sMode->GetIsEnd() == true || isSMode == false))
	{
		isSMode = true;
	}
}

void Tank::Release()
{
}

void Tank::Move()
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
	grid->Update();
}

void Tank::Attack()
{
	float dest = sqrt((attackObject->position.x - position.x) * (attackObject->position.x - position.x) + (attackObject->position.y - position.y) * (attackObject->position.y - position.y));
	fireImageDel2 += DELTA_TIME;

	if (range > dest)
	{
		imgRot = atan2(attackObject->position.x - position.x, attackObject->position.y - position.y);
		if (fireImageDel2 > (isSMode == false ? 1.5f : 3.1f))
		{
			attackObject->m_hp -= m_attack;
			fireImageDel2 = 0;
			if (isSMode == true)
			{
				SOUNDMANAGER->play("ttafi200", 0.5f);
				EFFECTMANAGER->AddEffect("tmnexplo0000", { attackObject->position.x - 30,  attackObject->position.y - 30 }, 1, 0.05f);
			}
			else
			{
				SOUNDMANAGER->play("ttafir00", 0.5f);
				EFFECTMANAGER->AddEffect("bang1Effect", { attackObject->position.x - 30,  attackObject->position.y - 30 }, 0.5, 0.05f);
			}
		}
		m_dest = { 0,0 };
	}
	else
	{
		m_dest = attackObject->position;
		player->Astar(position, m_dest, this);
	}
}

void Tank::CollisionUpdate()
{
	clickRect = { int(position.x - 20) , int(position.y - 20) , int(position.x + 20) , int(position.y + 20) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}
