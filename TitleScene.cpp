#include "stdafx.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
}

void TitleScene::Update()
{
}

void TitleScene::Render()
{
	IMAGEMANAGER->Render(IMAGEMANAGER->FindImage("titleBackGround"), Vector2{ 0,0 }, 1.71, 0);
}

void TitleScene::Release()
{
}

void TitleScene::UIRender()
{
}
