#include "stdafx.h"
#include "MapToolScene.h"

MapToolScene::MapToolScene()
{
}

MapToolScene::~MapToolScene()
{
}

void MapToolScene::Init()
{
	string player1, player2;
	player1 = FILEMANAGER->GetFileData("Player1", "Batch");
	player2 = FILEMANAGER->GetFileData("Player2", "Batch");

	MY_UTILITY::ConvertStructureString2Vec(&objdatas1, player1);
	MY_UTILITY::ConvertStructureString2Vec(&objdatas2, player2);

}

void MapToolScene::Update()
{
	if (KEYMANAGER->GetOnceKeyDown(VK_ESCAPE))
	{
		string player1;
		MY_UTILITY::ConvertVec2StructureString(objdatas1, &player1);
		FILEMANAGER->WriteFileData("Player1", "Batch", player1);

		string player2;
		MY_UTILITY::ConvertVec2StructureString(objdatas2, &player2);
		FILEMANAGER->WriteFileData("Player2", "Batch", player2);
	}

	if (KEYMANAGER->GetOnceKeyDown(VK_TAB))
	{
		pageTag = !pageTag;
	}

	if (KEYMANAGER->GetStayKeyDown(VK_LEFT))
	{
		if (IMAGEMANAGER->GetCameraPosition().x > -250)
		{
			IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x - DELTA_TIME * 800, IMAGEMANAGER->GetCameraPosition().y });
		}
	}
	if (KEYMANAGER->GetStayKeyDown(VK_RIGHT))
	{
		if (IMAGEMANAGER->GetCameraPosition().x < 128 * 32 * 1.5f - WINSIZE_X)
		{
			IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x + DELTA_TIME * 800, IMAGEMANAGER->GetCameraPosition().y });
		}
	}
	if (KEYMANAGER->GetStayKeyDown(VK_DOWN))
	{
		if (IMAGEMANAGER->GetCameraPosition().y < 128 * 32 * 1.5f - WINSIZE_Y)
		{
			IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x , IMAGEMANAGER->GetCameraPosition().y + DELTA_TIME * 800 });
		}
	}
	if (KEYMANAGER->GetStayKeyDown(VK_UP))
	{
		if (IMAGEMANAGER->GetCameraPosition().y > 5)
		{
			IMAGEMANAGER->CameraSetPosition({ IMAGEMANAGER->GetCameraPosition().x , IMAGEMANAGER->GetCameraPosition().y - DELTA_TIME * 800 });
		}
	}
	if (KEYMANAGER->GetOnceKeyDown(VK_LBUTTON) && _ptMouse.x > 250 && objTag != -1)
	{
		if (team == 0)
		{
			ObjectData* objd = new ObjectData;
			objd->key = objTag;
			objd->x = _ptMouse.x + IMAGEMANAGER->GetCameraPosition().x;
			objd->y = _ptMouse.y + IMAGEMANAGER->GetCameraPosition().y;
			objdatas1.push_back(objd);
		}
		if (team == 1)
		{
			ObjectData* objd = new ObjectData;
			objd->key = objTag;
			objd->x = _ptMouse.x + IMAGEMANAGER->GetCameraPosition().x;
			objd->y = _ptMouse.y + IMAGEMANAGER->GetCameraPosition().y;
			objdatas2.push_back(objd);
		}
	}
	if (KEYMANAGER->GetOnceKeyDown(VK_F1))
	{
		team = !team;
	}
}

