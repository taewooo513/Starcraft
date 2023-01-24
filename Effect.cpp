#include "stdafx.h"
#include "Effect.h"

void Effect::Setting(string key, Vector2 pos, float scale, float timeDelay)
{
	m_image = IMAGEMANAGER->FindImageVector(key);
	m_image->Setting(timeDelay, false);

	m_scale = scale;
	position = pos;
}

void Effect::Init()
{
}

void Effect::Update()
{
	if (m_image->GetIsEnd())
	{
		m_isDestroy = true;
	}
}

void Effect::Render()
{
	m_image->CenterRenderBlendBlack(position, m_scale, 0, false);
}

void Effect::Release()
{
	m_image->Release();
	SAFE_DELETE(m_image);
}
