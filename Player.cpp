#include "stdafx.h"
#include "Player.h"
#include "SpaceConstructionVehicle.h"
#include "CommandCenter.h"

void Player::Astar()
{
}

void Player::Init()
{
	m_clickRad = 0;
	sizeClick = 1.7;
	m_isCameraClick = true;
	m_cursorImage = IMAGEMANAGER->AddImageVectorCopy("arrow0000");
	m_cursorImage->Setting(0.15, true);
	for (int i = 0; i < 5; i++)
	{
		Unit* scv = new SpaceConstructionVehicle;
		scv->SetPlayer(this);
		OBJECTMANAGER->AddObject(scv, "Unit", WINSIZE_X / 2, WINSIZE_Y / 2, 1);
		//m_units.push_back(scv);
	}
	Build* commandCenter = new CommandCenter;
	commandCenter->SetPlayer(this);
	OBJECTMANAGER->AddObject(commandCenter, "Build", WINSIZE_X / 2, WINSIZE_Y / 2, 0);
	commandCenter->AddComplete();
	//m_builds.push_back(commandCenter);

	m_selectBuild = nullptr;
	m_selectUnit = nullptr;

	mapRect = {
		11,
		560,
		11 + int(4096 * 0.0495f),
		560 + int(4096 * 0.0495f)
	};
}

void Player::Update()
{
	if (m_selectBuild != nullptr)
	{
		m_selectBuild->SelectBuild();
	}

	if (KEYMANAGER->GetOnceKeyDown(VK_RBUTTON))
	{
		m_clickRad = 0;
		m_rClickPos.x = _ptMouse.x + IMAGEMANAGER->GetCameraPosition().x;
		m_rClickPos.y = _ptMouse.y + IMAGEMANAGER->GetCameraPosition().y;
		if (m_selectUnit != nullptr)
		{
			m_selectUnit->rot2 = atan2(m_rClickPos.y - m_selectUnit->GetPosition().y, m_rClickPos.x - m_selectUnit->GetPosition().x);

			m_selectUnit->SetDestPosition(m_rClickPos);
		}
		for (auto iter : m_selectUnits)
		{
			iter->rot2 = atan2(m_rClickPos.y - iter->GetPosition().y, m_rClickPos.x - iter->GetPosition().x);

			iter->SetDestPosition(m_rClickPos);
		}
	}

	if (KEYMANAGER->GetOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&mapRect, _ptMouse))
		{
			m_isCameraClick = true;
		}
		else if (_ptMouse.y < 600)
		{
			m_isClick = true;
			m_clickStartX = _ptMouse.x;
			m_clickStartY = _ptMouse.y;

			m_clickEndX = _ptMouse.x;
			m_clickEndY = _ptMouse.y;
		}
		if (m_selectUnit != nullptr)
		{
			if (typeid(*m_selectUnit).name() == typeid(SpaceConstructionVehicle).name())
			{
				m_rClickPos.x = _ptMouse.x + IMAGEMANAGER->GetCameraPosition().x;
				m_rClickPos.y = _ptMouse.y + IMAGEMANAGER->GetCameraPosition().y;
				SpaceConstructionVehicle* scv = dynamic_cast<SpaceConstructionVehicle*>(m_selectUnit);
				if (scv->buildIndex != 0)
				{
					m_selectUnit->SetDestPosition({ (float)_ptMouse.x,(float)_ptMouse.y });
					m_selectUnit->rot2 = atan2(m_rClickPos.y - m_selectUnit->GetPosition().y, m_rClickPos.x - m_selectUnit->GetPosition().x);
					scv->m_isBuild = true;
				}
			}
		}
	}
	else if (KEYMANAGER->GetStayKeyDown(VK_LBUTTON))
	{
		if (m_isClick == true)
		{
			m_clickEndX = _ptMouse.x;
			if (_ptMouse.y < 600)
			{
				m_clickEndY = _ptMouse.y;
			}
			else
			{
				m_clickEndY = 600;
			}
		}
		else if (m_isCameraClick)
		{
			int mapWidth = mapRect.right - mapRect.left;
			int mapHeight = mapRect.bottom - mapRect.top;

			float xx = (mapWidth / 4096.f * WINSIZE_X / 2);
			float yy = (mapWidth / 4296.f * WINSIZE_Y / 2);
			float mouseMapX = _ptMouse.x - xx - mapRect.left > 0 ? _ptMouse.x - xx - mapRect.left : 0;
			float mouseMapY = _ptMouse.y - yy - mapRect.top > 0 ? _ptMouse.y - yy - mapRect.top : 0;
			mapWidth -= xx * 3;
			mapHeight -= yy * 3;

			float camPosX = mouseMapX * ((4096.f) / mapWidth) < 4096 + WINSIZE_X - 20 ? mouseMapX * ((4096.f) / mapWidth) : 4096 + WINSIZE_X - 20;
			float camPosY = mouseMapY * ((4296.f / mapHeight)) < 4296.f + WINSIZE_Y - 20 ? mouseMapY * ((4296.f) / mapHeight) : 4296.f + WINSIZE_Y - 20;
			IMAGEMANAGER->CameraSetPosition({ camPosX ,camPosY });
		}
	}
	if (m_isClick == true && KEYMANAGER->GetOnceKeyUp(VK_LBUTTON))
	{
		RECT pRt;
		if (m_clickStartX > m_clickEndX)
		{
			float swap = m_clickEndX;
			m_clickEndX = m_clickStartX;
			m_clickStartX = swap;
		}
		if (m_clickStartY > m_clickEndY)
		{
			float swap = m_clickEndY;
			m_clickEndY = m_clickStartY;
			m_clickStartY = swap;
		}

		for (auto iter : m_builds)
		{

			RECT rtMouse = { m_clickStartX  ,
			m_clickStartY ,
				m_clickEndX ,
				m_clickEndY
			};
			if (IntersectRect(&pRt, &rtMouse, &iter->GetClickRect()))
			{
				m_selectBuild = iter;
				m_selectUnit = nullptr;
				m_selectUnits.clear();
				break;
			}
		}

		m_isClick = false;
		m_isCameraClick = false;
		int count = 0;
		for (auto iter : m_units)
		{
			RECT rtMouse = { m_clickStartX  ,
			m_clickStartY ,
				m_clickEndX ,
				m_clickEndY
			};

			RECT pRt;

			if (abs(m_clickEndX - m_clickStartX) > 0.1f)
			{
				if (IntersectRect(&pRt, &rtMouse, &iter->GetClickRect()))
				{
					if (count == 0)
					{
						m_selectUnits.clear();
						m_selectUnit = nullptr;
						m_selectBuild = nullptr;
					}
					m_selectUnits.push_back(iter);
					count++;
				}
			}
			else
			{
				if (PtInRect(&iter->GetClickRect(), _ptMouse))
				{
					if (m_selectUnits.size() != 0)
						m_selectUnits.clear();
					m_selectUnit = iter;
					m_selectBuild = nullptr;
					break;
				}
			}
		}
		if (m_selectUnits.size() == 1)
		{
			m_selectUnit = m_selectUnits[0];
			m_selectUnits.clear();
		}
	}

	if (m_isClick == false)
	{
		if (KEYMANAGER->GetStayKeyDown(VK_LEFT))
		{
			if (IMAGEMANAGER->GetCameraPosition().x > 5)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x - DELTA_TIME * 800, IMAGEMANAGER->GetCameraPosition().y });
			}
		}
		if (KEYMANAGER->GetStayKeyDown(VK_RIGHT))
		{
			if (IMAGEMANAGER->GetCameraPosition().x < 128 * 32 * 1.5f - WINSIZE_X)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x + DELTA_TIME * 800, IMAGEMANAGER->GetCameraPosition().y });
			}
		}
		if (KEYMANAGER->GetStayKeyDown(VK_DOWN))
		{
			if (IMAGEMANAGER->GetCameraPosition().y < 128 * 32 * 1.5f - WINSIZE_Y)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x , IMAGEMANAGER->GetCameraPosition().y + DELTA_TIME * 800 });
			}
		}
		if (KEYMANAGER->GetStayKeyDown(VK_UP))
		{
			if (IMAGEMANAGER->GetCameraPosition().y > 5)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x , IMAGEMANAGER->GetCameraPosition().y - DELTA_TIME * 800 });
			}
		}
	}

}

