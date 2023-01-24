#include "stdafx.h"
#include "Build.h"

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

void Build::AddBuild()
{
	if (m_completeTime >= m_maxCompleteTime)
	{
		m_buildIndex = 4;
	}
	else
	{
		if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
		{
			ObjectDestroyed();
			EFFECTMANAGER->AddEffect("f2", { position.x - 120,position.y - 90 }, 1.5, 0.1f);
		}

		m_completeTime += DELTA_TIME;
		if (m_hp < m_maxHp)
			m_hp += (m_maxHp / m_maxCompleteTime) * DELTA_TIME;

		if (m_buildIndex < 3)
		{
			m_buildIndex = int(m_completeTime) / int(m_maxCompleteTime / 4);
		}
		cout << m_hp << endl;
		cout << m_completeTime << endl;
	}
}
