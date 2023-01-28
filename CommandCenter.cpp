#include "stdafx.h"
#include "CommandCenter.h"

CommandCenter::CommandCenter()
{
}

CommandCenter::~CommandCenter()
{
}

void CommandCenter::Init()
{
	m_maxCompleteTime = 75.8f;
	m_completeTime = 0;
	m_costM = 400;
	m_costG = 0;
	m_buildIndex = 0;
	m_maxHp = 1500;
	m_hp = 1;

	idle = IMAGEMANAGER->FindImage("control0000");
	buildImage[0] = IMAGEMANAGER->FindImage("tbldlrg0000");
	buildImage[1] = IMAGEMANAGER->FindImage("tbldlrg0001");
	buildImage[2] = IMAGEMANAGER->FindImage("tbldlrg0002");
	buildImage[3] = IMAGEMANAGER->FindImage("control0001");

}

void CommandCenter::Update()
{
}

void CommandCenter::Render()
{
	if (m_buildIndex < 4)
	{
		IMAGEMANAGER->CenterRenderBlendBlack(buildImage[m_buildIndex], { position.x,position.y }, 1.5, 0, false);
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(idle, { position.x,position.y }, 1.5, 0, false);
	}
}

void CommandCenter::Release()
{
}

void CommandCenter::UIRender()
{
	IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[5].x + 25,UIPosition[5].y + 25 }, 1.7, 0, 0);
	IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0286"), { UIPosition[5].x - 1 ,UIPosition[5].y - 2 }, 1.7, 0, 0);

	if (m_buildIndex < 4)
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0236"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
	}
	else
	{
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[0].x + 25,UIPosition[0].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0000"), { UIPosition[0].x - 1 ,UIPosition[0].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[1].x + 25,UIPosition[1].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0010"), { UIPosition[1].x - 1 ,UIPosition[1].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[2].x + 25,UIPosition[2].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0001"), { UIPosition[2].x - 1 ,UIPosition[2].y - 2 }, 1.7, 0, 0);

		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("tcmdbtns0000"), { UIPosition[8].x + 25,UIPosition[8].y + 25 }, 1.7, 0, 0);
		IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("cmdicons0282"), { UIPosition[8].x - 1 ,UIPosition[8].y - 2 }, 1.7, 0, 0);
	}
}
