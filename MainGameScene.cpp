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
	float _X = WINSIZE_X / 4;
	float _Y = WINSIZE_Y / 4;
	float w = 50;
	float h = 26;
	if (player2->m_builds.size() == 0)
	{
		cout << "fdsa";
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				if (x == 0 && y == 0)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0000"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else if (x == w - 1 && y == 0)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0002"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else if (x == 0 && y == h - 1)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0006"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else if (x == w - 1 && y == h - 1)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0008"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else if (y == h - 1)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0007"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else if (y == 0)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0001"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else if (x == 0)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0003"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else if (x == w - 1)
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0005"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
				else
				{
					IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("tile0004"), { _X + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,_Y + y * (IMAGEMANAGER->FindImage("tile0000")->GetHeight() - 1) * 1.5f }, { 1.5f,1.5f }, 0, 0);
				}
			}
		}
		for (int x = 0; x < 35; x++)
		{
			if (x == 0)
				IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("terran0115"), { 520 + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,430 }, { 1.5f,1.5f }, 0, 0);
			else if (x == 34)
				IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("terran0117"), { 520 + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,430 }, { 1.5f,1.5f }, 0, 0);
			else
			{
				IMAGEMANAGER->UICenterRenderBlendBlack2(IMAGEMANAGER->FindImage("terran0116"), { 520 + x * (IMAGEMANAGER->FindImage("tile0000")->GetWidth() - 1) * 1.5f,430 }, { 1.5f,1.5f }, 0, 0);
			}
		}
		IMAGEMANAGER->DirectDrawText(L"Congratulations!", { 380,220 }, { 30 ,30 });
		IMAGEMANAGER->DirectDrawText(L"You are victorious!", { 350,270 }, { 30 ,30 });
		IMAGEMANAGER->DirectDrawText(L"V", { 450,340 }, { 30 ,30 });
		IMAGEMANAGER->DirectDrawText(L"ictory", { 470,340 }, { 30 ,30 }, { 50,255,0 ,255 });
	}

	IMAGEMANAGER->DrawRegion();
}
