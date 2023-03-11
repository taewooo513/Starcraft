#include "stdafx.h"
#include "MainGameScene.h"
#include "SpaceConstructionVehicle.h"
#include "Marine.h"
#include "Barrack.h"
#include "MapReader.h"
#include "Tank.h"
MainGameScene::MainGameScene()
{
}

MainGameScene::~MainGameScene()
{
}

void MainGameScene::Init()
{
	IMAGEMANAGER->AddWireImage("wirefram0000");
	IMAGEMANAGER->AddWireImage("wirefram0002");
	IMAGEMANAGER->AddWireImage("wirefram0005");
	IMAGEMANAGER->AddWireImage("wirefram0007");
	IMAGEMANAGER->AddWireImage("wirefram0106");
	IMAGEMANAGER->AddWireImage("wirefram0113");
	IMAGEMANAGER->AddWireImage("wirefram0111");
	IMAGEMANAGER->AddWireImage("wirefram0112");
	IMAGEMANAGER->AddWireImage("wirefram0120");
	IMAGEMANAGER->AddWireImage("wirefram0123");
	IMAGEMANAGER->AddWireImage("wirefram0109");

	IMAGEMANAGER->AddWireImage("grpwire0000");
	IMAGEMANAGER->AddWireImage("grpwire0002");
	IMAGEMANAGER->AddWireImage("grpwire0005");
	IMAGEMANAGER->AddWireImage("grpwire0007");
	IMAGEMANAGER->AddWireImage("grpwire0106");
	IMAGEMANAGER->AddWireImage("grpwire0113");
	IMAGEMANAGER->AddWireImage("grpwire0111");
	IMAGEMANAGER->AddWireImage("grpwire0112");
	IMAGEMANAGER->AddWireImage("grpwire0120");
	IMAGEMANAGER->AddWireImage("grpwire0123");
	IMAGEMANAGER->AddWireImage("grpwire0109");
	SOUNDMANAGER->stop("main-menu");
	SOUNDMANAGER->play("terran-1", 0.5f);
	player1 = new Player;

	Tank* tank = new Tank;
	tank->SetPlayer(player1);
	OBJECTMANAGER->AddObject(tank, "tank", 200, 200, 1);

	player2 = new Player2;
	player2->Init();
	player1->otherPlayer = player2;
	player2->otherPlayer = player1;

	GRIDMANAGER->Init();
	player1->Init();

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
