#include "stdafx.h"
#include "ImageManager.h"
#include "MapReader.h"
#include <numeric>
#include <d3dx9math.h>
ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
	mapReader->Release();
	SAFE_DELETE(mapReader);

	for (auto iter : m_vImageList)
	{
		for (auto _iter : iter.second->GetImages())
		{
			SAFE_DELETE(_iter);
		}
		SAFE_DELETE(iter.second);
	}
	m_vImageList.clear();

	for (auto iter : m_imageList)
	{
		SAFE_DELETE(iter.second);
	}
	m_imageList.clear();
}

void ImageManager::Test(RECT rc)
{
	ComPtr<ID2D1SolidColorBrush> pBlackBrush;
	m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&pBlackBrush
	);

	D2D1_RECT_F rt2 = D2D1::RectF(
		rc.left,
		rc.top,
		rc.right,
		rc.bottom);
	m_d2dContext->DrawRectangle(rt2, pBlackBrush.Get(), 1, 0);

}

void ImageManager::Init(ID2D1DeviceContext* context, IDXGISwapChain1* swapChain)
{
	CoInitialize(0);
	CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));

	m_d2dContext = context;
	m_swapChain = swapChain;
	context->CreateEffect(CLSID_D2D1ChromaKey, &blendEffect);

	ComPtr<ID2D1ColorContext> displayColorContext;

	context->CreateColorContext(D2D1_COLOR_SPACE_SRGB, nullptr, 0, &displayColorContext);
	blendEffect->SetValue(
		D2D1_COLORMANAGEMENT_PROP_DESTINATION_COLOR_CONTEXT,
		displayColorContext.Get()
	);
	blendEffect->SetValue(D2D1_CHROMAKEY_PROP_COLOR, D2D1::ColorF{ D2D1::ColorF::Black });
	blendEffect->SetValue(D2D1_CHROMAKEY_PROP_TOLERANCE, 0.01f);
	blendEffect->SetValue(D2D1_CHROMAKEY_PROP_INVERT_ALPHA, false);
	blendEffect->SetValue(D2D1_CHROMAKEY_PROP_FEATHER, false);
	blendEffect->SetValue(D2D1_BLEND_PROP_MODE, D2D1_BLEND_MODE_EXCLUSION);


	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
	);

	IDWriteFontFile* fontFileReference;

	m_pDWriteFactory->CreateFontFileReference(L"./Resources/StarCraft.ttf", nullptr, &fontFileReference);

	IDWriteFontSetBuilder1* fontSetBuilder;
	m_pDWriteFactory->CreateFontSetBuilder(reinterpret_cast<IDWriteFontSetBuilder**>(&fontSetBuilder));

	fontSetBuilder->AddFontFile(fontFileReference);

	IDWriteFontSet* customFontSet;

	fontSetBuilder->CreateFontSet(&customFontSet);
	IDWriteFontCollection1* col;
	m_pDWriteFactory->CreateFontCollectionFromFontSet(
		customFontSet
		, &col
	);
	int c = col->GetFontFamilyCount();


	m_pDWriteFactory->CreateTextFormat(
		L"Txt",                  // ��Ʈ �йи� �̸��� ���ڿ�
		NULL,                        // ��Ʈ �÷��� ��ü, NULL=�ý��� ��Ʈ �÷���
		DWRITE_FONT_WEIGHT_NORMAL,   // ��Ʈ ����. LIGHT, NORMAL, BOLD ��.
		DWRITE_FONT_STYLE_NORMAL,    // ��Ʈ ��Ÿ��. NORMAL, OBLIQUE, ITALIC.
		DWRITE_FONT_STRETCH_NORMAL,  // ��Ʈ ����. CONDENSED, NORMAL, MEDIUM, EXTEXDED ��.
		1,                          // ��Ʈ ũ��.
		L"Txt",                         // �������� ���ڿ��� ���.  ����-�̱�=L"en-us", �ѱ���-�ѱ�=L"ko-kr"
		&m_pTextFormat
	);

	D2D1_COLOR_F color;
	color.a = 1;
	color.r = 1;
	color.g = 1;
	color.b = 1;

	IDWriteFontFamily* fontFamily;
	IDWriteLocalizedStrings* localizedFontName;
	//IDWriteFontFamilyName* localizedFontName;

	WCHAR* c_styleFontName = new WCHAR[70];
	col->GetFontFamily(0, &fontFamily);
	fontFamily->GetFamilyNames(&localizedFontName);
	localizedFontName->GetString(1, c_styleFontName, 65);

	m_pDWriteFactory->Release();

	RECT rect;
	GetClientRect(_hWnd, &rect);

	ImageLoad();
	LoadMap();
	elipse = new D2D1_ELLIPSE;
	UINT32  length;

	localizedFontName->GetStringLength(0, &length);
	wchar_t* name = new (std::nothrow) wchar_t[length + 1];
	if (name == NULL)
	{
	}

	// Get the family name.

	localizedFontName->GetString(0, name, length + 1);
	context->CreateSolidColorBrush({}, &brush);
}

void ImageManager::CameraSetting()
{
}

void ImageManager::Render(CImage* img, Vector2 vec, float scale, float rot)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matR = D2D1::Matrix3x2F::Rotation(rot, { 0,0 });
	matS = D2D1::Matrix3x2F::Scale(scale, scale);
	matP = D2D1::Matrix3x2F::Translation(vec.x, vec.y);
	matW = matS * matR * matP;

	m_d2dContext->SetTransform(matW);
	m_d2dContext->DrawBitmap(img->GetBitMap());
}

void ImageManager::DirectDrawText(wstring str, Vector2 pos, Vector2 Size, D2D1_COLOR_F color)
{
	D2D1::Matrix3x2F mat, matS;
	mat = D2D1::Matrix3x2F::Translation(pos.x, pos.y);
	matS = D2D1::Matrix3x2F::Scale(Size.x, Size.y);

	m_d2dContext->SetTransform(matS * mat);
	brush->SetColor(color);
	m_d2dContext->DrawTextA(str.c_str(), str.size(), m_pTextFormat, D2D1::RectF(0, 0, WINSIZE_X, WINSIZE_Y), brush);
}

void ImageManager::RenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot)
{
	UIRenderBlendBlack(img, { vec.x - GetCameraPosition().x, vec.y - GetCameraPosition().y }, scale, rot);
}

void ImageManager::RenderBlendBlack2(CImage* img, Vector2 vec, float scale, float rot, float alpha)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matR = D2D1::Matrix3x2F::Rotation(rot, { 0,0 });
	matS = D2D1::Matrix3x2F::Scale(scale, scale);
	matP = D2D1::Matrix3x2F::Translation(vec.x - GetCameraPosition().x, vec.y - GetCameraPosition().y);
	matW = matS * matR * matP;

	blendEffect->SetInput(0, img->GetBitMap());

	m_d2dContext->SetTransform(matW);
	HRESULT hr = S_OK;

	// Create a layer.
	ID2D1Layer* pLayer = NULL;
	m_d2dContext->CreateLayer(NULL, &pLayer);

	if (SUCCEEDED(hr))
	{
		m_d2dContext->SetTransform(matW);
		ComPtr<ID2D1SolidColorBrush> b;
		m_d2dContext->CreateSolidColorBrush({ 0,0,0,0.5f }, &b);

		m_d2dContext->PushLayer(
			D2D1::LayerParameters(
				D2D1::InfiniteRect(),
				NULL,
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
				D2D1::IdentityMatrix(),
				alpha),
			pLayer
		);
		m_d2dContext->DrawImage(blendEffect);

		m_d2dContext->PopLayer();
	}

	pLayer->Release();




	//m_d2dContext->DrawImage(blendEffect);
}


void ImageManager::UIRenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matR = D2D1::Matrix3x2F::Rotation(rot, { 0,0 });
	matS = D2D1::Matrix3x2F::Scale(scale, scale);
	matP = D2D1::Matrix3x2F::Translation(vec.x, vec.y);
	matW = matS * matR * matP;

	//Color_;CLSID_D2D1AlphaMask

	blendEffect->SetInput(0, img->GetBitMap());

	m_d2dContext->SetTransform(matW);

	m_d2dContext->DrawImage(blendEffect);
}

void ImageManager::CenterRenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot, bool isReverse)
{
	UICenterRenderBlendBlack(img, { vec.x - GetCameraPosition().x, vec.y - GetCameraPosition().y }, scale, rot, isReverse);
}

void ImageManager::UICenterRenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot, bool isReverse)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matR = D2D1::Matrix3x2F::Rotation(rot, { 0,0 });
	if (isReverse == false)
	{
		matS = D2D1::Matrix3x2F::Scale(scale, scale);
		matP = D2D1::Matrix3x2F::Translation(vec.x - img->GetWidth() * scale / 2, vec.y - img->GetHeight() * scale / 2);
	}
	else
	{
		matS = D2D1::Matrix3x2F::Scale(-scale, scale);
		matP = D2D1::Matrix3x2F::Translation(vec.x + img->GetWidth() * scale / 2, vec.y - img->GetHeight() * scale / 2);
	}

	matW = matS * matP * matR;

	//Color_;CLSID_D2D1AlphaMask
	blendEffect->SetInput(0, img->GetBitMap());

	m_d2dContext->SetTransform(matW);
	m_d2dContext->DrawImage(blendEffect);
}

void ImageManager::UICenterRenderBlendBlack2(CImage* img, Vector2 vec, Vector2 scale, float rot, bool isReverse, float alpha)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matR = D2D1::Matrix3x2F::Rotation(rot, { 0,0 });
	if (isReverse == false)
	{
		matS = D2D1::Matrix3x2F::Scale(scale.x, scale.y);
		matP = D2D1::Matrix3x2F::Translation(vec.x - img->GetWidth() * scale.x / 2, vec.y - img->GetHeight() * scale.y / 2);
	}
	else
	{
		matS = D2D1::Matrix3x2F::Scale(-scale.x, scale.y);
		matP = D2D1::Matrix3x2F::Translation(vec.x + img->GetWidth() * scale.x / 2, vec.y - img->GetHeight() * scale.y / 2);
	}

	matW = matS * matP * matR;

	//Color_;CLSID_D2D1AlphaMask
	blendEffect->SetInput(0, img->GetBitMap());

	m_d2dContext->SetTransform(matW);
	m_d2dContext->DrawImage(blendEffect);
}


void ImageManager::MapRender()
{
	mapReader->MapRender(GetCameraPosition());
}

void ImageManager::UIMapRender()
{
	mapReader->UIMapRender();
}

void ImageManager::DrawCircle(Vector2 vec, float scaleX, float scaleY, D2D1_COLOR_F color)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matS = D2D1::Matrix3x2F::Scale(scaleX, scaleY);
	matP = D2D1::Matrix3x2F::Translation(vec.x - GetCameraPosition().x, vec.y - GetCameraPosition().y);
	matW = matS * matP;

	m_d2dContext->SetTransform(matW);

	ComPtr<ID2D1SolidColorBrush> brush;
	elipse->point = { 0, 0 };
	elipse->radiusX = 2;
	elipse->radiusY = 2;

	m_d2dContext->CreateSolidColorBrush({ 0,255,0,255 }, &brush);
	m_d2dContext->DrawEllipse(elipse, brush.Get(), 0.05f);
}

void ImageManager::DrawRect(Vector2 startPos, Vector2 endPos, D2D1_COLOR_F color, int rectStyle)
{

	D2D1_MATRIX_3X2_F matW, matR, matS, matP;
	matP = D2D1::Matrix3x2F::Translation(0, 0);
	matW = matP;

	m_d2dContext->SetTransform(matW);
	//m_d2dContext->CreateSolidColorBrush(color, &brush);
	brush->SetColor(color);
	if (rectStyle == 1)
		m_d2dContext->FillRectangle({ startPos.x,startPos.y ,endPos.x ,endPos.y }, brush);
	else
	{
		m_d2dContext->DrawRectangle({ startPos.x,startPos.y ,endPos.x ,endPos.y }, brush, 1.5f);
	}
}

void ImageManager::DrawRectRed(Vector2 startPos, Vector2 endPos)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matP = D2D1::Matrix3x2F::Translation(0, 0);
	matW = matP;

	m_d2dContext->SetTransform(matW);

	ComPtr<ID2D1SolidColorBrush> brush;
	m_d2dContext->CreateSolidColorBrush({ 255,0,0,0.5f }, &brush);
	m_d2dContext->FillRectangle({ startPos.x,startPos.y ,endPos.x ,endPos.y }, brush.Get());
}

void ImageManager::DrawLine(Vector2 startPos, Vector2 endPos)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matP = D2D1::Matrix3x2F::Translation(-GetCameraPosition().x, -GetCameraPosition().y);
	matW = matP;

	m_d2dContext->SetTransform(matW);

	ComPtr<ID2D1SolidColorBrush> brush;
	m_d2dContext->CreateSolidColorBrush({ 0,255,0,255 }, &brush);

	HDC dc;

	m_d2dContext->DrawLine({ startPos.x,startPos.y }, { endPos.x,endPos.y }, brush.Get());
}

