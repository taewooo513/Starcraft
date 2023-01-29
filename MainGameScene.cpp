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
	player1 = new Player;
	player1->Init();
}

void MainGameScene::Update()
{
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
}
