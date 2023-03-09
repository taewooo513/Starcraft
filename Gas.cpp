#include "stdafx.h"
#include "Gas.h"

Gas::Gas()
{
}

Gas::~Gas()
{
}

void Gas::MineGas()
{
}

void Gas::Init()
{
	img = IMAGEMANAGER->FindImage("gas2");
	grid = GRIDMANAGER->AddGrid(this, 14, 7, 0,0, -8, -3);
	grid->gridTag = -1;
}

void Gas::Update()
{
	grid->Update();
}

void Gas::Render()
{
	IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("gass"), position, 1.5f, 0, 0);
	IMAGEMANAGER->CenterRenderBlendBlack(img, position, 1.5f, 0, 0);
}

void Gas::Release()
{
}

void Gas::UIRender()
{
}
