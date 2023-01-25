#include "stdafx.h"
#include "MapReader.h"

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

	fopen_s(&file, "./Resources/map/MTXM", "rb");
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

	RGBAbyte* imageData = new RGBAbyte[w * h];
	arr2D<RGBAbyte>img(imageData, w);

	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			int group = mtxm[i][j] >> 4;
			int index = mtxm[i][j] & 0xf;

			int megaTile = tileSetData->cv5->pCV5Data[group].MegaTileIndex[index];
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					int miniTileIndex = tileSetData->vx4->data[megaTile].VR4Index[y * 4 + x] >> 1;
					bool flipped = (tileSetData->vx4->data[megaTile].VR4Index[y * 4 + x] & 1) == true;

					const TileSetData::VR4::VR4Data& _vr4 = tileSetData->vr4->pVR4Data[miniTileIndex];

					const int draw_offsetX = j * 32 + x * 8;
					const int draw_offsetY = i * 32 + y * 8;

					for (int k = 0; k < 8; k++)
					{
						for (int l = 0; l < 8; l++)
						{
							int drawX = draw_offsetX + (flipped ? 7 - l : l);
							int drawY = draw_offsetY + k;

							const TileSetData::WPE::WPEData& wre = tileSetData->wpe->data[_vr4.color[(y << 3) + x]];
							RGBAbyte colr = { wre.b, wre.g,wre.r,255 };
							img[drawY][drawX] = colr;
						}
					}
				}
			}

		}
	}

	ComPtr<ID2D1ColorContext> colorContext;
	context->CreateColorContext(D2D1_COLOR_SPACE_SRGB, nullptr, 0, &colorContext);

	D2D1_BITMAP_PROPERTIES pros = D2D1::BitmapProperties();
	pros.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

	D2D1_SIZE_U s;
	s.height = h;
	s.width = w;
	const void* b;
	HRESULT hr = context->CreateBitmap(s, pros, &tileSetData->bitmap);
	D2D1_RECT_U rect = { 0,0,w,h };
	hr = tileSetData->bitmap->CopyFromMemory(&rect, img._p, img._pitch);
	int sizeff = sizeof(img._p);
	cout << hr;
}

void MapReader::MapRender()
{
	D2D_MATRIX_3X2_F matT, matS;
	if (KEYMANAGER->GetStayKeyDown(VK_LEFT))
	{
		xoff -= DELTA_TIME * 100;
	}
	if (KEYMANAGER->GetStayKeyDown(VK_RIGHT))
	{
		xoff += DELTA_TIME * 100;
	}
	matT = D2D1::Matrix3x2F::Translation(0 - xoff, 0);
	matS = D2D1::Matrix3x2F::Scale(0.1, 0.1);

	m_context->SetTransform(matT * matS);
	m_context->DrawBitmap(tileSetData->bitmap);
}
