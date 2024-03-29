#include "stdafx.h"
#include "Mineral.h"

Mineral::Mineral()
{
}

Mineral::~Mineral()
{
}

void Mineral::MineMineral()
{
	nowCount -= 8;
}

void Mineral::Init()
{
	nowMineUnit = nullptr;
	resourceImage[0] = IMAGEMANAGER->FindImage("min010000");
	resourceImage[1] = IMAGEMANAGER->FindImage("min010001");
	resourceImage[2] = IMAGEMANAGER->FindImage("min010002");
	resourceImage[3] = IMAGEMANAGER->FindImage("min010003");

	nowCount = maxCount;
	resourceshadImage[0] = IMAGEMANAGER->FindImage("min01sha0000");
	resourceshadImage[1] = IMAGEMANAGER->FindImage("min01sha0001");
	resourceshadImage[2] = IMAGEMANAGER->FindImage("min01sha0002");
	resourceshadImage[3] = IMAGEMANAGER->FindImage("min01sha0003");

	grid = GRIDMANAGER->AddGrid(this, 7, 4, 20, 20, -3, -2);
	grid->gridTag = rand() % 100000 + 100;
}

void Mineral::Update()
{
	grid->Update();
	clickRect = { int(position.x) , int(position.y) , int((position.x + 32 * 4 * 1.5f)) , int((position.y + 32 * 3 * 1.5f)) };
	clickRect.left -= IMAGEMANAGER->GetCameraPosition().x + 50;
	clickRect.right -= IMAGEMANAGER->GetCameraPosition().x + 145;
	clickRect.bottom -= IMAGEMANAGER->GetCameraPosition().y + 110;
	clickRect.top -= IMAGEMANAGER->GetCameraPosition().y + 50;
}

void Mineral::Render()
{

	if (KEYMANAGER->GetOnceKeyDown(VK_F1))
	{
		nowCount -= 500;
	}
	if (maxCount / 4 * 3 < nowCount)
	{
		IMAGEMANAGER->CenterRenderBlendBlack(resourceshadImage[0], position, 1.5, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[0], position, 1.5, 0, 0);

	}
	else if (maxCount / 4 * 2 < nowCount)
	{
		IMAGEMANAGER->CenterRenderBlendBlack(resourceshadImage[1], position, 1.5, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[1], position, 1.5, 0, 0);

	}
	else if (maxCount / 4 * 1 < nowCount)
	{

		IMAGEMANAGER->CenterRenderBlendBlack(resourceshadImage[2], position, 1.5, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[2], position, 1.5, 0, 0);
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(resourceshadImage[3], position, 1.5, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[3], position, 1.5, 0, 0);
	}
}

void Mineral::Release()
{
}

void Mineral::UIRender()
{

}
