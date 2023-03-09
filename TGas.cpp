#include "stdafx.h"
#include "TGas.h"

TGas::TGas()
{
}

TGas::~TGas()
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

void TGas::Init()
{
	player->AddBuild(this);
	m_buildImage[0] = IMAGEMANAGER->FindImage("refinery0001");
	m_buildImage[1] = IMAGEMANAGER->FindImage("refinery0002");
	m_buildImage[2] = IMAGEMANAGER->FindImage("refinery0003");
	m_buildImage[3] = IMAGEMANAGER->FindImage("refinery0004");

	m_maxCompleteTime = 10.5f;
	m_completeTime = 0;
	m_costM = 100;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1000;
	m_hp = 1;
}

void TGas::Update()
{
	IMAGEMANAGER->FogUpdate(position, 30);

	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y;
}

void TGas::Render()
{
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
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("refinery0000"), { position.x ,position.y }, 1.5, 0);
	}
	m_isClick = false;
}

void TGas::Release()
{
}

void TGas::UIRender()
{
	m_isClick = true;

	IMAGEMANAGER->DirectDrawText(L"Terran TGas", { 400,625 }, { 15,15 });
}
