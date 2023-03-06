#include "stdafx.h"
#include "Build.h"
#include "SpaceConstructionVehicle.h"

Build::Build()
{
}

Build::~Build()
{
}

void Build::Init()
{
}

void Build::Update()
{

}

void Build::Render()
{
}

void Build::Release()
{
}

void Build::UIRender()
{
}

void Build::AddBuild(SpaceConstructionVehicle* spaceConstructionVehicle)
{
	buildUnit = spaceConstructionVehicle;
	if (m_completeTime >= m_maxCompleteTime)
	{
		m_buildIndex = 4;
		isComplete = true;
	}
	else
	{
		m_completeTime += DELTA_TIME;
		if (m_hp < m_maxHp)
			m_hp += (m_maxHp / m_maxCompleteTime) * DELTA_TIME;

		if (m_buildIndex < 3)
		{
			m_buildIndex = int(4 / m_maxCompleteTime * m_completeTime);
		}
	}
}

void Build::SelectBuild()
{
	if (m_completeTime >= m_maxCompleteTime)
	{
		SelectBuildCommand();
	}
	else
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			ObjectDestroyed();
			buildUnit->m_nowBuild = nullptr;
			EFFECTMANAGER->AddEffect("f2", { position.x - 120,position.y - 90 }, 1, 0.1f);
		}
	}
}

void Build::SelectBuildCommand()
{
}