void ImageManager::DrawRegion()
{
	mapReader->RenderLine();
}

void ImageManager::LoadMap()
{
	mapReader = new MapReader;
	mapReader->Init(m_d2dContext);
}

void ImageManager::AddWireImage(std::string key)
{
	D2D1_SIZE_U s;
	s.height = 64;
	s.width = 64;
	ComPtr<ID2D1ColorContext> colorContext;
	m_d2dContext->CreateColorContext(D2D1_COLOR_SPACE_SRGB, nullptr, 0, &colorContext);

	D2D1_BITMAP_PROPERTIES pros = D2D1::BitmapProperties();
	pros.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

	RGBAbyte*** rgb = new RGBAbyte * *[4];
	for (int i = 0; i < 4; i++)
	{
		rgb[i] = new RGBAbyte * [4];
		for (int i2 = 0; i2 < 4; i2++)
		{
			rgb[i][i2] = new RGBAbyte[64 * 64];
		}
	}

	auto find = wires.find(key);
	if (find == wires.end())
	{
		return;
	}
	auto wire = find->second;
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			auto color = GetPixel(wire->dc, i, j);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);

			for (int k = 0; k < 4; k++)
			{
				if (r == 252 && g == 252 && b == 56 && k == 0)
				{
					rgb[0][0][i + j * 64].r = 24;
					rgb[0][0][i + j * 64].g = 252;
					rgb[0][0][i + j * 64].b = 16;
					rgb[0][0][i + j * 64].padding = 255;

					rgb[0][1][i + j * 64].r = 56;
					rgb[0][1][i + j * 64].g = 252;
					rgb[0][1][i + j * 64].b = 252;
					rgb[0][1][i + j * 64].padding = 255;

					rgb[0][2][i + j * 64].r = 20;
					rgb[0][2][i + j * 64].g = 140;
					rgb[0][2][i + j * 64].b = 248;
					rgb[0][2][i + j * 64].padding = 255;

					rgb[0][3][i + j * 64].r = 24;
					rgb[0][3][i + j * 64].g = 24;
					rgb[0][3][i + j * 64].b = 200;
					rgb[0][3][i + j * 64].padding = 255;

				}
				else if (r == 248 && g == 140 && b == 20 && k == 1)
				{
					rgb[1][0][i + j * 64].r = 24;
					rgb[1][0][i + j * 64].g = 252;
					rgb[1][0][i + j * 64].b = 16;
					rgb[1][0][i + j * 64].padding = 255;


					rgb[1][1][i + j * 64].r = 56;
					rgb[1][1][i + j * 64].g = 252;
					rgb[1][1][i + j * 64].b = 252;
					rgb[1][1][i + j * 64].padding = 255;

					rgb[1][2][i + j * 64].r = 20;
					rgb[1][2][i + j * 64].g = 140;
					rgb[1][2][i + j * 64].b = 248;
					rgb[1][2][i + j * 64].padding = 255;

					rgb[1][3][i + j * 64].r = 24;
					rgb[1][3][i + j * 64].g = 24;
					rgb[1][3][i + j * 64].b = 200;
					rgb[1][3][i + j * 64].padding = 255;

				}
				else if (r == 200 && g == 24 && b == 24 && k == 2)
				{
					rgb[2][0][i + j * 64].r = 24;
					rgb[2][0][i + j * 64].g = 252;
					rgb[2][0][i + j * 64].b = 16;
					rgb[2][0][i + j * 64].padding = 255;

					rgb[2][1][i + j * 64].r = 56;
					rgb[2][1][i + j * 64].g = 252;
					rgb[2][1][i + j * 64].b = 252;
					rgb[2][1][i + j * 64].padding = 255;

					rgb[2][2][i + j * 64].r = 20;
					rgb[2][2][i + j * 64].g = 140;
					rgb[2][2][i + j * 64].b = 248;
					rgb[2][2][i + j * 64].padding = 255;

					rgb[2][3][i + j * 64].r = 24;
					rgb[2][3][i + j * 64].g = 24;
					rgb[2][3][i + j * 64].b = 200;
					rgb[2][3][i + j * 64].padding = 255;

				}
				else if (r == 16 && g == 252 && b == 24 && k == 3)
				{
					rgb[3][0][i + j * 64].r = 24;
					rgb[3][0][i + j * 64].g = 252;
					rgb[3][0][i + j * 64].b = 16;
					rgb[3][0][i + j * 64].padding = 255;

					rgb[3][1][i + j * 64].r = 56;
					rgb[3][1][i + j * 64].g = 252;
					rgb[3][1][i + j * 64].b = 252;
					rgb[3][1][i + j * 64].padding = 255;

					rgb[3][2][i + j * 64].r = 20;
					rgb[3][2][i + j * 64].g = 140;
					rgb[3][2][i + j * 64].b = 248;
					rgb[3][2][i + j * 64].padding = 255;

					rgb[3][3][i + j * 64].r = 24;
					rgb[3][3][i + j * 64].g = 24;
					rgb[3][3][i + j * 64].b = 200;
					rgb[3][3][i + j * 64].padding = 255;
				}
				else
				{
					for (int v = 0; v < 4; v++)
					{
						rgb[k][v][i + j * 64].r = 0;
						rgb[k][v][i + j * 64].g = 0;
						rgb[k][v][i + j * 64].b = 0;
						rgb[k][v][i + j * 64].padding = 255;
					}
				}
			}
		}
	}
	//DC ����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			RGBAbyte r[64 * 64];
			for (int i2 = 0; i2 < 64; i2++)
			{
				for (int j2 = 0; j2 < 64; j2++)
				{
					r[i2 * 64 + j2] = rgb[i][j][i2 * 64 + j2];
				}
			}
			CImage* img = new CImage;
			HRESULT hr = m_d2dContext->CreateBitmap(s, pros, &img->bitmap);
			D2D1_RECT_U rectc = { 0 , 0 ,64 , 64 };
			HRESULT hr2 = img->bitmap->CopyFromMemory(&rectc, r, 4 * 64);
			img->SetWidth(64);
			img->SetHeight(64);
			wire->wireImages[i].push_back(img);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SAFE_DELETE_ARRAY(rgb[i][j]);
		}
		SAFE_DELETE_ARRAY(rgb[i]);
	}
	SAFE_DELETE_ARRAY(rgb);
}

void ImageManager::FogRender()
{
	for (int j = IMAGEMANAGER->GetCameraPosition().x / 8 / 1.5f; j < (IMAGEMANAGER->GetCameraPosition().x + WINSIZE_X) / 8 / 1.5f; j++)
	{
		for (int i = IMAGEMANAGER->GetCameraPosition().y / 8 / 1.5f; i < (IMAGEMANAGER->GetCameraPosition().y + WINSIZE_Y) / 8 / 1.5f; i++)
		{
			if (i >= 0 && j >= 0 && j < 511 && i < 511)
					DrawRect({
					(float)j * 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().x ,
					(float)i * 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().y }, {
					(float)j * 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().x + 8 * 1.52f ,
					(float)i * 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().y + 8 * 1.52f },
						{ 0,0,0,1.f - GRIDMANAGER->regionsTile[j][i].fogTag }, 1);
		}
	}

}

void ImageManager::FogUpdate(Vector2 pos, float dest)
{
	Vector2 i = pos / 1.5 / 8;
	vector<pair<pair<float, Vector2>, GridManager::tileNum*>> obstacle;
	list<GridManager::tileNum*> tiless;

	float rotA = atan2(i.y - (i.y + dest), i.x - i.x);
	float rotB = atan2(i.y - (i.y + dest), i.x - (i.x + 1));
	float rot = abs(rotB - rotA);

	float r = 0;
	while (r < 3.141592 * 2.f)
	{
		r += rot / 2.f;
		for (int j = 0; j < dest; j++)
		{
			float x = round(cos(r) * j + i.x);
			float y = round(sin(r) * j + i.y);

			if (x > 0 && x < 511 && y > 0 && y < 511)
			{
				if (mapReader->miniTiles2[(int)i.y][(int)i.x] != 1)
				{
					if (mapReader->miniTiles2[(int)y][(int)x] == 1)
						break;
				}
				GRIDMANAGER->regionsTile[(int)x][(int)y].fogTag = 1;
			}
			else
			{
				break;
			}
		}
	}
}

void ImageManager::DrawUI2(CImage* img, Vector2 vec, float scale, float rot, bool isReverse)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matR = D2D1::Matrix3x2F::Rotation(rot, { 0,0 });
	if (isReverse == false)
	{
		matS = D2D1::Matrix3x2F::Scale(scale, scale);
		matP = D2D1::Matrix3x2F::Translation(vec.x - img->GetWidth() * scale / 2, vec.y - img->GetHeight() * scale / 2);
	}
	else
	{
		matS = D2D1::Matrix3x2F::Scale(-scale, scale);
		matP = D2D1::Matrix3x2F::Translation(vec.x + img->GetWidth() * scale / 2, vec.y - img->GetHeight() * scale / 2);
	}

	matW = matS * matP * matR;
	ComPtr<ID2D1Effect> saturationEffect;

	m_d2dContext->CreateEffect(CLSID_D2D1Saturation, &saturationEffect);
	//Color_;CLSID_D2D1AlphaMask
	blendEffect->SetInput(0, img->GetBitMap());

	m_d2dContext->SetTransform(matW);
	saturationEffect->SetInput(0, img->GetBitMap());
	saturationEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 0.0f);
	saturationEffect->SetInputEffect(0, blendEffect);

	m_d2dContext->DrawImage(saturationEffect.Get());
}

void ImageManager::AddWireImage(std::string key, std::string path)
{
	UINT _width = 0, _height = 0;


	WireFrame* wire = new WireFrame;

	D2D1_SIZE_U s;
	s.height = 64;
	s.width = 64;

	HDC hdc = GetDC(_hWnd);
	wire->defaultImage = (HBITMAP)LoadImage(hInst, path.c_str(), IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);

	vector<CImage*> vec1;
	wire->wireImages.insert(make_pair(0, vec1));

	vector<CImage*> vec2;
	wire->wireImages.insert(make_pair(1, vec2));

	vector<CImage*> vec3;
	wire->wireImages.insert(make_pair(2, vec3));

	vector<CImage*> vec4;
	wire->wireImages.insert(make_pair(3, vec4));

	HDC dc;
	dc = CreateCompatibleDC(hdc);
	(HBITMAP)SelectObject(dc, wire->defaultImage);
	HBITMAP hOBit = (HBITMAP)SelectObject(dc, wire->defaultImage);
	wire->dc = dc;

	wires.insert(make_pair(key, wire));

}