void Player::Render()
{
	IMAGEMANAGER->MapRender();
}

void Player::UIRender()
{
	if (m_clickRad < 3.141592)
	{
		m_clickRad += DELTA_TIME * 30;
	}
	else
	{
		m_rClickPos = { -100,-100 };
	}

	IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("targg0001"), { (float)m_rClickPos.x - IMAGEMANAGER->GetCameraPosition().x,(float)m_rClickPos.y - IMAGEMANAGER->GetCameraPosition().y }, sizeClick + sin(m_clickRad) * 0.6, 0, 0);

	if (m_isClick == true)
	{
		IMAGEMANAGER->DrawRect({ m_clickStartX,m_clickStartY }, { m_clickEndX, m_clickEndY });
	}

	IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("playerUI"), { 0,0 }, 1.6f, 0);
	IMAGEMANAGER->UIMapRender();

	if (m_isClick == false)
	{
		m_cursorImage->CenterRenderBlendBlack(Vector2{ (float)_ptMouse.x - 110,(float)_ptMouse.y - 110 } + IMAGEMANAGER->GetCameraPosition(), 1.7f, 0, 0);
	}
	else
	{
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("drag0000"), { (float)_ptMouse.x - 110,(float)_ptMouse.y - 110 }, 1.7f, 0);
	}

	int count = 0;
	for (auto iter : m_selectUnits)
	{
		iter->m_isClick = true;
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0018"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.8, 0);
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("grpwire0007"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 1.7, 0);
		count++;
	}
	float mapWidth = mapRect.right - mapRect.left;
	float mapHeight = mapRect.bottom - mapRect.top;

	float xx = (mapWidth / (4096.f + WINSIZE_X * 2.f) * IMAGEMANAGER->GetCameraPosition().x);
	float yy = (mapHeight / (4096.f + WINSIZE_Y * 2.5f) * IMAGEMANAGER->GetCameraPosition().y);
	IMAGEMANAGER->DrawRect({ 10 + xx ,560 + yy }, { 10 + xx + float(mapWidth / (4096.f + WINSIZE_X * 2.f) * WINSIZE_X), 560 + yy + float(mapHeight / (4096.f + WINSIZE_Y * 2.5f) * WINSIZE_Y) });


	if (m_selectUnit != nullptr)
	{
		m_selectUnit->UIRender();
	}
	if (m_selectBuild != nullptr)
	{
		m_selectBuild->UIRender();
	}
}

void Player::Release()
{
	SAFE_DELETE(m_cursorImage);
}

