#include "stdafx.h"
#include "MapReader.h"
#include <packon.h>

void MapReader::Init(ID2D1DeviceContext* context)
{
	tileSetData = new TileSetData;
	m_context = context;
	FILE* file;
	FILE* vx4File;
	FILE* vr4File;
	FILE* vf4File;
	FILE* wpeFile;
	FILE* cv5File;

	cout << sizeof(void*);
	fopen_s(&file, "./Resources/map/MTXM2", "rb");
	fopen_s(&wpeFile, "./Resources/map/jungle.wpe", "rb");
	fopen_s(&cv5File, "./Resources/map/jungle.cv5", "rb");
	fopen_s(&vr4File, "./Resources/map/jungle.vr4", "rb");
	fopen_s(&vx4File, "./Resources/map/jungle.vx4", "rb");
	fopen_s(&vf4File, "./Resources/map/jungle.vf4", "rb");

	if (file == NULL || cv5File == NULL || wpeFile == NULL || vr4File == NULL || vx4File == NULL)
	{
		cout << "파일을 못불러온듯?" << endl;
		return;
	}

	tileSetData->cv5 = new TileSetData::CV5;
	tileSetData->vx4 = new TileSetData::VX4;
	tileSetData->vf4 = new TileSetData::VF4;
	tileSetData->vr4 = new TileSetData::VR4;
	tileSetData->wpe = new TileSetData::WPE;

	memset(tileSetData->cv5, 0, sizeof(TileSetData::CV5));
	memset(tileSetData->vx4, 0, sizeof(TileSetData::VX4));
	memset(tileSetData->vf4, 0, sizeof(TileSetData::VF4));
	memset(tileSetData->vr4, 0, sizeof(TileSetData::VR4));
	memset(tileSetData->wpe, 0, sizeof(TileSetData::WPE));

	fread(tileSetData->cv5, 1, sizeof(TileSetData::CV5), cv5File);
	fread(tileSetData->vx4, 1, sizeof(TileSetData::VX4), vx4File);
	fread(tileSetData->vf4, 1, sizeof(TileSetData::VF4), vf4File);
	fread(tileSetData->vr4, 1, sizeof(TileSetData::VR4), vr4File);
	fread(tileSetData->wpe, 1, sizeof(TileSetData::WPE), wpeFile);

	fclose(cv5File);
	fclose(vx4File);
	fclose(vf4File);
	fclose(vr4File);
	fclose(wpeFile);
	unsigned short* mtxmdata = new unsigned short[128 * 128];
	fread(mtxmdata, 2, 128 * 128, file);

	arr2D<unsigned short>mtxm(mtxmdata, 128);

	const int w = 32 * 128;
	const int h = 32 * 128;

	ComPtr<ID2D1ColorContext> colorContext;
	context->CreateColorContext(D2D1_COLOR_SPACE_SRGB, nullptr, 0, &colorContext);

	D2D1_BITMAP_PROPERTIES pros = D2D1::BitmapProperties();
	pros.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

	D2D1_SIZE_U s;
	s.height = h;
	s.width = w;

	HRESULT hr = context->CreateBitmap(s, pros, &tileSetData->bitmap);
	RGBAbyte* colr = new RGBAbyte[w * h];

	for (int y = 0; y < 128; y++)
	{
		for (int x = 0; x < 128; x++)
		{
			int group = mtxm[y][x] >> 4;
			int index = mtxm[y][x] & 0xf;

			int megaTile = tileSetData->cv5->group[group].MegaTileIndex[index];

			for (int subY = 0; subY < 4; subY++)
			{
				for (int subX = 0; subX < 4; subX++)
				{

					int miniTileIndex = tileSetData->vx4->data[megaTile].VR4Index[subY * 4 + subX] >> 1;
					bool flipped = (tileSetData->vx4->data[megaTile].VR4Index[subY * 4 + subX] & 1) == true;
					const TileSetData::VF4::VF4Data& _vf4 = tileSetData->vf4->pVX4Data[megaTile];
					const TileSetData::VR4::VR4Data& _vr4 = tileSetData->vr4->pVR4Data[miniTileIndex];


					int offsetX = x * 32 + subX * 8;
					int offsetY = y * 32 + subY * 8;

					for (int j = 0; j < 8; )
					{
						for (int i = 0; i < 8; )
						{
							int siz = j * 8 + i;
							//cout << siz << endl;
							const TileSetData::WPE::WPEData& wre = tileSetData->wpe->data[_vr4.color[siz]];
							RGBAbyte colr2 = { wre.b, wre.g,wre.r,255 };
							int indexX = offsetX + (flipped == true ? 7 - i : i);
							int indexY = (offsetY + j) * w;

							colr[indexY + indexX] = { wre.b, wre.g,wre.r,255 };
							i++;
						}
						j++;
					}

					if (_vf4.flag[subY * 4 + subX] == 1 || _vf4.flag[subY * 4 + subX] == 3 || _vf4.flag[subY * 4 + subX] == 3 || _vf4.flag[subY * 4 + subX] == 19 || _vf4.flag[subY * 4 + subX] == 17)
					{
						miniTiles[y * 4 + subY][x * 4 + subX] = 1;
					}
					else
					{
						miniTiles[y * 4 + subY][x * 4 + subX] = 0;
					}
				}
			}
		}
	}
	MapRegionSetting();

	for (int i = 0; i < 4096; i++)
	{
		for (int j = 0; j < 4096; j++)
		{
			//int ids = miniTiles[j/8][i/8]
			int ids = region->regionsIds[i / 8][j / 8].regionsIds;
			if (ids != -1)
			{
				colr[i * 4096 + j] = { (unsigned char)(ids * 20) ,(unsigned char)(ids * 20),0,255 };
			}
		}
	}


	D2D1_RECT_U rect = { 0 , 0 ,w , h };
	tileSetData->bitmap->CopyFromMemory(&rect, colr, 4 * w);
	SAFE_DELETE(colr);
	SAFE_DELETE(mtxmdata);
}

