#include "stdafx.h"
#include "VultureBullet.h"

VultureBullet::VultureBullet()
{
}

VultureBullet::~VultureBullet()
{
	SOUNDMANAGER->play("tvuhit02", 0.5f);
}

void VultureBullet::Init()
{
	img = IMAGEMANAGER->AddImageVectorCopy("grenade0000");
	img->Setting(0.1f, false);
	SOUNDMANAGER->play("tvufir00", 0.5f);
	rot = atan2(destPos.y - position.y, destPos.x - position.x);
}

void VultureBullet::Update()
{
	position.x += cos(rot) * DELTA_TIME * 500;
	position.y += sin(rot) * DELTA_TIME * 500;

	if (img->GetIsEnd() == true)
	{
		EFFECTMANAGER->AddEffect("bang1Effect", { position.x - 30,  position.y - 30 }, 0.5, 0.05f);
		ObjectDestroyed();
	}
}

void VultureBullet::Render()
{
	img->CenterRenderBlendBlack(position, 1.8f, 0, 0);
}

void VultureBullet::Release()
{
}

void VultureBullet::UIRender()
{
}
