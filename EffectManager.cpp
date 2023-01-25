#include "stdafx.h"
#include "EffectManager.h"
#include "Effect.h"
EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

Effect* EffectManager::AddEffect(string key, Vector2 pos, float size, float timeDelay)
{
	Effect* effect = new Effect;
	effect->Setting(key, pos, size, timeDelay);
	effect->Init();
	m_effects.push_back(effect);
	return effect;
}

void EffectManager::Update()
{
	if (m_effects.size() != 0)
	{
		for (auto iter = m_effects.begin(); iter != m_effects.end(); )
		{
			(*iter)->Update();

			if ((*iter)->GetDeostroy() == true)
			{
				(*iter)->Release();
				SAFE_DELETE(*iter);
				*iter = nullptr;
				iter = m_effects.erase(iter);
			}
			else
				iter++;
		}
	}
}

void EffectManager::Render()
{
	for (auto iter : m_effects)
	{
		iter->Render();
	}
}

void EffectManager::Release()
{
	for (auto iter : m_effects)
	{
		iter->Release();
		SAFE_DELETE(iter);
	}
	m_effects.clear();
}
