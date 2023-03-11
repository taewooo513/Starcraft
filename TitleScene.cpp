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


	SOUNDMANAGER->play("main-menu", 0.5f);
	/*
	"
	",
	"
	*/
	button[0] = { 650 , 210, 850, 450 };
	button[1] = { 100 , 110, 540, 250 };
	button[3] = { 600 , 510, 940,750 };

	vimage[0] = IMAGEMANAGER->AddImageVectorCopy("Editor");
	vimage[1] = IMAGEMANAGER->AddImageVectorCopy("Single Player");
	vimage[2] = IMAGEMANAGER->AddImageVectorCopy("Multi Player");

	vImage2[0] = IMAGEMANAGER->AddImageVectorCopy("editoron");
	vImage2[1] = IMAGEMANAGER->AddImageVectorCopy("singleon");
	vImage2[2] = IMAGEMANAGER->AddImageVectorCopy("exiton");

	vimage[3] = IMAGEMANAGER->AddImageVectorCopy("Exit");
	vimage[0]->Setting(0.1, true);
	vimage[1]->Setting(0.1, true);
	vimage[2]->Setting(0.1, true);
	vimage[3]->Setting(0.1, true);

	vImage2[0]->Setting(0.1, true);
	vImage2[1]->Setting(0.1, true);
	vImage2[2]->Setting(0.1, true);
}

void TitleScene::Update()
{
}

void TitleScene::Render()
{

	IMAGEMANAGER->Render(IMAGEMANAGER->FindImage("backgnd"), Vector2{ 0,0 }, 1.71, 0);
	if (PtInRect(&button[1], _ptMouse))
	{
		vImage2[1]->CenterRenderBlendBlack({ 140,150 }, 1.7, 0, 0);
		if (KEYMANAGER->GetOnceKeyDown(VK_LBUTTON))
		{
			SCENEMANAGER->ChangeScene("MainGameScene");
		}
	}
	else
	{
		vImage2[1]->Reset();
	}
	vimage[1]->CenterRenderBlendBlack({ 50,50 }, 1.7, 0, 0);
	vimage[2]->CenterRenderBlendBlack({ 100,300 }, 1.7, 0, 0);
	IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("mul-tail"), { 425,660 }, 1.7, 0, 0);
	vimage[0]->CenterRenderBlendBlack({ 600,200 }, 1.7, 0, 0);
	vimage[3]->CenterRenderBlendBlack({ 600,500 }, 1.7, 0, 0);
	if (PtInRect(&button[0], _ptMouse))
	{
		vImage2[0]->CenterRenderBlendBlack({ 640,225 }, 1.7, 0, 0);
		if (KEYMANAGER->GetOnceKeyDown(VK_LBUTTON))
		{
			SCENEMANAGER->ChangeScene("MapToolScene");
		}
	}
	else
	{
		vImage2[0]->Reset();
	}

	if (PtInRect(&button[3], _ptMouse))
	{
		vImage2[2]->CenterRenderBlendBlack({ 640,500 }, 1.7, 0, 0);
		if (KEYMANAGER->GetOnceKeyDown(VK_LBUTTON))
		{
			exit(0);
		}
	}
	else
	{
		vImage2[2]->Reset();
	}
	//IMAGEMANAGER->DrawRect({ (float)button[0].left , (float)button[0].top }, { (float)button[0].right, (float)button[0].bottom });
	//IMAGEMANAGER->DrawRect({ (float)button[1].left , (float)button[1].top }, { (float)button[1].right, (float)button[1].bottom });
	//IMAGEMANAGER->DrawRect({ (float)button[3].left , (float)button[3].top }, { (float)button[3].right, (float)button[3].bottom });


}

void TitleScene::Release()
{
	SAFE_DELETE(vimage[0]);
	SAFE_DELETE(vimage[1]);
	SAFE_DELETE(vimage[2]);
	SAFE_DELETE(vimage[3]);
}

void TitleScene::UIRender()
{
}
