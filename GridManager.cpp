#include "stdafx.h"
#include "GridManager.h"
#include "MapReader.h"

GridManager::~GridManager()
{
	SAFE_DELETE(IMAGEMANAGER->GetMapReader()->region);

}

ObjectGrid* GridManager::AddGrid(Object* _obj, float sizeX, float sizeY, float searchX, float searchY, float x, float y)
{
	ObjectGrid* grid = new ObjectGrid;
	grid->Init(_obj, { sizeX, sizeY }, { searchX, searchY }, x, y);


	objectGrid.push_back(grid);
	return grid;
}

void GridManager::Init()
{
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			regionsTile[j][i].regionsIds = IMAGEMANAGER->GetMapReader()->region->regionsIds[i][j].regionsIds;
			regionsTile[j][i].nearRegionIds = IMAGEMANAGER->GetMapReader()->region->regionsIds[i][j].nearRegionIds;
			for (auto iter : IMAGEMANAGER->GetMapReader()->region->regionsIds[i][j].nearRegionIds)
			{
				auto find = IMAGEMANAGER->GetMapReader()->mapRegions[regionsTile[j][i].regionsIds]->nearRegionPosition.find(iter);
				if (find == IMAGEMANAGER->GetMapReader()->mapRegions[regionsTile[j][i].regionsIds]->nearRegionPosition.end())
				{
					vector<Vector2> positions;
					positions.push_back(Vector2{ (float)j,(float)i });
					IMAGEMANAGER->GetMapReader()->mapRegions[regionsTile[j][i].regionsIds]->nearRegionPosition.insert(make_pair(iter, positions));
				}
				else
				{
					find->second.push_back(Vector2{ (float)j,(float)i });
				}
			}
			if (regionsTile[j][i].regionsIds != -1)
			{
				regionsTile[j][i].isBuildAble = true;
				regionsTile[j][i].x = j;
				regionsTile[j][i].y = i;
			}
		}
	}
	cout << "d";
}

void GridManager::Update()
{
	for (auto iter : objectGrid)
	{
		iter->Update();
	}
}

void GridManager::Render()
{
	//for (int j = 0; j < 512; j++)
	//{
	//	for (int i = 0; i < 512; i++)
	//	{
	//		if (float(j) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x <= WINSIZE_X && float(j) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x >= 0)
	//		{
	//			if (float(i) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y <= WINSIZE_Y && float(i) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y >= 0)
	//			{
	//				if (regionsTile[j][i].isBuildAble != false)
	//				{
	//					//IMAGEMANAGER->DrawRect({ float(j) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x,float(i) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y }, { float(j) * 8.f * 1.5f + float(j) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x,float(i) * 8.f * 1.5f + float(i) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y });
	//				}
	//				else
	//				{
	//					IMAGEMANAGER->DrawRectRed({ float(j) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x,float(i) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y }, { float(j) * 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x,float(i) * 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y });
	//				}
	//			}
	//		}
	//	}
	//}

	//for (auto iter : IMAGEMANAGER->GetMapReader()->mapRegions)
	//{
	//	for (auto _iter : iter->nearRegionPosition)
	//	{
	//		for (auto __iter : _iter.second)
	//		{
	//			IMAGEMANAGER->DrawRectRed({ float(__iter.x) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().x,float(__iter.y) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y }, { float(__iter.x)* 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition//().x,float(__iter.y) * 8.f * 1.5f + float(1) * 8.f * 1.5f - IMAGEMANAGER->GetCameraPosition().y });
	//		}
	//	}
	//}
	//
	//for (auto iter : objectGrid)
	//{
	//	iter->Render();
	//}
}
