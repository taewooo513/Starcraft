#include "stdafx.h"
#include "MainGameScene.h"
#include "SpaceConstructionVehicle.h"
#include "Barrack.h"
#include "MapReader.h"
MainGameScene::MainGameScene()
{
}

MainGameScene::~MainGameScene()
{
}

void MainGameScene::Init()
{
	OBJECTMANAGER->AddObject(new SpaceConstructionVehicle, "Unit", WINSIZE_X/2, WINSIZE_Y/2, 1);
}

void MainGameScene::Update()
{
}

void MainGameScene::Render()
{
	//IMAGEMANAGER->Render(IMAGEMANAGER->FindImage("titleBackGround"), { 0,0 }, 1.7f, 0);
}

void MainGameScene::Release()
{
}

void MainGameScene::UIRender()
{
	IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("playerUI"), { 0,0 }, 1.6f, 0);
	IMAGEMANAGER->MapRender();
}
