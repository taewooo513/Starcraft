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

	grid = GRIDMANAGER->AddGrid(this, 7, 4, 20, 20, -3, -2);
	grid->gridTag = rand() % 100000 + 100;
}

void Mineral::Update()
{
}

void Mineral::Render()
{
	if (KEYMANAGER->GetOnceKeyDown(VK_F1))
	{
		nowCount -= 500;
	}
	if (maxCount / 4 * 3 < nowCount)
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[0], position, 1.5, 0, 0);
	else if (maxCount / 4 * 2 < nowCount)
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[1], position, 1.5, 0, 0);
	else if (maxCount / 4 * 1 < nowCount)
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[2], position, 1.5, 0, 0);
	else
		IMAGEMANAGER->CenterRenderBlendBlack(resourceImage[3], position, 1.5, 0, 0);
}

void Mineral::Release()
{
}

void Mineral::UIRender()
{

}