ID2D1Bitmap* ImageManager::AddBitmap(std::wstring path, UINT* Width, UINT* Height)
{
	ID2D1Bitmap* bitmap;
	IWICBitmapDecoder* decoder = nullptr;
	factory->CreateDecoderFromFilename(path.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

	IWICBitmapFrameDecode* frameDecode = nullptr;
	decoder->GetFrame(0, &frameDecode);

	IWICFormatConverter* converter = nullptr;
	factory->CreateFormatConverter(&converter);
	converter->Initialize(frameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0.0, WICBitmapPaletteTypeCustom);
	m_d2dContext->CreateBitmapFromWicBitmap(converter, 0, &bitmap);
	converter->GetSize(Width, Height);
	decoder->Release();
	frameDecode->Release();
	converter->Release();

	return bitmap;
}

CImage* ImageManager::AddImage(const std::string key, std::wstring path)
{
	UINT _width = 0, _height = 0;
	CImage* img = new CImage(AddBitmap(path, &_width, &_height), _width, _height);
	img->SetWidth(_width);
	img->SetHeight(_height);

	m_imageList.insert(std::make_pair(key, img));
	return img;
}

CImage* ImageManager::FindImage(const std::string key)
{
	auto find = m_imageList.find(key);
	if (find != m_imageList.end())
	{
		return find->second;
	}
	cout << "�̹��� key ���� : " << key << "\n";
	return nullptr;
}

vImage* ImageManager::AddImageVector(const std::string key, std::wstring path, int startIndex, int endIndex, bool isPng)
{
	auto find = m_vImageList.find(key);

	if (find == m_vImageList.end())
	{
		vImage* imgs = new vImage();
		for (int i = startIndex; i <= endIndex; i++)
		{
			UINT _width = 0, _height = 0;
			std::wstring _path;
			if (i < 10)
			{
				if (isPng == true)
				{
					_path = path + L"0" + std::to_wstring(i).c_str() + L".png";
				}
				else
				{
					_path = path + L"0" + std::to_wstring(i).c_str() + L".bmp";
				}
			}
			else
			{
				if (isPng == true)
				{
					_path = path + std::to_wstring(i).c_str() + L".png";
				}
				else
				{
					_path = path + std::to_wstring(i).c_str() + L".bmp";
				}
			}

			CImage* cImage = new CImage(AddBitmap(_path, &_width, &_height), _width, _height);
			imgs->AddImage(cImage);
		}
		m_vImageList.insert(std::make_pair(key, imgs));
		return imgs;
	}
	return nullptr;
}

vImage* ImageManager::FindImageVector(const std::string key)
{
	auto find = m_vImageList.find(key);
	if (find != m_vImageList.end())
	{
		return find->second;
	}
	cout << "�ִϸ��̼� key ���� : " << key << "\n";
	return nullptr;
}

vImage* ImageManager::AddImageVectorCopy(const std::string key)
{
	auto find = m_vImageList.find(key);
	if (find != m_vImageList.end())
	{
		vImage* vimg = new vImage();
		for (auto iter : find->second->GetImages())
		{
			vimg->AddImage(iter);
		}
		return vimg;
	}
	return nullptr;
}

void ImageManager::ImageLoad()
{
	AddImage("titleBackGround", L"./Resources/titleBackGround.png");

	AddImage("playerUI", L"./Resources/UI/playerUI.png");

	AddImage("tcmdbtns0018", L"./Resources/UI2/tcmdbtns0018.bmp");

	AddImage("backgnd", L"./Resources/backgnd.bmp");
	// �̳׶�

	AddWireImage("wirefram0000", "./Resources/Icon2/wirefram0000.bmp");
	AddWireImage("wirefram0002", "./Resources/Icon2/wirefram0002.bmp");
	AddWireImage("wirefram0005", "./Resources/Icon2/wirefram0005.bmp");
	AddWireImage("wirefram0007", "./Resources/Icon2/wirefram0007.bmp");
	AddWireImage("wirefram0106", "./Resources/Icon2/wirefram0106.bmp");
	AddWireImage("wirefram0113", "./Resources/Icon2/wirefram0113.bmp");
	AddWireImage("wirefram0111", "./Resources/Icon2/wirefram0111.bmp");
	AddWireImage("wirefram0112", "./Resources/Icon2/wirefram0112.bmp");
	AddWireImage("wirefram0120", "./Resources/Icon2/wirefram0120.bmp");
	AddWireImage("wirefram0123", "./Resources/Icon2/wirefram0123.bmp");
	AddWireImage("wirefram0109", "./Resources/Icon2/wirefram0109.bmp");

	AddWireImage("grpwire0000", "./Resources/Icon3/grpwire0000.bmp");
	AddWireImage("grpwire0002", "./Resources/Icon3/grpwire0002.bmp");
	AddWireImage("grpwire0005", "./Resources/Icon3/grpwire0005.bmp");
	AddWireImage("grpwire0007", "./Resources/Icon3/grpwire0007.bmp");
	AddWireImage("grpwire0106", "./Resources/Icon3/grpwire0106.bmp");
	AddWireImage("grpwire0113", "./Resources/Icon3/grpwire0113.bmp");
	AddWireImage("grpwire0111", "./Resources/Icon3/grpwire0111.bmp");
	AddWireImage("grpwire0112", "./Resources/Icon3/grpwire0112.bmp");
	AddWireImage("grpwire0120", "./Resources/Icon3/grpwire0120.bmp");
	AddWireImage("grpwire0123", "./Resources/Icon3/grpwire0123.bmp");
	AddWireImage("grpwire0109", "./Resources/Icon3/grpwire0109.bmp");

	AddImage("cmdicons0323", L"./Resources/Icon/cmdicons0323.bmp"); //����
	AddImage("cmdicons0238", L"./Resources/Icon/cmdicons0238.bmp"); //��ī���� 
	AddImage("cmdicons0237", L"./Resources/Icon/cmdicons0237.bmp"); //������ 
	AddImage("cmdicons0243", L"./Resources/Icon/cmdicons0243.bmp"); //����
	AddImage("cmdicons0245", L"./Resources/Icon/cmdicons0245.bmp"); //������ũ
	AddImage("cmdicons0246", L"./Resources/Icon/cmdicons0246.bmp"); //�Ϲ���ũ
	AddImage("cmdicons0120", L"./Resources/Icon/cmdicons0120.bmp"); //���丮 ����
	AddImage("cmdicons0240", L"./Resources/Icon/cmdicons0240.bmp"); //���ٿ�
	AddImage("cmdicons0287", L"./Resources/Icon/cmdicons0287.bmp"); //���� �߾�cmdicons0333
	AddImage("cmdicons0333", L"./Resources/Icon/cmdicons0333.bmp"); //���� ����

	AddImage("gas2", L"./Resources/gas/geyser0000.bmp"); //SCVcmdicons0007 ��ó
	AddImage("gass", L"./Resources/gas/geyshad0000.bmp"); //SCVcmdicons0007 ��ó

	AddImage("min01sha0000", L"./Resources/min/min01sha0000.bmp"); //SCVcmdicons0007 ��ó
	AddImage("min01sha0001", L"./Resources/min/min01sha0001.bmp"); //SCVcmdicons0007 ��ó
	AddImage("min01sha0002", L"./Resources/min/min01sha0002.bmp"); //SCVcmdicons0007 ��ó
	AddImage("min01sha0003", L"./Resources/min/min01sha0003.bmp"); //SCVcmdicons0007 ��ó

	AddImage("refinery0001", L"./Resources/gas/refinery0001.bmp"); //SCVcmdicons0007 ��ó
	AddImage("refinery0002", L"./Resources/gas/refinery0002.bmp"); //SCVcmdicons0007 ��ó
	AddImage("refinery0003", L"./Resources/gas/refinery0003.bmp"); //SCVcmdicons0007 ��ó
	AddImage("refinery0004", L"./Resources/gas/refinery0004.bmp"); //SCVcmdicons0007 ��ó
	AddImage("refinery0000", L"./Resources/gas/refinery0000.bmp"); //SCVcmdicons0007 ��ó


	AddImage("cmdicons0002", L"./Resources/Icon/cmdicons0002.bmp"); //SCVcmdicons0007 ��ó
	AddImage("cmdicons0013", L"./Resources/Icon/cmdicons0013.bmp"); //SCVcmdicons0007cmdicons0013 ����
	AddImage("cmdicons0023", L"./Resources/Icon/cmdicons0023.bmp"); //��ũ 
	AddImage("cmdicons0120", L"./Resources/Icon/cmdicons0120.bmp"); //��ũ cmdicons0120

	AddImage("cmdicons0023", L"./Resources/Icon/cmdicons0023.bmp"); //SCVcmdicons0007
	AddImage("cmdicons0005", L"./Resources/Icon/cmdicons0005.bmp"); //SCVcmdicons0007
	AddImage("cmdicons0003", L"./Resources/Icon/cmdicons0003.bmp"); //SCVcmdicons0007
	AddImage("cmdicons0002", L"./Resources/Icon/cmdicons0002.bmp"); //SCVcmdicons0007

	AddImage("tile0000", L"./Resources/ui6/tile0000.bmp"); //SCVcmdicons0007
	AddImage("tile0001", L"./Resources/ui6/tile0001.bmp"); //SCVcmdicons0007
	AddImage("tile0002", L"./Resources/ui6/tile0002.bmp"); //SCVcmdicons0007
	AddImage("tile0003", L"./Resources/ui6/tile0003.bmp"); //SCVcmdicons0007
	AddImage("tile0004", L"./Resources/ui6/tile0004.bmp"); //SCVcmdicons0007
	AddImage("tile0005", L"./Resources/ui6/tile0005.bmp"); //SCVcmdicons0007
	AddImage("tile0006", L"./Resources/ui6/tile0006.bmp"); //SCVcmdicons0007
	AddImage("tile0007", L"./Resources/ui6/tile0007.bmp"); //SCVcmdicons0007
	AddImage("tile0008", L"./Resources/ui6/tile0008.bmp"); //SCVcmdicons0007

	AddImage("terran0115", L"./Resources/ui6/terran0115.bmp"); //SCVcmdicons0007
	AddImage("terran0116", L"./Resources/ui6/terran0116.bmp"); //SCVcmdicons0007
	AddImage("terran0117", L"./Resources/ui6/terran0117.bmp"); //SCVcmdicons0007



	// SCV
	AddImage("wirefram0106", L"./Resources/Icon2/wirefram0106.bmp"); //SCVcmdicons0007
	AddImage("wirefram0000", L"./Resources/Icon2/wirefram0000.bmp"); //SCVcmdicons0007
	AddImage("wirefram0007", L"./Resources/Icon2/wirefram0007.bmp"); //wirefram0010
	AddImage("wirefram0010", L"./Resources/Icon2/wirefram0010.bmp"); //wirefram0010
	AddImage("wirefram0002", L"./Resources/Icon2/wirefram0002.bmp"); //����grpwire0002

	AddImage("cmdicons0007", L"./Resources/Icon/cmdicons0007.bmp"); //SCVcmdicons0007
	AddImage("grpwire0000", L"./Resources/Icon3/grpwire0000.bmp"); //SCVcmdicons0007
	AddImage("grpwire0010", L"./Resources/Icon3/grpwire0010.bmp"); //SCVcmdicons0007
	AddImage("grpwire0002", L"./Resources/Icon3/grpwire0002.bmp"); //����


	AddImage("scv_idle_1", L"./Resources/scv/scv0000.bmp");
	AddImage("scv_idle_2", L"./Resources/scv/scv0002.bmp");
	AddImage("scv_idle_3", L"./Resources/scv/scv0004.bmp");
	AddImage("scv_idle_4", L"./Resources/scv/scv0006.bmp");
	AddImage("scv_idle_5", L"./Resources/scv/scv0008.bmp");
	AddImage("scv_idle_6", L"./Resources/scv/scv0010.bmp");
	AddImage("scv_idle_7", L"./Resources/scv/scv0012.bmp");
	AddImage("scv_idle_8", L"./Resources/scv/scv0014.bmp");
	AddImage("scv_idle_9", L"./Resources/scv/scv0016.bmp");

	AddImage("scv_action_1_1", L"./Resources/scv/scv0017.bmp");
	AddImage("scv_action_1_2", L"./Resources/scv/scv0019.bmp");
	AddImage("scv_action_1_3", L"./Resources/scv/scv0021.bmp");
	AddImage("scv_action_1_4", L"./Resources/scv/scv0023.bmp");
	AddImage("scv_action_1_5", L"./Resources/scv/scv0025.bmp");
	AddImage("scv_action_1_6", L"./Resources/scv/scv0027.bmp");
	AddImage("scv_action_1_7", L"./Resources/scv/scv0029.bmp");
	AddImage("scv_action_1_8", L"./Resources/scv/scv0031.bmp");
	AddImage("scv_action_1_9", L"./Resources/scv/scv0033.bmp");

	AddImage("scv_action_2_1", L"./Resources/scv/scv0034.bmp");
	AddImage("scv_action_2_2", L"./Resources/scv/scv0036.bmp");
	AddImage("scv_action_2_3", L"./Resources/scv/scv0038.bmp");
	AddImage("scv_action_2_4", L"./Resources/scv/scv0040.bmp");
	AddImage("scv_action_2_5", L"./Resources/scv/scv0042.bmp");
	AddImage("scv_action_2_6", L"./Resources/scv/scv0044.bmp");
	AddImage("scv_action_2_7", L"./Resources/scv/scv0046.bmp");
	AddImage("scv_action_2_8", L"./Resources/scv/scv0048.bmp");
	AddImage("scv_action_2_9", L"./Resources/scv/scv0050.bmp");

	// SCV ������
	AddImage("cmdicons0234", L"./Resources/Icon/cmdicons0234.bmp");
	AddImage("cmdicons0235", L"./Resources/Icon/cmdicons0235.bmp");
	AddImage("cmdicons0236", L"./Resources/Icon/cmdicons0236.bmp");
	AddImage("cmdicons0025", L"./Resources/Icon/cmdicons0025.bmp");
	AddImage("cmdicons0231", L"./Resources/Icon/cmdicons0231.bmp");
	AddImage("cmdicons0232", L"./Resources/Icon/cmdicons0232.bmp");


	AddImage("cmdicons0230", L"./Resources/Icon/cmdicons0230.bmp"); // attack 
	AddImage("cmdicons0229", L"./Resources/Icon/cmdicons0229.bmp"); // stop
	AddImage("cmdicons0228", L"./Resources/Icon/cmdicons0228.bmp"); // move

	AddImage("tbldmed0000", L"./Resources/Bulid/Building2/tbldmed0000.bmp"); // attack 
	AddImage("tbldmed0001", L"./Resources/Bulid/Building2/tbldmed0001.bmp"); // stop
	AddImage("tbldmed0002", L"./Resources/Bulid/Building2/tbldmed0002.bmp"); // move

	AddImage("tb3shad0000", L"./Resources/Bulid/Building2/tb3shad0000.bmp"); // attack 
	AddImage("tb3shad0001", L"./Resources/Bulid/Building2/tb3shad0001.bmp"); // stop
	AddImage("tb3shad0002", L"./Resources/Bulid/Building2/tb3shad0002.bmp"); // move

	AddImage("tb2shad0000", L"./Resources/Bulid/Buliding/tb2shad0000.bmp"); // attack 
	AddImage("tb2shad0001", L"./Resources/Bulid/Buliding/tb2shad0001.bmp"); // stop
	AddImage("tb2shad0002", L"./Resources/Bulid/Buliding/tb2shad0002.bmp"); // move

	AddImage("tbldsml0000", L"./Resources/Bulid/Building3/tbldsml0000.bmp"); // attack 
	AddImage("tbldsml0001", L"./Resources/Bulid/Building3/tbldsml0001.bmp"); // stop
	AddImage("tbldsml0002", L"./Resources/Bulid/Building3/tbldsml0002.bmp"); // move
	AddImage("cmdicons0254", L"./Resources/Icon/cmdicons0254.bmp"); // stop
	AddImage("cmdicons0255", L"./Resources/Icon/cmdicons0255.bmp"); // move
	AddImage("cmdicons0237", L"./Resources/Icon/cmdicons0237.bmp"); // move

	AddImage("depot0000", L"./Resources/Bulid/depot/depot0000.bmp"); // �Ϲ�
	AddImage("depot0001", L"./Resources/Bulid/depot/depot0001.bmp"); // �ϼ���
	AddImage("depotshad", L"./Resources/Bulid/depot/tdeshad0000.bmp"); // �Ϲ� �׸���
	AddImage("depotshad1", L"./Resources/Bulid/depot/tdeshad0001.bmp"); // �ϼ��� �׸���

	AddImageVector("DepotIdle", L"./Resources/Bulid/depot/Idle/depott00", 0, 5);

	// Ŀ��� ���� 
	AddImage("tccshad0000", L"./Resources/Bulid/CommandCenter/tccshad0000.bmp"); // �Ϲݻ��� 
	AddImage("tccshad0001", L"./Resources/Bulid/CommandCenter/tccshad0001.bmp"); // �Ϲݻ��� 

	AddImage("control0000", L"./Resources/Bulid/CommandCenter/control0000.bmp"); // �Ϲݻ��� 
	AddImage("control0001", L"./Resources/Bulid/CommandCenter/control0001.bmp"); // �ϼ����ܰ�
	AddImage("control0002", L"./Resources/Bulid/CommandCenter/control0002.bmp"); // Up 1
	AddImage("control0003", L"./Resources/Bulid/CommandCenter/control0003.bmp"); // Up 2
	AddImage("control0004", L"./Resources/Bulid/CommandCenter/control0004.bmp"); // Up 3
	AddImage("controlt", L"./Resources/Bulid/CommandCenter/controlt.bmp"); // ���� ������

	// Ŀ��� ���� 
	AddImage("research0000", L"./Resources/Bulid/Science/research0000.bmp"); // �Ϲݻ��� 
	AddImage("research0001", L"./Resources/Bulid/Science/research0001.bmp"); // �ϼ����ܰ�
	AddImage("research0003", L"./Resources/Bulid/Science/research0003.bmp"); // Up 1
	AddImage("research0004", L"./Resources/Bulid/Science/research0004.bmp"); // Up 2
	AddImage("research0005", L"./Resources/Bulid/Science/research0005.bmp"); // Up 3

	//��ī����
	AddImage("academy0000", L"./Resources/Bulid/Academy/academy0000.bmp"); // �Ϲݻ��� 
	AddImage("academy0001", L"./Resources/Bulid/Academy/academy0001.bmp"); // �ϼ����ܰ�

	AddImage("tacshad0000", L"./Resources/Bulid/Academy/tacshad0000.bmp"); // �Ϲݻ��� 
	AddImage("tacshad0001", L"./Resources/Bulid/Academy/tacshad0001.bmp"); // �ϼ����ܰ�


	//�ƸӸ�
	AddImage("chemlab0000", L"./Resources/Bulid/Armory/chemlab0000.bmp"); // �Ϲݻ��� 
	AddImage("chemlab0001", L"./Resources/Bulid/Armory/chemlab0001.bmp"); // �ϼ����ܰ�


	// �����Ͼ ���̽�
	AddImage("weaponpl0000", L"./Resources/Bulid/engin/weaponpl0000.bmp"); // �Ϲݻ��� 
	AddImage("weaponpl0001", L"./Resources/Bulid/engin/weaponpl0001.bmp"); // �ϼ����ܰ�
	AddImage("weaponpl0002", L"./Resources/Bulid/engin/weaponpl0002.bmp"); // Up 1
	AddImage("weaponpl0003", L"./Resources/Bulid/engin/weaponpl0003.bmp"); // Up 2
	AddImage("weaponpl0004", L"./Resources/Bulid/engin/weaponpl0004.bmp"); // Up 3

	// ���� ������
	AddImage("tbldlrg0000", L"./Resources/Bulid/Buliding/tbldlrg0000.bmp"); // move
	AddImage("tbldlrg0001", L"./Resources/Bulid/Buliding/tbldlrg0001.bmp"); // move
	AddImage("tbldlrg0002", L"./Resources/Bulid/Buliding/tbldlrg0002.bmp"); // move
	AddImage("grpwire0106", L"./Resources/Icon3/grpwire0106.bmp");

	//���� �̹��� 
	AddImage("cmdicons0111", L"./Resources/Icon/cmdicons0111.bmp"); // ������
	AddImage("tbarrack0000", L"./Resources/Bulid/Barrack/tbarrack0000.bmp"); // ����
	AddImage("tbarrack0001", L"./Resources/Bulid/Barrack/tbarrack0001.bmp"); // �ϼ���
	AddImage("tbarrack0002", L"./Resources/Bulid/Barrack/tbarrack0002.bmp"); // ���߿� �߱���
	AddImage("tbarrack0003", L"./Resources/Bulid/Barrack/tbarrack0003.bmp"); // ���߿� �߱���
	AddImageVector("Barrack_Work", L"./Resources/Bulid/Barrack/tbarrack00", 7, 8);
	AddImageVector("deathEffect", L"./Resources/ty/death/tbangl00", 0, 9);
	AddImageVector("bang1Effect", L"./Resources/ty/bang1/eychit00", 0, 13);

	AddImageVector("bang2Effect", L"./Resources/ty/bang2/tbangs00", 0, 8);

	AddImage("tbrshad0000", L"./Resources/Bulid/Barrack/shad/tbrshad0000.bmp"); // ���߿� �߱���
	AddImage("tbrshad0001", L"./Resources/Bulid/Barrack/shad/tbrshad0001.bmp"); // ���߿� �߱���


	AddImage("starport0000", L"./Resources/Bulid/starport/starport0000.bmp"); // ����
	AddImage("starport0001", L"./Resources/Bulid/starport/starport0001.bmp"); // ���߿� �߱���
	AddImage("starport0002", L"./Resources/Bulid/starport/starport0002.bmp"); // ���߿� �߱���
	AddImage("starport0003", L"./Resources/Bulid/starport/starport0003.bmp"); // ���߿� �߱���
	AddImage("starport0004", L"./Resources/Bulid/starport/starport0004.bmp"); // 
	AddImage("starport0005", L"./Resources/Bulid/starport/starport0005.bmp"); // �ϼ���

	AddImage("tcmdbtns0000", L"./Resources/UI2/tcmdbtns0000.bmp");

	AddImage("targg0001", L"./Resources/cursor/targg0001.bmp"); // Ŭ���� ��ġ�����°�

	AddImage("wirefram0106", L"./Resources/Icon2/wirefram0106.bmp"); //SCVcmdicons0007
	AddImage("grpwire0007", L"./Resources/Icon3/grpwire0007.bmp"); // SCV �δ����� ������

	AddImage("cmdicons0106", L"./Resources/Icon/cmdicons0106.bmp"); // Ŀ��弾��
	AddImage("cmdicons0109", L"./Resources/Icon/cmdicons0109.bmp"); // ���ö��� ����Ʈ
	AddImage("cmdicons0110", L"./Resources/Icon/cmdicons0110.bmp"); // ����

	AddImage("cmdicons0112", L"./Resources/Icon/cmdicons0112.bmp"); // ��ī����

	// ���丮
	AddImage("tfashad0000", L"./Resources/Bulid/Factory/tfashad0000.bmp"); // �⺻
	AddImage("tfashad0001", L"./Resources/Bulid/Factory/tfashad0001.bmp"); // �⺻

	AddImage("tmsshad0000", L"./Resources/Bulid/Factory/tmsshad0000.bmp"); // �⺻
	AddImage("tmsshad0001", L"./Resources/Bulid/Factory/tmsshad0001.bmp"); // �⺻


	AddImage("factory0000", L"./Resources/Bulid/Factory/factory0000.bmp"); // �⺻
	AddImage("factory0001", L"./Resources/Bulid/Factory/factory0001.bmp"); // �ϼ� ��
	AddImage("factory0002", L"./Resources/Bulid/Factory/factory0002.bmp"); // ���� 1
	AddImage("factory0003", L"./Resources/Bulid/Factory/factory0003.bmp"); // ���� 2
	AddImage("factory0004", L"./Resources/Bulid/Factory/factory0004.bmp"); // ���� 3
	AddImage("factory0005", L"./Resources/Bulid/Factory/factory0005.bmp"); // ���� 4

	AddImageVector("tmnexplo0000", L"./Resources/ty/tmnexplo00", 0, 9); // �⺻

	AddImageVector("machines", L"./Resources/Bulid/Factory/machines00", 0, 3); // �⺻
	AddImage("machines", L"./Resources/Bulid/Factory/machines0000.bmp"); // �⺻
	AddImage("machines0004", L"./Resources/Bulid/Factory/machines0004.bmp"); // ���� 4

	AddImageVector("machinec0000", L"./Resources/Bulid/Factory/machinec00", 0, 3); // �ϼ� ��
	AddImageVector("tmsshad0000", L"./Resources/Bulid/Factory/tmsshad00", 1, 4); // �ϼ� ��

	AddImageVector("FactoryIdle", L"./Resources/Bulid/Factory/Idle/factoryt00", 0, 2);
	AddImageVector("grenade0000", L"./Resources/Vulture/grenade00", 0, 3);

	AddImageVector("mush", L"./Resources/ty/mush/mushroom00", 0, 13);
	AddImageVector("bang1", L"./Resources/ty/bang1/eychit00", 0, 13);

	AddImage("cmdicons0113", L"./Resources/Icon/cmdicons0113.bmp"); // ���丮
	AddImage("cmdicons0114", L"./Resources/Icon/cmdicons0114.bmp"); // ��Ÿ��Ʈ
	AddImage("cmdicons0122", L"./Resources/Icon/cmdicons0122.bmp"); // �����Ͼ
	AddImage("cmdicons0124", L"./Resources/Icon/cmdicons0124.bmp"); // �ͷ�
	AddImage("cmdicons0123", L"./Resources/Icon/cmdicons0123.bmp"); // �ͷ�

	AddImage("cmdicons0125", L"./Resources/Icon/cmdicons0125.bmp"); // ��Ŀ
	AddImage("cmdicons0116", L"./Resources/Icon/cmdicons0116.bmp"); // ���̾�
	AddImage("cmdicons0120", L"./Resources/Icon/cmdicons0120.bmp"); // ��Ŀ

	AddImage("cmdicons0287", L"./Resources/Icon/cmdicons0287.bmp"); // ��Ŀcmdicons0287

	AddImage("cmdicons0292", L"./Resources/Icon/cmdicons0292.bmp"); // ��Ŀcmdicons0287
	AddImage("cmdicons0288", L"./Resources/Icon/cmdicons0288.bmp"); // ��Ŀcmdicons0287
	AddImage("tcmdbtns0012", L"./Resources/UI2/tcmdbtns0012.bmp"); // ��Ŀcmdicons0287


	// ���� �̹��� 
	AddImage("cmdicons0000", L"./Resources/Icon/cmdicons0000.bmp"); // ���� ������ 

	// �޵� �̹��� �޵����
	//AddImage("cmdicons0000", L"./Resources/UI/cmdicons0000.bmp"); // ���� ������ 

	// ���̾ �̹���
	AddImage("cmdicons0010", L"./Resources/Icon/cmdicons0010.bmp"); // ���̾ ������ 

	// ��Ʈ �̹���
	AddImage("cmdicons0001", L"./Resources/Icon/cmdicons0001.bmp"); // ��Ʈ ������ 

	AddImage("cmdicons0286", L"./Resources/Icon/cmdicons0286.bmp"); // ������ ���� ��������

	AddImage("cmdicons0282", L"./Resources/Icon/cmdicons0282.bmp"); // �ǹ� ����
	AddImage("cmdicons0283", L"./Resources/Icon/cmdicons0283.bmp"); // �ǹ� ������

	AddImage("cmdicons0236", L"./Resources/Icon/cmdicons0236.bmp"); // ĵ��

	// ����Ʈ
	AddImageVector("f2", L"./Resources/f2/tbangx00", 0, 13); // �ǹ� ����

	AddImageVector("SCVEffect", L"./Resources/SCVEffect/scvspark00", 0, 9); // �ǹ� ����

	// ���콺 Ŀ��
	AddImageVector("arrow0000", L"./Resources/cursor/arrow00", 0, 4); // �Ϲ�
	AddImage("drag0000", L"./Resources/cursor/drag0000.bmp"); // �巡��

	// �̳׶�
	AddImage("min010000", L"./Resources/min/min010000.bmp"); // �̳׶� ū��
	AddImage("min010001", L"./Resources/min/min010001.bmp"); // �̳׶� �߰�
	AddImage("min010002", L"./Resources/min/min010002.bmp"); // �̳׶� ���߰�
	AddImage("min010003", L"./Resources/min/min010003.bmp"); // �̳׶� ����

	AddImage("mul-tail", L"./Resources/lobby/Multi Player/mul-tail.png"); // �̳׶� ����

	AddImageVector("Editor", L"./Resources/lobby/Editor/editor", 0, 84, true); // �Ϲ�
	AddImageVector("Multi Player", L"./Resources/lobby/Multi Player/multi", 0, 49, true); // �Ϲ�
	AddImageVector("Exit", L"./Resources/lobby/Exit/exit", 0, 49, true); // �Ϲ�
	AddImageVector("Single Player", L"./Resources/lobby/Single Player/single", 0, 34, true); // �Ϲ�

	AddImageVector("editoron", L"./Resources/lobby/Editor/English/editoron", 0, 19, true); // ���콺 
	AddImageVector("exiton", L"./Resources/lobby/Exit/English/exiton", 0, 29, true); // ���콺
	AddImageVector("singleon", L"./Resources/lobby/Single Player/English/singleon", 8, 67, true); // ���콺

	//���� �ʻ�ȭ
	AddImageVector("scv1P", L"./Resources/Terran/SCV/tscfid00", 0, 9, true); // ���콺
	AddImageVector("scv2P", L"./Resources/Terran/SCV/tscfid01", 0, 9, true); // ���콺
	AddImageVector("scv3P", L"./Resources/Terran/SCV/tscfid02", 0, 9, true); // ���콺
	AddImageVector("scv4P", L"./Resources/Terran/SCV/tscfid03", 0, 14, true); // ���콺

	AddImageVector("vulture1P", L"./Resources/Terran/Vulture/tvufid00", 0, 9, true); // ���콺
	AddImageVector("vulture2P", L"./Resources/Terran/Vulture/tvufid01", 0, 9, true); // ���콺
	AddImageVector("vulture3P", L"./Resources/Terran/Vulture/tvufid02", 0, 9, true); // ���콺
	AddImageVector("vulture4P", L"./Resources/Terran/Vulture/tvufid03", 0, 15, true); // ���콺

	AddImageVector("Marine1P", L"./Resources/Terran/Marine/tmafid00", 0, 9, true); // ���콺
	AddImageVector("Marine2P", L"./Resources/Terran/Marine/tmafid01", 0, 9, true); // ���콺
	AddImageVector("Marine3P", L"./Resources/Terran/Marine/tmafid02", 0, 9, true); // ���콺
	AddImageVector("Marine4P", L"./Resources/Terran/Marine/tmafid03", 0, 14, true); // ���콺

	AddImageVector("tank1P", L"./Resources/Terran/Seige Tank/ttafid00", 0, 9, true); // ���콺
	AddImageVector("tank2P", L"./Resources/Terran/Seige Tank/ttafid01", 0, 9, true); // ���콺
	AddImageVector("tank3P", L"./Resources/Terran/Seige Tank/ttafid02", 0, 9, true); // ���콺
	AddImageVector("tank4P", L"./Resources/Terran/Seige Tank/ttafid03", 0, 14, true); // ���콺

	AddImageVector("Advisor1P", L"./Resources/Terran/Advisor/tadfid00", 0, 9, true); // ���콺
	AddImageVector("Advisor2P", L"./Resources/Terran/Advisor/tadfid01", 0, 9, true); // ���콺
	AddImageVector("Advisor3P", L"./Resources/Terran/Advisor/tadfid02", 0, 9, true); // ���콺
	AddImageVector("Advisor4P", L"./Resources/Terran/Advisor/tadfid03", 0, 14, true); // ���콺

	AddImage("marin_attack_1_1_s", L"./Resources/marin/tmashad0034.bmp"); // ���� ���� ������ 1
	AddImage("marin_attack_1_2_s", L"./Resources/marin/tmashad0036.bmp");
	AddImage("marin_attack_1_3_s", L"./Resources/marin/tmashad0038.bmp");
	AddImage("marin_attack_1_4_s", L"./Resources/marin/tmashad0040.bmp");
	AddImage("marin_attack_1_5_s", L"./Resources/marin/tmashad0042.bmp");
	AddImage("marin_attack_1_6_s", L"./Resources/marin/tmashad0044.bmp");
	AddImage("marin_attack_1_7_s", L"./Resources/marin/tmashad0046.bmp");
	AddImage("marin_attack_1_8_s", L"./Resources/marin/tmashad0048.bmp");
	AddImage("marin_attack_1_9_s", L"./Resources/marin/tmashad0050.bmp");

	AddImage("marin_attack_2_1_s", L"./Resources/marin/tmashad0051.bmp"); // ���� ���������� 2
	AddImage("marin_attack_2_2_s", L"./Resources/marin/tmashad0053.bmp");
	AddImage("marin_attack_2_3_s", L"./Resources/marin/tmashad0055.bmp");
	AddImage("marin_attack_2_4_s", L"./Resources/marin/tmashad0057.bmp");
	AddImage("marin_attack_2_5_s", L"./Resources/marin/tmashad0059.bmp");
	AddImage("marin_attack_2_6_s", L"./Resources/marin/tmashad0061.bmp");
	AddImage("marin_attack_2_7_s", L"./Resources/marin/tmashad0063.bmp");
	AddImage("marin_attack_2_8_s", L"./Resources/marin/tmashad0065.bmp");
	AddImage("marin_attack_2_9_s", L"./Resources/marin/tmashad0067.bmp");

	AddImage("marin_idle_1_1_s", L"./Resources/marin/tmashad0068.bmp"); // ���� �⺻ ���
	AddImage("marin_idle_1_2_s", L"./Resources/marin/tmashad0070.bmp");
	AddImage("marin_idle_1_3_s", L"./Resources/marin/tmashad0072.bmp");
	AddImage("marin_idle_1_4_s", L"./Resources/marin/tmashad0074.bmp");
	AddImage("marin_idle_1_5_s", L"./Resources/marin/tmashad0076.bmp");
	AddImage("marin_idle_1_6_s", L"./Resources/marin/tmashad0078.bmp");
	AddImage("marin_idle_1_7_s", L"./Resources/marin/tmashad0080.bmp");
	AddImage("marin_idle_1_8_s", L"./Resources/marin/tmashad0082.bmp");
	AddImage("marin_idle_1_9_s", L"./Resources/marin/tmashad0084.bmp");

	AddImage("marin_idle_action_1_1_s", L"./Resources/marin/tmashad0000.bmp"); // ���� ��� ��� ������ ������ 2
	AddImage("marin_idle_action_1_2_s", L"./Resources/marin/tmashad0002.bmp");
	AddImage("marin_idle_action_1_3_s", L"./Resources/marin/tmashad0004.bmp");
	AddImage("marin_idle_action_1_4_s", L"./Resources/marin/tmashad0006.bmp");
	AddImage("marin_idle_action_1_5_s", L"./Resources/marin/tmashad0008.bmp");
	AddImage("marin_idle_action_1_6_s", L"./Resources/marin/tmashad0010.bmp");
	AddImage("marin_idle_action_1_7_s", L"./Resources/marin/tmashad0012.bmp");
	AddImage("marin_idle_action_1_8_s", L"./Resources/marin/tmashad0014.bmp");
	AddImage("marin_idle_action_1_9_s", L"./Resources/marin/tmashad0016.bmp");

	AddImage("marin_idle_action_2_1_s", L"./Resources/marin/tmashad0018.bmp"); // ���� ��� ��� ������ ������ 2
	AddImage("marin_idle_action_2_2_s", L"./Resources/marin/tmashad0020.bmp");
	AddImage("marin_idle_action_2_3_s", L"./Resources/marin/tmashad0022.bmp");
	AddImage("marin_idle_action_2_4_s", L"./Resources/marin/tmashad0024.bmp");
	AddImage("marin_idle_action_2_5_s", L"./Resources/marin/tmashad0026.bmp");
	AddImage("marin_idle_action_2_6_s", L"./Resources/marin/tmashad0028.bmp");
	AddImage("marin_idle_action_2_7_s", L"./Resources/marin/tmashad0030.bmp");
	AddImage("marin_idle_action_2_8_s", L"./Resources/marin/tmashad0032.bmp");
	AddImage("marin_idle_action_2_9_s", L"./Resources/marin/tmashad0034.bmp");

	AddImage("marin_run_1_1_s", L"./Resources/marin/tmashad0085.bmp"); // ����
	AddImage("marin_run_1_2_s", L"./Resources/marin/tmashad0087.bmp"); // ����
	AddImage("marin_run_1_3_s", L"./Resources/marin/tmashad0089.bmp"); // ����
	AddImage("marin_run_1_4_s", L"./Resources/marin/tmashad0091.bmp"); // ����
	AddImage("marin_run_1_5_s", L"./Resources/marin/tmashad0093.bmp"); // ����
	AddImage("marin_run_1_6_s", L"./Resources/marin/tmashad0095.bmp"); // ����
	AddImage("marin_run_1_7_s", L"./Resources/marin/tmashad0097.bmp"); // ����
	AddImage("marin_run_1_8_s", L"./Resources/marin/tmashad0099.bmp"); // ����
	AddImage("marin_run_1_9_s", L"./Resources/marin/tmashad0101.bmp"); // ����

	AddImage("marin_run_2_1_s", L"./Resources/marin/tmashad0102.bmp"); // ����
	AddImage("marin_run_2_2_s", L"./Resources/marin/tmashad0104.bmp"); // ����
	AddImage("marin_run_2_3_s", L"./Resources/marin/tmashad0106.bmp"); // ����
	AddImage("marin_run_2_4_s", L"./Resources/marin/tmashad0108.bmp"); // ����
	AddImage("marin_run_2_5_s", L"./Resources/marin/tmashad0110.bmp"); // ����
	AddImage("marin_run_2_6_s", L"./Resources/marin/tmashad0112.bmp"); // ����
	AddImage("marin_run_2_7_s", L"./Resources/marin/tmashad0114.bmp"); // ����
	AddImage("marin_run_2_8_s", L"./Resources/marin/tmashad0116.bmp"); // ����
	AddImage("marin_run_2_9_s", L"./Resources/marin/tmashad0118.bmp"); // ����

	AddImage("marin_run_3_1_s", L"./Resources/marin/tmashad0119.bmp"); // ����
	AddImage("marin_run_3_2_s", L"./Resources/marin/tmashad0121.bmp"); // ����
	AddImage("marin_run_3_3_s", L"./Resources/marin/tmashad0123.bmp"); // ����
	AddImage("marin_run_3_4_s", L"./Resources/marin/tmashad0125.bmp"); // ����
	AddImage("marin_run_3_5_s", L"./Resources/marin/tmashad0127.bmp"); // ����
	AddImage("marin_run_3_6_s", L"./Resources/marin/tmashad0129.bmp"); // ����
	AddImage("marin_run_3_7_s", L"./Resources/marin/tmashad0131.bmp"); // ����
	AddImage("marin_run_3_8_s", L"./Resources/marin/tmashad0133.bmp"); // ����
	AddImage("marin_run_3_9_s", L"./Resources/marin/tmashad0135.bmp"); // ����

	AddImage("marin_run_4_1_s", L"./Resources/marin/tmashad0136.bmp"); // ����
	AddImage("marin_run_4_2_s", L"./Resources/marin/tmashad0138.bmp"); // ����
	AddImage("marin_run_4_3_s", L"./Resources/marin/tmashad0140.bmp"); // ����
	AddImage("marin_run_4_4_s", L"./Resources/marin/tmashad0142.bmp"); // ����
	AddImage("marin_run_4_5_s", L"./Resources/marin/tmashad0144.bmp"); // ����
	AddImage("marin_run_4_6_s", L"./Resources/marin/tmashad0146.bmp"); // ����
	AddImage("marin_run_4_7_s", L"./Resources/marin/tmashad0148.bmp"); // ����
	AddImage("marin_run_4_8_s", L"./Resources/marin/tmashad0150.bmp"); // ����
	AddImage("marin_run_4_9_s", L"./Resources/marin/tmashad0152.bmp"); // ����

	AddImage("marin_run_5_1_s", L"./Resources/marin/tmashad0153.bmp"); // ����
	AddImage("marin_run_5_2_s", L"./Resources/marin/tmashad0155.bmp"); // ����
	AddImage("marin_run_5_3_s", L"./Resources/marin/tmashad0157.bmp"); // ����
	AddImage("marin_run_5_4_s", L"./Resources/marin/tmashad0159.bmp"); // ����
	AddImage("marin_run_5_5_s", L"./Resources/marin/tmashad0161.bmp"); // ����
	AddImage("marin_run_5_6_s", L"./Resources/marin/tmashad0163.bmp"); // ����
	AddImage("marin_run_5_7_s", L"./Resources/marin/tmashad0165.bmp"); // ����
	AddImage("marin_run_5_8_s", L"./Resources/marin/tmashad0167.bmp"); // ����
	AddImage("marin_run_5_9_s", L"./Resources/marin/tmashad0169.bmp"); // ����

	AddImage("marin_run_6_1_s", L"./Resources/marin/tmashad0170.bmp"); // ����
	AddImage("marin_run_6_2_s", L"./Resources/marin/tmashad0172.bmp"); // ����
	AddImage("marin_run_6_3_s", L"./Resources/marin/tmashad0174.bmp"); // ����
	AddImage("marin_run_6_4_s", L"./Resources/marin/tmashad0176.bmp"); // ����
	AddImage("marin_run_6_5_s", L"./Resources/marin/tmashad0178.bmp"); // ����
	AddImage("marin_run_6_6_s", L"./Resources/marin/tmashad0180.bmp"); // ����
	AddImage("marin_run_6_7_s", L"./Resources/marin/tmashad0182.bmp"); // ����
	AddImage("marin_run_6_8_s", L"./Resources/marin/tmashad0184.bmp"); // ����
	AddImage("marin_run_6_9_s", L"./Resources/marin/tmashad0186.bmp"); // ����

	AddImage("marin_run_7_1_s", L"./Resources/marin/tmashad0187.bmp"); // ����
	AddImage("marin_run_7_2_s", L"./Resources/marin/tmashad0189.bmp"); // ����
	AddImage("marin_run_7_3_s", L"./Resources/marin/tmashad0191.bmp"); // ����
	AddImage("marin_run_7_4_s", L"./Resources/marin/tmashad0193.bmp"); // ����
	AddImage("marin_run_7_5_s", L"./Resources/marin/tmashad0195.bmp"); // ����
	AddImage("marin_run_7_6_s", L"./Resources/marin/tmashad0197.bmp"); // ����
	AddImage("marin_run_7_7_s", L"./Resources/marin/tmashad0199.bmp"); // ����
	AddImage("marin_run_7_8_s", L"./Resources/marin/tmashad0201.bmp"); // ����
	AddImage("marin_run_7_9_s", L"./Resources/marin/tmashad0203.bmp"); // ����

	AddImage("marin_run_8_1_s", L"./Resources/marin/tmashad0204.bmp"); // ����
	AddImage("marin_run_8_2_s", L"./Resources/marin/tmashad0206.bmp"); // ����
	AddImage("marin_run_8_3_s", L"./Resources/marin/tmashad0208.bmp"); // ����
	AddImage("marin_run_8_4_s", L"./Resources/marin/tmashad0210.bmp"); // ����
	AddImage("marin_run_8_5_s", L"./Resources/marin/tmashad0212.bmp"); // ����
	AddImage("marin_run_8_6_s", L"./Resources/marin/tmashad0214.bmp"); // ����
	AddImage("marin_run_8_7_s", L"./Resources/marin/tmashad0216.bmp"); // ����
	AddImage("marin_run_8_8_s", L"./Resources/marin/tmashad0218.bmp"); // ����
	AddImage("marin_run_8_9_s", L"./Resources/marin/tmashad0220.bmp"); // ����

	AddImage("marin_attack_1_1", L"./Resources/marin/Attack/marine0034.bmp"); // ���� ���� ������ 1
	AddImage("marin_attack_1_2", L"./Resources/marin/Attack/marine0036.bmp");
	AddImage("marin_attack_1_3", L"./Resources/marin/Attack/marine0038.bmp");
	AddImage("marin_attack_1_4", L"./Resources/marin/Attack/marine0040.bmp");
	AddImage("marin_attack_1_5", L"./Resources/marin/Attack/marine0042.bmp");
	AddImage("marin_attack_1_6", L"./Resources/marin/Attack/marine0044.bmp");
	AddImage("marin_attack_1_7", L"./Resources/marin/Attack/marine0046.bmp");
	AddImage("marin_attack_1_8", L"./Resources/marin/Attack/marine0048.bmp");
	AddImage("marin_attack_1_9", L"./Resources/marin/Attack/marine0050.bmp");

	AddImage("marin_attack_2_1", L"./Resources/marin/Attack/marine0051.bmp"); // ���� ���������� 2
	AddImage("marin_attack_2_2", L"./Resources/marin/Attack/marine0053.bmp");
	AddImage("marin_attack_2_3", L"./Resources/marin/Attack/marine0055.bmp");
	AddImage("marin_attack_2_4", L"./Resources/marin/Attack/marine0057.bmp");
	AddImage("marin_attack_2_5", L"./Resources/marin/Attack/marine0059.bmp");
	AddImage("marin_attack_2_6", L"./Resources/marin/Attack/marine0061.bmp");
	AddImage("marin_attack_2_7", L"./Resources/marin/Attack/marine0063.bmp");
	AddImage("marin_attack_2_8", L"./Resources/marin/Attack/marine0065.bmp");
	AddImage("marin_attack_2_9", L"./Resources/marin/Attack/marine0067.bmp");

	AddImage("marin_idle_1_1", L"./Resources/marin/Idle/marine0068.bmp"); // ���� �⺻ ���
	AddImage("marin_idle_1_2", L"./Resources/marin/Idle/marine0070.bmp");
	AddImage("marin_idle_1_3", L"./Resources/marin/Idle/marine0072.bmp");
	AddImage("marin_idle_1_4", L"./Resources/marin/Idle/marine0074.bmp");
	AddImage("marin_idle_1_5", L"./Resources/marin/Idle/marine0076.bmp");
	AddImage("marin_idle_1_6", L"./Resources/marin/Idle/marine0078.bmp");
	AddImage("marin_idle_1_7", L"./Resources/marin/Idle/marine0080.bmp");
	AddImage("marin_idle_1_8", L"./Resources/marin/Idle/marine0082.bmp");
	AddImage("marin_idle_1_9", L"./Resources/marin/Idle/marine0084.bmp");

	AddImage("marin_idle_action_1_1", L"./Resources/marin/Idle2/marine0000.bmp"); // ���� ��� ��� ������ ������ 2
	AddImage("marin_idle_action_1_2", L"./Resources/marin/Idle2/marine0002.bmp");
	AddImage("marin_idle_action_1_3", L"./Resources/marin/Idle2/marine0004.bmp");
	AddImage("marin_idle_action_1_4", L"./Resources/marin/Idle2/marine0006.bmp");
	AddImage("marin_idle_action_1_5", L"./Resources/marin/Idle2/marine0008.bmp");
	AddImage("marin_idle_action_1_6", L"./Resources/marin/Idle2/marine0010.bmp");
	AddImage("marin_idle_action_1_7", L"./Resources/marin/Idle2/marine0012.bmp");
	AddImage("marin_idle_action_1_8", L"./Resources/marin/Idle2/marine0014.bmp");
	AddImage("marin_idle_action_1_9", L"./Resources/marin/Idle2/marine0016.bmp");

	AddImage("marin_idle_action_2_1", L"./Resources/marin/Idle2/marine0018.bmp"); // ���� ��� ��� ������ ������ 2
	AddImage("marin_idle_action_2_2", L"./Resources/marin/Idle2/marine0020.bmp");
	AddImage("marin_idle_action_2_3", L"./Resources/marin/Idle2/marine0022.bmp");
	AddImage("marin_idle_action_2_4", L"./Resources/marin/Idle2/marine0024.bmp");
	AddImage("marin_idle_action_2_5", L"./Resources/marin/Idle2/marine0026.bmp");
	AddImage("marin_idle_action_2_6", L"./Resources/marin/Idle2/marine0028.bmp");
	AddImage("marin_idle_action_2_7", L"./Resources/marin/Idle2/marine0030.bmp");
	AddImage("marin_idle_action_2_8", L"./Resources/marin/Idle2/marine0032.bmp");
	AddImage("marin_idle_action_2_9", L"./Resources/marin/Idle2/marine0034.bmp");

	AddImage("marin_run_1_1", L"./Resources/marin/Run1/marine0085.bmp"); // ����
	AddImage("marin_run_1_2", L"./Resources/marin/Run1/marine0087.bmp"); // ����
	AddImage("marin_run_1_3", L"./Resources/marin/Run1/marine0089.bmp"); // ����
	AddImage("marin_run_1_4", L"./Resources/marin/Run1/marine0091.bmp"); // ����
	AddImage("marin_run_1_5", L"./Resources/marin/Run1/marine0093.bmp"); // ����
	AddImage("marin_run_1_6", L"./Resources/marin/Run1/marine0095.bmp"); // ����
	AddImage("marin_run_1_7", L"./Resources/marin/Run1/marine0097.bmp"); // ����
	AddImage("marin_run_1_8", L"./Resources/marin/Run1/marine0099.bmp"); // ����
	AddImage("marin_run_1_9", L"./Resources/marin/Run1/marine0101.bmp"); // ����

	AddImage("marin_run_2_1", L"./Resources/marin/Run2/marine0102.bmp"); // ����
	AddImage("marin_run_2_2", L"./Resources/marin/Run2/marine0104.bmp"); // ����
	AddImage("marin_run_2_3", L"./Resources/marin/Run2/marine0106.bmp"); // ����
	AddImage("marin_run_2_4", L"./Resources/marin/Run2/marine0108.bmp"); // ����
	AddImage("marin_run_2_5", L"./Resources/marin/Run2/marine0110.bmp"); // ����
	AddImage("marin_run_2_6", L"./Resources/marin/Run2/marine0112.bmp"); // ����
	AddImage("marin_run_2_7", L"./Resources/marin/Run2/marine0114.bmp"); // ����
	AddImage("marin_run_2_8", L"./Resources/marin/Run2/marine0116.bmp"); // ����
	AddImage("marin_run_2_9", L"./Resources/marin/Run2/marine0118.bmp"); // ����

	AddImage("marin_run_3_1", L"./Resources/marin/Run3/marine0119.bmp"); // ����
	AddImage("marin_run_3_2", L"./Resources/marin/Run3/marine0121.bmp"); // ����
	AddImage("marin_run_3_3", L"./Resources/marin/Run3/marine0123.bmp"); // ����
	AddImage("marin_run_3_4", L"./Resources/marin/Run3/marine0125.bmp"); // ����
	AddImage("marin_run_3_5", L"./Resources/marin/Run3/marine0127.bmp"); // ����
	AddImage("marin_run_3_6", L"./Resources/marin/Run3/marine0129.bmp"); // ����
	AddImage("marin_run_3_7", L"./Resources/marin/Run3/marine0131.bmp"); // ����
	AddImage("marin_run_3_8", L"./Resources/marin/Run3/marine0133.bmp"); // ����
	AddImage("marin_run_3_9", L"./Resources/marin/Run3/marine0135.bmp"); // ����

	AddImage("marin_run_4_1", L"./Resources/marin/Run4/marine0136.bmp"); // ����
	AddImage("marin_run_4_2", L"./Resources/marin/Run4/marine0138.bmp"); // ����
	AddImage("marin_run_4_3", L"./Resources/marin/Run4/marine0140.bmp"); // ����
	AddImage("marin_run_4_4", L"./Resources/marin/Run4/marine0142.bmp"); // ����
	AddImage("marin_run_4_5", L"./Resources/marin/Run4/marine0144.bmp"); // ����
	AddImage("marin_run_4_6", L"./Resources/marin/Run4/marine0146.bmp"); // ����
	AddImage("marin_run_4_7", L"./Resources/marin/Run4/marine0148.bmp"); // ����
	AddImage("marin_run_4_8", L"./Resources/marin/Run4/marine0150.bmp"); // ����
	AddImage("marin_run_4_9", L"./Resources/marin/Run4/marine0152.bmp"); // ����

	AddImage("marin_run_5_1", L"./Resources/marin/Run5/marine0153.bmp"); // ����
	AddImage("marin_run_5_2", L"./Resources/marin/Run5/marine0155.bmp"); // ����
	AddImage("marin_run_5_3", L"./Resources/marin/Run5/marine0157.bmp"); // ����
	AddImage("marin_run_5_4", L"./Resources/marin/Run5/marine0159.bmp"); // ����
	AddImage("marin_run_5_5", L"./Resources/marin/Run5/marine0161.bmp"); // ����
	AddImage("marin_run_5_6", L"./Resources/marin/Run5/marine0163.bmp"); // ����
	AddImage("marin_run_5_7", L"./Resources/marin/Run5/marine0165.bmp"); // ����
	AddImage("marin_run_5_8", L"./Resources/marin/Run5/marine0167.bmp"); // ����
	AddImage("marin_run_5_9", L"./Resources/marin/Run5/marine0169.bmp"); // ����

	AddImage("marin_run_6_1", L"./Resources/marin/Run6/marine0170.bmp"); // ����
	AddImage("marin_run_6_2", L"./Resources/marin/Run6/marine0172.bmp"); // ����
	AddImage("marin_run_6_3", L"./Resources/marin/Run6/marine0174.bmp"); // ����
	AddImage("marin_run_6_4", L"./Resources/marin/Run6/marine0176.bmp"); // ����
	AddImage("marin_run_6_5", L"./Resources/marin/Run6/marine0178.bmp"); // ����
	AddImage("marin_run_6_6", L"./Resources/marin/Run6/marine0180.bmp"); // ����
	AddImage("marin_run_6_7", L"./Resources/marin/Run6/marine0182.bmp"); // ����
	AddImage("marin_run_6_8", L"./Resources/marin/Run6/marine0184.bmp"); // ����
	AddImage("marin_run_6_9", L"./Resources/marin/Run6/marine0186.bmp"); // ����

	AddImage("marin_run_7_1", L"./Resources/marin/Run7/marine0187.bmp"); // ����
	AddImage("marin_run_7_2", L"./Resources/marin/Run7/marine0189.bmp"); // ����
	AddImage("marin_run_7_3", L"./Resources/marin/Run7/marine0191.bmp"); // ����
	AddImage("marin_run_7_4", L"./Resources/marin/Run7/marine0193.bmp"); // ����
	AddImage("marin_run_7_5", L"./Resources/marin/Run7/marine0195.bmp"); // ����
	AddImage("marin_run_7_6", L"./Resources/marin/Run7/marine0197.bmp"); // ����
	AddImage("marin_run_7_7", L"./Resources/marin/Run7/marine0199.bmp"); // ����
	AddImage("marin_run_7_8", L"./Resources/marin/Run7/marine0201.bmp"); // ����
	AddImage("marin_run_7_9", L"./Resources/marin/Run7/marine0203.bmp"); // ����

	AddImage("marin_run_8_1", L"./Resources/marin/Run8/marine0204.bmp"); // ����
	AddImage("marin_run_8_2", L"./Resources/marin/Run8/marine0206.bmp"); // ����
	AddImage("marin_run_8_3", L"./Resources/marin/Run8/marine0208.bmp"); // ����
	AddImage("marin_run_8_4", L"./Resources/marin/Run8/marine0210.bmp"); // ����
	AddImage("marin_run_8_5", L"./Resources/marin/Run8/marine0212.bmp"); // ����
	AddImage("marin_run_8_6", L"./Resources/marin/Run8/marine0214.bmp"); // ����
	AddImage("marin_run_8_7", L"./Resources/marin/Run8/marine0216.bmp"); // ����
	AddImage("marin_run_8_8", L"./Resources/marin/Run8/marine0218.bmp"); // ����
	AddImage("marin_run_8_9", L"./Resources/marin/Run8/marine0220.bmp"); // ����

	AddImage("marin_death_1", L"./Resources/marin/Death/marine0221.bmp"); // ����
	AddImage("marin_death_2", L"./Resources/marin/Death/marine0222.bmp"); // ����
	AddImage("marin_death_3", L"./Resources/marin/Death/marine0223.bmp"); // ����
	AddImage("marin_death_4", L"./Resources/marin/Death/marine0224.bmp"); // ����
	AddImage("marin_death_5", L"./Resources/marin/Death/marine0225.bmp"); // ����
	AddImage("marin_death_6", L"./Resources/marin/Death/marine0226.bmp"); // ����
	AddImage("marin_death_7", L"./Resources/marin/Death/marine0227.bmp"); // ����
	AddImage("marin_death_8", L"./Resources/marin/Death/marine0228.bmp"); // ����

	AddImage("tcmdbtns0002", L"./Resources/UI2/tcmdbtns0002.bmp"); // ����
	AddImage("tcmdbtns0003", L"./Resources/UI2/tcmdbtns0003.bmp"); // ����
	AddImage("tcmdbtns0004", L"./Resources/UI2/tcmdbtns0004.bmp"); // ����
	AddImage("tcmdbtns0007", L"./Resources/UI2/tcmdbtns0007.bmp"); // ����
	AddImage("tcmdbtns0008", L"./Resources/UI2/tcmdbtns0008.bmp"); // ����
	AddImage("tcmdbtns0009", L"./Resources/UI2/tcmdbtns0009.bmp"); // ����
	AddImage("tcmdbtns0010", L"./Resources/UI2/tcmdbtns0010.bmp"); // ����
	AddImage("tcmdbtns0011", L"./Resources/UI2/tcmdbtns0011.bmp"); // ����
	AddImage("tcmdbtns0017", L"./Resources/UI2/tcmdbtns0017.bmp"); // ����

	AddImage("Coll", L"./Resources/UI2/Coll.bmp"); // ����
	AddImage("coolTimeBar", L"./Resources/UI2/coolTimeBar.bmp"); // ����


	//�Ĺ�

	AddImage("firebat_attack_1_1", L"./Resources/firebat/fire1/firebat0000.bmp"); // ���� ���� ������ 1
	AddImage("firebat_attack_1_2", L"./Resources/firebat/fire1/firebat0002.bmp");
	AddImage("firebat_attack_1_3", L"./Resources/firebat/fire1/firebat0004.bmp");
	AddImage("firebat_attack_1_4", L"./Resources/firebat/fire1/firebat0006.bmp");
	AddImage("firebat_attack_1_5", L"./Resources/firebat/fire1/firebat0008.bmp");
	AddImage("firebat_attack_1_6", L"./Resources/firebat/fire1/firebat0010.bmp");
	AddImage("firebat_attack_1_7", L"./Resources/firebat/fire1/firebat0012.bmp");
	AddImage("firebat_attack_1_8", L"./Resources/firebat/fire1/firebat0014.bmp");
	AddImage("firebat_attack_1_9", L"./Resources/firebat/fire1/firebat0016.bmp");

	AddImage("firebat_attack_2_1", L"./Resources/firebat/fire2/firebat0017.bmp"); // ���� ���������� 2
	AddImage("firebat_attack_2_2", L"./Resources/firebat/fire2/firebat0019.bmp");
	AddImage("firebat_attack_2_3", L"./Resources/firebat/fire2/firebat0021.bmp");
	AddImage("firebat_attack_2_4", L"./Resources/firebat/fire2/firebat0023.bmp");
	AddImage("firebat_attack_2_5", L"./Resources/firebat/fire2/firebat0025.bmp");
	AddImage("firebat_attack_2_6", L"./Resources/firebat/fire2/firebat0027.bmp");
	AddImage("firebat_attack_2_7", L"./Resources/firebat/fire2/firebat0029.bmp");
	AddImage("firebat_attack_2_8", L"./Resources/firebat/fire2/firebat0031.bmp");
	AddImage("firebat_attack_2_9", L"./Resources/firebat/fire2/firebat0033.bmp");

	AddImage("firebat_idle_1", L"./Resources/firebat/Idle/firebat0051.bmp"); // ����
	AddImage("firebat_idle_2", L"./Resources/firebat/Idle/firebat0053.bmp"); // ����
	AddImage("firebat_idle_3", L"./Resources/firebat/Idle/firebat0055.bmp"); // ����
	AddImage("firebat_idle_4", L"./Resources/firebat/Idle/firebat0057.bmp"); // ����
	AddImage("firebat_idle_5", L"./Resources/firebat/Idle/firebat0059.bmp"); // ����
	AddImage("firebat_idle_6", L"./Resources/firebat/Idle/firebat0061.bmp"); // ����
	AddImage("firebat_idle_7", L"./Resources/firebat/Idle/firebat0063.bmp"); // ����
	AddImage("firebat_idle_8", L"./Resources/firebat/Idle/firebat0065.bmp"); // ����
	AddImage("firebat_idle_9", L"./Resources/firebat/Idle/firebat0067.bmp"); // ����


	AddImage("firebat_run_1_1", L"./Resources/firebat/run/firebat0034.bmp"); // ����
	AddImage("firebat_run_1_2", L"./Resources/firebat/run/firebat0036.bmp"); // ����
	AddImage("firebat_run_1_3", L"./Resources/firebat/run/firebat0038.bmp"); // ����
	AddImage("firebat_run_1_4", L"./Resources/firebat/run/firebat0040.bmp"); // ����
	AddImage("firebat_run_1_5", L"./Resources/firebat/run/firebat0042.bmp"); // ����
	AddImage("firebat_run_1_6", L"./Resources/firebat/run/firebat0044.bmp"); // ����
	AddImage("firebat_run_1_7", L"./Resources/firebat/run/firebat0046.bmp"); // ����
	AddImage("firebat_run_1_8", L"./Resources/firebat/run/firebat0048.bmp"); // ����
	AddImage("firebat_run_1_9", L"./Resources/firebat/run/firebat0050.bmp"); // ����

	AddImage("firebat_run_2_1", L"./Resources/firebat/run2/firebat0068.bmp"); // ����
	AddImage("firebat_run_2_2", L"./Resources/firebat/run2/firebat0070.bmp"); // ����
	AddImage("firebat_run_2_3", L"./Resources/firebat/run2/firebat0072.bmp"); // ����
	AddImage("firebat_run_2_4", L"./Resources/firebat/run2/firebat0074.bmp"); // ����
	AddImage("firebat_run_2_5", L"./Resources/firebat/run2/firebat0076.bmp"); // ����
	AddImage("firebat_run_2_6", L"./Resources/firebat/run2/firebat0078.bmp"); // ����
	AddImage("firebat_run_2_7", L"./Resources/firebat/run2/firebat0080.bmp"); // ����
	AddImage("firebat_run_2_8", L"./Resources/firebat/run2/firebat0082.bmp"); // ����
	AddImage("firebat_run_2_9", L"./Resources/firebat/run2/firebat0084.bmp"); // ����

	AddImage("firebat_run_3_1", L"./Resources/firebat/run3/firebat0085.bmp"); // ����
	AddImage("firebat_run_3_2", L"./Resources/firebat/run3/firebat0087.bmp"); // ����
	AddImage("firebat_run_3_3", L"./Resources/firebat/run3/firebat0089.bmp"); // ����
	AddImage("firebat_run_3_4", L"./Resources/firebat/run3/firebat0091.bmp"); // ����
	AddImage("firebat_run_3_5", L"./Resources/firebat/run3/firebat0093.bmp"); // ����
	AddImage("firebat_run_3_6", L"./Resources/firebat/run3/firebat0095.bmp"); // ����
	AddImage("firebat_run_3_7", L"./Resources/firebat/run3/firebat0097.bmp"); // ����
	AddImage("firebat_run_3_8", L"./Resources/firebat/run3/firebat0099.bmp"); // ����
	AddImage("firebat_run_3_9", L"./Resources/firebat/run3/firebat0101.bmp"); // ����

	AddImage("firebat_run_4_1", L"./Resources/firebat/run4/firebat0102.bmp"); // ����
	AddImage("firebat_run_4_2", L"./Resources/firebat/run4/firebat0104.bmp"); // ����
	AddImage("firebat_run_4_3", L"./Resources/firebat/run4/firebat0106.bmp"); // ����
	AddImage("firebat_run_4_4", L"./Resources/firebat/run4/firebat0108.bmp"); // ����
	AddImage("firebat_run_4_5", L"./Resources/firebat/run4/firebat0110.bmp"); // ����
	AddImage("firebat_run_4_6", L"./Resources/firebat/run4/firebat0112.bmp"); // ����
	AddImage("firebat_run_4_7", L"./Resources/firebat/run4/firebat0114.bmp"); // ����
	AddImage("firebat_run_4_8", L"./Resources/firebat/run4/firebat0116.bmp"); // ����
	AddImage("firebat_run_4_9", L"./Resources/firebat/run4/firebat0118.bmp"); // ����

	AddImage("firebat_run_5_1", L"./Resources/firebat/run5/firebat0119.bmp"); // ����
	AddImage("firebat_run_5_2", L"./Resources/firebat/run5/firebat0121.bmp"); // ����
	AddImage("firebat_run_5_3", L"./Resources/firebat/run5/firebat0123.bmp"); // ����
	AddImage("firebat_run_5_4", L"./Resources/firebat/run5/firebat0125.bmp"); // ����
	AddImage("firebat_run_5_5", L"./Resources/firebat/run5/firebat0127.bmp"); // ����
	AddImage("firebat_run_5_6", L"./Resources/firebat/run5/firebat0129.bmp"); // ����
	AddImage("firebat_run_5_7", L"./Resources/firebat/run5/firebat0131.bmp"); // ����
	AddImage("firebat_run_5_8", L"./Resources/firebat/run5/firebat0133.bmp"); // ����
	AddImage("firebat_run_5_9", L"./Resources/firebat/run5/firebat0135.bmp"); // ����

	AddImage("firebat_run_6_1", L"./Resources/firebat/run6/firebat0136.bmp"); // ����
	AddImage("firebat_run_6_2", L"./Resources/firebat/run6/firebat0138.bmp"); // ����
	AddImage("firebat_run_6_3", L"./Resources/firebat/run6/firebat0140.bmp"); // ����
	AddImage("firebat_run_6_4", L"./Resources/firebat/run6/firebat0142.bmp"); // ����
	AddImage("firebat_run_6_5", L"./Resources/firebat/run6/firebat0144.bmp"); // ����
	AddImage("firebat_run_6_6", L"./Resources/firebat/run6/firebat0146.bmp"); // ����
	AddImage("firebat_run_6_7", L"./Resources/firebat/run6/firebat0148.bmp"); // ����
	AddImage("firebat_run_6_8", L"./Resources/firebat/run6/firebat0150.bmp"); // ����
	AddImage("firebat_run_6_9", L"./Resources/firebat/run6/firebat0152.bmp"); // ����

	AddImage("firebat_run_7_1", L"./Resources/firebat/run7/firebat0153.bmp"); // ����
	AddImage("firebat_run_7_2", L"./Resources/firebat/run7/firebat0155.bmp"); // ����
	AddImage("firebat_run_7_3", L"./Resources/firebat/run7/firebat0157.bmp"); // ����
	AddImage("firebat_run_7_4", L"./Resources/firebat/run7/firebat0159.bmp"); // ����
	AddImage("firebat_run_7_5", L"./Resources/firebat/run7/firebat0161.bmp"); // ����
	AddImage("firebat_run_7_6", L"./Resources/firebat/run7/firebat0163.bmp"); // ����
	AddImage("firebat_run_7_7", L"./Resources/firebat/run7/firebat0165.bmp"); // ����
	AddImage("firebat_run_7_8", L"./Resources/firebat/run7/firebat0167.bmp"); // ����
	AddImage("firebat_run_7_9", L"./Resources/firebat/run7/firebat0169.bmp"); // ����

	AddImage("gas", L"./Resources/ResourceIcon/gas.bmp"); // ����
	AddImage("min", L"./Resources/ResourceIcon/min.bmp"); // ����
	AddImage("suf", L"./Resources/ResourceIcon/suf.bmp"); // ����

	// ����
	AddImage("Vulture_1", L"./Resources/Vulture/vulture0000.bmp"); // ����
	AddImage("Vulture_2", L"./Resources/Vulture/vulture0002.bmp"); // ����
	AddImage("Vulture_3", L"./Resources/Vulture/vulture0004.bmp"); // ����
	AddImage("Vulture_4", L"./Resources/Vulture/vulture0006.bmp"); // ����
	AddImage("Vulture_5", L"./Resources/Vulture/vulture0008.bmp"); // ����
	AddImage("Vulture_6", L"./Resources/Vulture/vulture0010.bmp"); // ����
	AddImage("Vulture_7", L"./Resources/Vulture/vulture0012.bmp"); // ����
	AddImage("Vulture_8", L"./Resources/Vulture/vulture0014.bmp"); // ����
	AddImage("Vulture_9", L"./Resources/Vulture/vulture0016.bmp"); // ����

	AddImage("tank0000", L"./Resources/tank/tank0000.bmp"); // ��ũ �Ϲ�
	AddImage("tank0002", L"./Resources/tank/tank0002.bmp"); // ��ũ �Ϲ�
	AddImage("tank0004", L"./Resources/tank/tank0004.bmp"); // ��ũ �Ϲ�
	AddImage("tank0006", L"./Resources/tank/tank0006.bmp"); // ��ũ �Ϲ�
	AddImage("tank0008", L"./Resources/tank/tank0008.bmp"); // ��ũ �Ϲ�
	AddImage("tank0010", L"./Resources/tank/tank0010.bmp"); // ��ũ �Ϲ�
	AddImage("tank0012", L"./Resources/tank/tank0012.bmp"); // ��ũ �Ϲ�
	AddImage("tank0014", L"./Resources/tank/tank0014.bmp"); // ��ũ �Ϲ�
	AddImage("tank0016", L"./Resources/tank/tank0016.bmp"); // ��ũ �Ϲ�
	// ��ũ �Ϲ�
	AddImage("tank0018", L"./Resources/tank/tank0018.bmp"); // ��ũ �Ϲ�
	AddImage("tank0020", L"./Resources/tank/tank0020.bmp"); // ��ũ �Ϲ�
	AddImage("tank0022", L"./Resources/tank/tank0022.bmp"); // ��ũ �Ϲ�
	AddImage("tank0024", L"./Resources/tank/tank0024.bmp"); // ��ũ �Ϲ�
	AddImage("tank0026", L"./Resources/tank/tank0026.bmp"); // ��ũ �Ϲ�
	AddImage("tank0028", L"./Resources/tank/tank0028.bmp"); // ��ũ �Ϲ�
	AddImage("tank0030", L"./Resources/tank/tank0030.bmp"); // ��ũ �Ϲ�
	AddImage("tank0032", L"./Resources/tank/tank0032.bmp"); // ��ũ �Ϲ�
	AddImage("tank0033", L"./Resources/tank/tank0033.bmp"); // ��ũ �Ϲ�
	// ��ũ �Ϲ�
	AddImage("tank0035", L"./Resources/tank/tank0035.bmp"); // ��ũ �Ϲ�
	AddImage("tank0037", L"./Resources/tank/tank0037.bmp"); // ��ũ �Ϲ�
	AddImage("tank0039", L"./Resources/tank/tank0039.bmp"); // ��ũ �Ϲ�
	AddImage("tank0041", L"./Resources/tank/tank0041.bmp"); // ��ũ �Ϲ�
	AddImage("tank0043", L"./Resources/tank/tank0043.bmp"); // ��ũ �Ϲ�
	AddImage("tank0045", L"./Resources/tank/tank0045.bmp"); // ��ũ �Ϲ�
	AddImage("tank0047", L"./Resources/tank/tank0047.bmp"); // ��ũ �Ϲ�
	AddImage("tank0049", L"./Resources/tank/tank0049.bmp"); // ��ũ �Ϲ�
	AddImage("tank0050", L"./Resources/tank/tank0050.bmp"); // ��ũ �Ϲ�

	// �׸���
	AddImage("tank0000s", L"./Resources/tank/ttashad0000.bmp"); // ��ũ �Ϲ�
	AddImage("tank0002s", L"./Resources/tank/ttashad0002.bmp"); // ��ũ �Ϲ�
	AddImage("tank0004s", L"./Resources/tank/ttashad0004.bmp"); // ��ũ �Ϲ�
	AddImage("tank0006s", L"./Resources/tank/ttashad0006.bmp"); // ��ũ �Ϲ�
	AddImage("tank0008s", L"./Resources/tank/ttashad0008.bmp"); // ��ũ �Ϲ�
	AddImage("tank0010s", L"./Resources/tank/ttashad0010.bmp"); // ��ũ �Ϲ�
	AddImage("tank0012s", L"./Resources/tank/ttashad0012.bmp"); // ��ũ �Ϲ�
	AddImage("tank0014s", L"./Resources/tank/ttashad0014.bmp"); // ��ũ �Ϲ�
	AddImage("tank0016s", L"./Resources/tank/ttashad0016.bmp"); // ��ũ �Ϲ�
	// ��ũ �Ϲ�
	AddImage("tank0018s", L"./Resources/tank/ttashad0018.bmp"); // ��ũ �Ϲ�
	AddImage("tank0020s", L"./Resources/tank/ttashad0020.bmp"); // ��ũ �Ϲ�
	AddImage("tank0022s", L"./Resources/tank/ttashad0022.bmp"); // ��ũ �Ϲ�
	AddImage("tank0024s", L"./Resources/tank/ttashad0024.bmp"); // ��ũ �Ϲ�
	AddImage("tank0026s", L"./Resources/tank/ttashad0026.bmp"); // ��ũ �Ϲ�
	AddImage("tank0028s", L"./Resources/tank/ttashad0028.bmp"); // ��ũ �Ϲ�
	AddImage("tank0030s", L"./Resources/tank/ttashad0030.bmp"); // ��ũ �Ϲ�
	AddImage("tank0032s", L"./Resources/tank/ttashad0032.bmp"); // ��ũ �Ϲ�
	AddImage("tank0033s", L"./Resources/tank/ttashad0033.bmp"); // ��ũ �Ϲ�
	// ��ũ �Ϲ�
	AddImage("tank0035s", L"./Resources/tank/ttashad0035.bmp"); // ��ũ �Ϲ�
	AddImage("tank0037s", L"./Resources/tank/ttashad0037.bmp"); // ��ũ �Ϲ�
	AddImage("tank0039s", L"./Resources/tank/ttashad0039.bmp"); // ��ũ �Ϲ�
	AddImage("tank0041s", L"./Resources/tank/ttashad0041.bmp"); // ��ũ �Ϲ�
	AddImage("tank0043s", L"./Resources/tank/ttashad0043.bmp"); // ��ũ �Ϲ�
	AddImage("tank0045s", L"./Resources/tank/ttashad0045.bmp"); // ��ũ �Ϲ�
	AddImage("tank0047s", L"./Resources/tank/ttashad0047.bmp"); // ��ũ �Ϲ�
	AddImage("tank0049s", L"./Resources/tank/ttashad0049.bmp"); // ��ũ �Ϲ�
	AddImage("tank0050s", L"./Resources/tank/ttashad0050.bmp"); // ��ũ �Ϲ�

	AddImage("tankt0000", L"./Resources/tankT/tankt0000.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0002", L"./Resources/tankT/tankt0002.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0004", L"./Resources/tankT/tankt0004.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0006", L"./Resources/tankT/tankt0006.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0008", L"./Resources/tankT/tankt0008.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0010", L"./Resources/tankT/tankt0010.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0012", L"./Resources/tankT/tankt0012.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0014", L"./Resources/tankT/tankt0014.bmp"); // ��ũ �Ϲ� �Ӹ�
	AddImage("tankt0016", L"./Resources/tankT/tankt0016.bmp"); // ��ũ �Ϲ� �Ӹ�

	AddImage("stankt0000", L"./Resources/sTankT/stankt0000.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0002", L"./Resources/sTankT/stankt0002.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0004", L"./Resources/sTankT/stankt0004.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0006", L"./Resources/sTankT/stankt0006.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0008", L"./Resources/sTankT/stankt0008.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0010", L"./Resources/sTankT/stankt0010.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0012", L"./Resources/sTankT/stankt0012.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0014", L"./Resources/sTankT/stankt0014.bmp"); // ������ũ �Ӹ�
	AddImage("stankt0016", L"./Resources/sTankT/stankt0016.bmp"); // ������ũ �Ӹ�
	AddImage("sTank", L"./Resources/sTank/stank0005.bmp"); // ������ũ ��

	AddImageVector("sTankT", L"./Resources/sTankT/stankt00", 17, 99);// ������ũ �ϴ��� �Ӹ�
	AddImageVector("sTank", L"./Resources/sTank/stank00", 0, 5); // ������ũ �ϴ��� ��

	AddWireImage("wirefram0000", "./Resources/Icon2/wirefram0000.bmp");
	AddWireImage("wirefram0002", "./Resources/Icon2/wirefram0002.bmp");
	AddWireImage("wirefram0005", "./Resources/Icon2/wirefram0005.bmp");
	AddWireImage("wirefram0007", "./Resources/Icon2/wirefram0007.bmp");
	AddWireImage("wirefram0106", "./Resources/Icon2/wirefram0106.bmp");
	AddWireImage("wirefram0113", "./Resources/Icon2/wirefram0113.bmp");
	AddWireImage("wirefram0111", "./Resources/Icon2/wirefram0111.bmp");
	AddWireImage("wirefram0112", "./Resources/Icon2/wirefram0112.bmp");
	AddWireImage("wirefram0120", "./Resources/Icon2/wirefram0120.bmp");
	AddWireImage("wirefram0123", "./Resources/Icon2/wirefram0123.bmp");
	AddWireImage("wirefram0109", "./Resources/Icon2/wirefram0109.bmp");

	AddWireImage("grpwire0000", "./Resources/Icon3/grpwire0000.bmp");
	AddWireImage("grpwire0002", "./Resources/Icon3/grpwire0002.bmp");
	AddWireImage("grpwire0005", "./Resources/Icon3/grpwire0005.bmp");
	AddWireImage("grpwire0007", "./Resources/Icon3/grpwire0007.bmp");
	AddWireImage("grpwire0106", "./Resources/Icon3/grpwire0106.bmp");
	AddWireImage("grpwire0113", "./Resources/Icon3/grpwire0113.bmp");
	AddWireImage("grpwire0111", "./Resources/Icon3/grpwire0111.bmp");
	AddWireImage("grpwire0112", "./Resources/Icon3/grpwire0112.bmp");
	AddWireImage("grpwire0120", "./Resources/Icon3/grpwire0120.bmp");
	AddWireImage("grpwire0123", "./Resources/Icon3/grpwire0123.bmp");
	AddWireImage("grpwire0109", "./Resources/Icon3/grpwire0109.bmp");
}