void MapToolScene::Render()
{
	IMAGEMANAGER->MapRender();

	if (team == 0)
	{

		for (auto iter : objdatas1)
		{
			switch (iter->key)
			{
			case 0:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 1:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("control0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 2:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 3:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("depot0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 4:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("weaponpl0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 5:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("factory0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 6:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("marin_idle_1_9"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 7:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("scv_idle_9"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 8:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("Vulture_9"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 9:
				//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
				//IMAGEMANAGER->DirectDrawText(L"탱크", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 10:
				//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
				//IMAGEMANAGER->DirectDrawText(L"가스", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 11:
				//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
				//IMAGEMANAGER->DirectDrawText(L"고스트", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			}
		}
	}
	else
	{
		for (auto iter : objdatas2)
		{
			switch (iter->key)
			{
			case 0:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 1:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("control0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 2:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 3:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("depot0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 4:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("weaponpl0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 5:
				IMAGEMANAGER->RenderBlendBlack(IMAGEMANAGER->FindImage("factory0000"), { iter->x,iter->y }, 1.5, 0);
				break;
			case 6:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("marin_idle_1_9"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 7:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("scv_idle_9"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 8:
				IMAGEMANAGER->CenterRenderBlendBlack(IMAGEMANAGER->FindImage("Vulture_9"), { iter->x,iter->y }, 1.5, 0, 0);
				break;
			case 9:
				//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
				//IMAGEMANAGER->DirectDrawText(L"탱크", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 10:
				//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
				//IMAGEMANAGER->DirectDrawText(L"가스", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 11:
				//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
				//IMAGEMANAGER->DirectDrawText(L"고스트", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			}
		}
	}
}

void MapToolScene::Release()
{
}

void MapToolScene::UIRender()
{
	if (pageTag > 0)
	{
		IMAGEMANAGER->DrawRect({ 0.f,0.f }, { 250,WINSIZE_Y }, { 1,1,1,1 }, true);

		for (float i = 0; i < 12; i++)
		{
			RECT rt = {
				float((int)i % 2) * 100 + 50,
				200.f + int((int)i / 2) * 100,
				float((int)i % 2) * 100 + 50 + 50,
				200.f + int((int)i / 2) * 100 + 50 };

			IMAGEMANAGER->DrawRect({ (float)rt.left,(float)rt.top }, { (float)rt.right,(float)rt.bottom });

			if (KEYMANAGER->GetStayKeyDown(VK_LBUTTON))
			{
				if (PtInRect(&rt, _ptMouse))
				{
					objTag = i;
				}
			}

			switch ((int)i)
			{
			case 0:
				IMAGEMANAGER->DirectDrawText(L"미네랄", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 1:
				IMAGEMANAGER->DirectDrawText(L"커멘드센터", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 2:
				IMAGEMANAGER->DirectDrawText(L"베럭", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 3:
				IMAGEMANAGER->DirectDrawText(L"디폿", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 4:
				IMAGEMANAGER->DirectDrawText(L"엔지니어링", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 5:
				IMAGEMANAGER->DirectDrawText(L"펙토리", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 6:
				IMAGEMANAGER->DirectDrawText(L"마린", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 7:
				IMAGEMANAGER->DirectDrawText(L"SCV", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 8:
				IMAGEMANAGER->DirectDrawText(L"벌처", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 9:
				IMAGEMANAGER->DirectDrawText(L"탱크", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 10:
				IMAGEMANAGER->DirectDrawText(L"가스", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			case 11:
				IMAGEMANAGER->DirectDrawText(L"고스트", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
				break;
			}
		}
	}

	if (team == 0)
	{
		RECT rt = { 50 , 50, 150 , 150 };

		IMAGEMANAGER->DrawRect({ (float)rt.left,(float)rt.top }, { (float)rt.right,(float)rt.bottom }, { 0,255,0,1 }, true);
	}
	else
	{
		RECT rt = { 50 , 50, 150 , 150 };

		IMAGEMANAGER->DrawRect({ (float)rt.left,(float)rt.top }, { (float)rt.right,(float)rt.bottom }, { 255,0,0,1 }, true);
	}

	switch (objTag)
	{
	case 0:
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
		break;
	case 1:
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("control0000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0);
		break;
	case 2:
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("tbarrack0000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0);
		break;
	case 3:
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("depot0000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0);
		break;
	case 4:
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("weaponpl0000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0);
		break;
	case 5:
		IMAGEMANAGER->UIRenderBlendBlack(IMAGEMANAGER->FindImage("factory0000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0);
		break;
	case 6:
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("marin_idle_1_9"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
		break;
	case 7:
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("scv_idle_9"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
		break;
	case 8:
		IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("Vulture_9"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
		break;
	case 9:
		//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
		//IMAGEMANAGER->DirectDrawText(L"탱크", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
		break;
	case 10:
		//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
		//IMAGEMANAGER->DirectDrawText(L"가스", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
		break;
	case 11:
		//IMAGEMANAGER->UICenterRenderBlendBlack(IMAGEMANAGER->FindImage("min010000"), { (float)_ptMouse.x,(float)_ptMouse.y }, 1.5, 0, 0);
		//IMAGEMANAGER->DirectDrawText(L"고스트", { float((int)i % 2) * 100 + 50 ,200.f + int((int)i / 2) * 100 }, { 10,10 }, { 0,0,0,1 });
		break;
	}

}
