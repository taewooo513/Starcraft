#include "stdafx.h"
#include "MainGameScene.h"
#include "SpaceConstructionVehicle.h"
#include "Marine.h"
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
	player1 = new Player;
	GRIDMANAGER->Init();
	player1->Init();
	OBJECTMANAGER->AddObject(new Mineral, "mimeral", 500, 50, 0);
}

void MainGameScene::Update()
{
	GRIDMANAGER->Update();
	player1->Update();
}

void MainGameScene::Render()
{
	player1->Render();
	//IMAGEMANAGER->Render(IMAGEMANAGER->FindImage("titleBackGround"), { 0,0 }, 1.7f, 0);
}

void MainGameScene::Release()
{
	player1->Release();

	SAFE_DELETE(player1);
}

void MainGameScene::UIRender()
{
	player1->UIRender();

	IMAGEMANAGER->DrawRegion();
}