void MapReader::MapRender(Vector2 mapPos)
{
	D2D_MATRIX_3X2_F matT, matS;

	matT = D2D1::Matrix3x2F::Translation(-IMAGEMANAGER->GetCameraPosition().x, -IMAGEMANAGER->GetCameraPosition().y);
	matS = D2D1::Matrix3x2F::Scale(1.5f, 1.5f);

	m_context->SetTransform(matS * matT);

	m_context->DrawBitmap(tileSetData->bitmap);
}

void MapReader::UIMapRender()
{
	D2D_MATRIX_3X2_F matT, matS;

	matT = D2D1::Matrix3x2F::Translation(11, 560);
	matS = D2D1::Matrix3x2F::Scale(0.0495f, 0.0495f);

	m_context->SetTransform(matS * matT);

	m_context->DrawBitmap(tileSetData->bitmap);
}

void MapReader::MapRegionSetting()
{
	int width = 128;
	int height = 128;
	int res = 32; //region scale

	queue <MapRegions*> vectorList;
	region = new Regions;
	for (int i = 0; i < 512; i++)
	{
		//regionsIds[i] = new int[4096] {-1, };
		for (int j = 0; j < 512; j++)
		{
			region->regionsIds[i][j].regionsIds = -1;
		}
	}
	bool found = false;
	int regionId = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int cx = x * res + res;
			int cy = y * res + res;

			int error = 0;
			while (error < 5)
			{
				found = false;
				for (int i = 0; i < 2 * error + 1; i++)
				{
					for (int j = 0; j < 2 * error + 1; j++)
					{
						if (cy < 4095 && cx < 4095)
							if (miniTiles[cy / 8 + 1][cx / 8 + 1] == 1)
							{
								cx += i - error;
								cy += j - error;
								found = true;
								break;
							}
					}
					if (found == true)
					{
						break;
					}
				}
				if (found == true)
				{
					break;
				}
				error += 1;
			}
			if (found == false)
			{
				continue;
			}
			if (cy != 4096)
			{
				vectorList.push(new MapRegions{ Vector2{ (float)(int)(cx / 8),(float)(int)(cy / 8) } ,regionId });
				region->regionsIds[cy / 8][cx / 8].regionsIds = regionId;
				mapRegions.push_back(new MapRegions{ Vector2{ (float)(int)(cx / 8),(float)(int)(cy / 8) } ,regionId,{(int)(cx / 8),(int)(cy / 8),(int)(cx / 8),(int)(cy / 8)} });
				regionId++;
			}
		}
	}

	int c = 0;
	while (vectorList.empty() == false)
	{
		auto iter = vectorList.front();
		int tileX = iter->pos.x;
		int tileY = iter->pos.y;
		int nowRegionIds = region->regionsIds[tileY][tileX].regionsIds;
		c++;
		if (tileX > 0)
		{
			int regionId = region->regionsIds[tileY][tileX - 1].regionsIds;

			if (miniTiles[tileY][tileX - 1] == 1)
			{
				if (regionId == -1)
				{
					region->regionsIds[tileY][tileX - 1].regionsIds = nowRegionIds;
					vectorList.push(new MapRegions{ Vector2{ (float)tileX - 1,(float)tileY },nowRegionIds });
				}
				else if (regionId != nowRegionIds)
				{
					bool isAble = false;
					bool isFind = false;

					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}

					for (auto _iter : mapRegions[nowRegionIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;

						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}
		if (tileX < 511)
		{
			if (miniTiles[tileY][tileX + 1] == 1)
			{
				int regionId = region->regionsIds[tileY][tileX + 1].regionsIds;
				if (regionId == -1)
				{
					region->regionsIds[tileY][tileX + 1].regionsIds = nowRegionIds;
					vectorList.push(new MapRegions{ Vector2{ (float)tileX + 1,(float)tileY },0 });
				}
				else if (regionId != nowRegionIds)
				{
					bool isAble = false;
					bool isFind = false;
					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}
					for (auto _iter : mapRegions[nowRegionIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId || _iter.second->regionId == nowRegionIds)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;

						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}
		if (tileY > 0)
		{
			if (miniTiles[tileY - 1][tileX] == 1)
			{
				int regionId = region->regionsIds[tileY - 1][tileX].regionsIds;
				if (regionId == -1)
				{
					region->regionsIds[tileY - 1][tileX] = region->regionsIds[tileY][tileX];
					vectorList.push(new MapRegions{ Vector2{ (float)tileX,(float)tileY - 1},0 });
				}
				else if (regionId != region->regionsIds[tileY][tileX].regionsIds)
				{
					bool isAble = false;
					bool isFind = false;
					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}
					for (auto _iter : mapRegions[region->regionsIds[tileY][tileX].regionsIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;

						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}
		if (tileY < 511)
		{
			if (miniTiles[tileY + 1][tileX] == 1)
			{
				int regionId = region->regionsIds[tileY + 1][tileX].regionsIds;
				if (regionId == -1)
				{
					region->regionsIds[tileY + 1][tileX] = region->regionsIds[tileY][tileX];
					vectorList.push(new MapRegions{ Vector2{ (float)tileX ,(float)tileY + 1},0 });
				}
				else if (regionId != nowRegionIds)
				{
					bool isFind = false;
					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}
					bool isAble = false;
					for (auto _iter : mapRegions[nowRegionIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;
						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}

		if (tileY > 0 && tileX > 0)
		{
			if (miniTiles[tileY - 1][tileX - 1] == 1)
			{
				int regionId = region->regionsIds[tileY - 1][tileX - 1].regionsIds;
				if (regionId == -1)
				{
				}
				else if (regionId != nowRegionIds)
				{
					bool isFind = false;
					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}
					bool isAble = false;
					for (auto _iter : mapRegions[region->regionsIds[tileY - 1][tileX - 1].regionsIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;

						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}

		if (tileY > 511 && tileX > 0)
		{
			if (miniTiles[tileY + 1][tileX - 1] == 1)
			{
				int regionId = region->regionsIds[tileY + 1][tileX - 1].regionsIds;
				if (regionId == -1)
				{
				}
				else if (regionId != nowRegionIds)
				{
					bool isFind = false;
					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}
					bool isAble = false;
					for (auto _iter : mapRegions[region->regionsIds[tileY + 1][tileX - 1].regionsIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;

						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}

		if (tileY > 0 && tileX < 511)
		{
			if (miniTiles[tileY - 1][tileX + 1] == 1)
			{
				int regionId = region->regionsIds[tileY - 1][tileX + 1].regionsIds;
				if (regionId == -1)
				{
				}
				else if (regionId != nowRegionIds)
				{
					bool isAble = false;
					bool isFind = false;
					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}

					for (auto _iter : mapRegions[region->regionsIds[tileY - 1][tileX + 1].regionsIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;

						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}

		if (tileY < 511 && tileX < 511)
		{
			if (miniTiles[tileY + 1][tileX + 1] == 1)
			{
				int regionId = region->regionsIds[tileY + 1][tileX + 1].regionsIds;
				if (regionId == -1)
				{
				}
				else if (regionId != nowRegionIds)
				{
					bool isFind = false;
					for (auto iter : region->regionsIds[tileY][tileX].nearRegionIds)
					{
						if (iter == regionId)
						{
							isFind = true;
							break;
						}
					}
					if (isFind == false)
					{
						region->regionsIds[tileY][tileX].nearRegionIds.push_back(regionId);
					}

					bool isAble = false;
					for (auto _iter : mapRegions[region->regionsIds[tileY + 1][tileX + 1].regionsIds]->nearRegions)
					{
						if (_iter.second->regionId == regionId)
						{
							isAble = true;
							break;
						}
					}
					if (isAble == false)
					{
						float dx = abs(mapRegions[nowRegionIds]->pos.x - mapRegions[regionId]->pos.x);
						float dy = abs(mapRegions[nowRegionIds]->pos.y - mapRegions[regionId]->pos.y);
						float e1 = abs(dx - dy);
						float e2 = min(dx, dy);
						float dest = e1 * 10 + e2 * 14;

						mapRegions[nowRegionIds]->nearRegions.push_back(make_pair(dest, mapRegions[regionId]));
						mapRegions[regionId]->nearRegions.push_back(make_pair(dest, mapRegions[nowRegionIds]));
					}
				}
			}
		}

		SAFE_DELETE(vectorList.front());
		vectorList.pop();
	}

	for (int y = 0; y < 512; y++)
	{
		for (int x = 0; x < 512; x++)
		{
			int t = region->regionsIds[y][x].regionsIds;
			if (t == -1)
			{
				continue;
			}
			mapRegions[t]->AddRegion(x, y);
		}
	}

	for (auto iter : mapRegions)
	{
		for (auto& _iter : iter->nearRegions)
		{
			float destXY = 0;
			float dx = abs(iter->pos.x - _iter.second->pos.x);
			float dy = abs(iter->pos.y - _iter.second->pos.y);
			float e1 = abs(dx - dy);
			float e2 = min(dx, dy);
			destXY = e1 * 10 + e2 * 14;
			_iter.first = destXY;
		}
	}
	/*int total_connectivity_id = 1;
	for (auto iter : mapRegions)
	{

	}*/
	/*
	* def create_region_connectivity():
	region_deque = deque()
	total_connectivity_id = 1

	   for region in Managers.game_map.all_regions:
		if region.connectivity_id == -1:
			region.connectivity_id = total_connectivity_id
			total_connectivity_id += 1
			region_deque.append(region)

		while region_deque:
			temp = region_deque.popleft()
			current_id = temp.connectivity_id

			for neighbor in temp.neighbors.keys():
				if neighbor.connectivity_id == -1:
					neighbor.connectivity_id = current_id
					region_deque.append(neighbor)
	*/
}

void MapReader::RenderLine()
{
	//for (auto iter : mapRegions)
	//{
	//	for (auto _iter : iter->nearRegions)
	//	{
	//		IMAGEMANAGER->DrawLine({ iter->pos.x * 1.5f * 8, iter->pos.y * 1.5f * 8 }, { _iter.second->pos.x * 1.5f * 8, _iter.second->pos.y * 1.5f * 8 });
	//	}
	//}
	//for (int i = 0; i < 512; i++)
	//{
	//	for (int j = 0; j < 512; j++)
	//	{
	//		for (auto iter = region->regionsIds[j][i].nearRegionIdstileNum.begin(); iter + 1 != region->regionsIds[j][i].nearRegionIdstileNum.end(); iter++)
	//		{
	//			IMAGEMANAGER->DrawLine({ (float)(*iter).x,(float)(*iter).y }, { (float)(*(iter + 1)).x,(float)(*(iter + 1)).y });
	//		}
	//	}
	//}
}

void MapReader::Release()
{
	for (auto iter : mapRegions)
	{
		SAFE_DELETE(iter);
	}
	mapRegions.clear();

	


	tileSetData->bitmap->Release();
	SAFE_DELETE(tileSetData->cv5);
	SAFE_DELETE(tileSetData->vx4);
	SAFE_DELETE(tileSetData->vf4);
	SAFE_DELETE(tileSetData->vr4);
	SAFE_DELETE(tileSetData->wpe);

	SAFE_DELETE(tileSetData);
}
