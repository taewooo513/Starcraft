#include "stdafx.h"
#include "Player.h"
#include "SpaceConstructionVehicle.h"

void Player::Init()
{
	m_clickRad = 0;
	sizeClick = 1.7;
	m_isCameraClick = true;
	m_cursorImage = IMAGEMANAGER->AddImageVectorCopy("arrow0000");
	m_cursorImage->Setting(0.15, true);
	for (int i = 0; i < 6; i++)
	{
		Unit* scv = new SpaceConstructionVehicle;
		OBJECTMANAGER->AddObject(scv, "Unit", WINSIZE_X / 2, WINSIZE_Y / 2, 1);
		m_units.push_back(scv);
	}
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
	cout << _ptMouse.x << endl;
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

			float mouseMapX = _ptMouse.x - mapRect.left;
			float mouseMapY = _ptMouse.y - mapRect.top;

			if (mouseMapX > 0 && mouseMapY > 0)
				IMAGEMANAGER->CameraSetPosition({ mouseMapX * ((4096.f) / mapWidth),mouseMapY * (4096.f / mapHeight) });
		}
	}
	if (m_isClick == true && KEYMANAGER->GetOnceKeyUp(VK_LBUTTON))
	{
		m_isClick = false;
		m_selectUnit = nullptr;
		m_selectUnits.clear();
		m_isCameraClick = false;
		for (auto iter : m_units)
		{
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
					m_selectUnits.push_back(iter);
				}
			}
			else
			{
				if (PtInRect(&iter->GetClickRect(), _ptMouse))
				{
					m_selectUnit = iter;
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
			if (IMAGEMANAGER->GetCameraPosition().x < 128 * 32 * 1.7f - WINSIZE_X)
			{
				IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x + DELTA_TIME * 800, IMAGEMANAGER->GetCameraPosition().y });
			}
		}
		if (KEYMANAGER->GetStayKeyDown(VK_DOWN))
		{
			if (IMAGEMANAGER->GetCameraPosition().y < 128 * 32 * 1.7f - WINSIZE_Y)
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
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0018"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 0.8, 0);
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("grpwire0007"), { 265 + 60 * float(count / 2),630 + 60 * (float)(count % 2) }, 1.7, 0);
		count++;
	}

	if (m_selectUnit != nullptr)
	{
		m_selectUnit->UIRender();
	}
}

void Player::Release()
{
	SAFE_DELETE(m_cursorImage);
}

