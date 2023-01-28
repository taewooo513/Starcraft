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
						
							if (_vf4.flag[subY * 4 + subX] == 3)
							{
								colr[indexY + indexX] = { wre.b, wre.g, 0,255 };
							}

							i++;
						}
						j++;
					}
				}
			}
		}
	}
	D2D1_RECT_U rect = { 0 , 0 ,w , h };
	tileSetData->bitmap->CopyFromMemory(&rect, colr, 4 * w);
	SAFE_DELETE(colr);
}

void MapReader::MapRender()
{
	D2D_MATRIX_3X2_F matT, matS;

	if (KEYMANAGER->GetStayKeyDown(VK_LEFT))
	{
		xoff -= 10;
	}
	else if (KEYMANAGER->GetStayKeyDown(VK_RIGHT))
	{
		xoff += 10;
	}
	if (KEYMANAGER->GetStayKeyDown(VK_UP))
	{
		yoff -= 10;
	}
	else if (KEYMANAGER->GetStayKeyDown(VK_DOWN))
	{
		yoff += 10;
	}
	matT = D2D1::Matrix3x2F::Translation(-xoff, -yoff);
	matS = D2D1::Matrix3x2F::Scale(1.7f, 1.7f);

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
