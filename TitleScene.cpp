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
	vimage[0] = IMAGEMANAGER->AddImageVectorCopy("Editor");
	vimage[1] = IMAGEMANAGER->AddImageVectorCopy("Single Player");
	vimage[2] = IMAGEMANAGER->AddImageVectorCopy("Multi Player");
	vimage[3] = IMAGEMANAGER->AddImageVectorCopy("Exit");
	vimage[0]->Setting(0.1, true);
	vimage[1]->Setting(0.1, true);
	vimage[2]->Setting(0.1, true);
	vimage[3]->Setting(0.1, true);
}

void TitleScene::Update()
{
}

void TitleScene::Render()
{
	IMAGEMANAGER->Render(IMAGEMANAGER->FindImage("backgnd"), Vector2{ 0,0 }, 1.71, 0);
	vimage[1]->CenterRenderBlendBlack({ 50,50 },1.7, 0, 0);
	vimage[2]->CenterRenderBlendBlack({ 100,300 },1.7, 0, 0);
	IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("mul-tail"), {425,660}, 1.7, 0, 0);
	vimage[0]->CenterRenderBlendBlack({ 600,200 },1.7, 0, 0);
	vimage[3]->CenterRenderBlendBlack({ 600,500 },1.7, 0, 0);
}

void TitleScene::Release()
{
}

void TitleScene::UIRender()
{
}
