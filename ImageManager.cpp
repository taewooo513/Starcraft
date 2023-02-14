#include "stdafx.h"
#include "ImageManager.h"
#include "MapReader.h"
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
		L"Txt",                  // 폰트 패밀리 이름의 문자열
		NULL,                        // 폰트 컬렉션 객체, NULL=시스템 폰트 컬렉션
		DWRITE_FONT_WEIGHT_NORMAL,   // 폰트 굵기. LIGHT, NORMAL, BOLD 등.
		DWRITE_FONT_STYLE_NORMAL,    // 폰트 스타일. NORMAL, OBLIQUE, ITALIC.
		DWRITE_FONT_STRETCH_NORMAL,  // 폰트 간격. CONDENSED, NORMAL, MEDIUM, EXTEXDED 등.
		1,                          // 폰트 크기.
		L"Txt",                         // 로케일을 문자열로 명시.  영어-미국=L"en-us", 한국어-한국=L"ko-kr"
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

void ImageManager::UICenterRenderBlendBlack2(CImage* img, Vector2 vec, Vector2 scale, float rot, bool isReverse)
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
	if (startPos.x >= WINSIZE_X || startPos.x <= -30 || startPos.y >= WINSIZE_Y || startPos.y <= -30)
	{
		return;
	}
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
	m_d2dContext->CreateSolidColorBrush({ 255,0,0,255 }, &brush);
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

void ImageManager::FogRender()
{
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			if (GRIDMANAGER->regionsTile[j][i].fogTag == 0)
			{
				DrawRect({ (float)j * 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().x,(float)i * 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().y },
					{ (float)j * 8 * 1.5f + 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().x + 1, (float)i * 8 * 1.5f + 8 * 1.5f - IMAGEMANAGER->GetCameraPosition().y + 1 }, { 0,0,0,1 }, 1);
			}
		}
	}
}

void ImageManager::FogUpdate(Vector2 pos, float dest)
{
	Vector2 i = pos / 1.5 / 8;

	for (int x = i.x - dest; x < i.x + dest; x++)
	{
		for (int y = i.y - dest; y < i.y + dest; y++)
		{
			if (sqrt((x - i.x) * (x - i.x) + (y - i.y) * (y - i.y)) < dest)
			{
				GRIDMANAGER->regionsTile[x][y].fogTag = 1;
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
	cout << "이미지 key 없음 : " << key << "\n";
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
	cout << "애니메이션 key 없음 : " << key << "\n";
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
	// 미네랄


	AddImage("cmdicons0002", L"./Resources/Icon/cmdicons0002.bmp"); //SCVcmdicons0007 벌처
	AddImage("cmdicons0013", L"./Resources/Icon/cmdicons0013.bmp"); //SCVcmdicons0007cmdicons0013 마인
	AddImage("cmdicons0023", L"./Resources/Icon/cmdicons0023.bmp"); //탱크 
	AddImage("cmdicons0120", L"./Resources/Icon/cmdicons0120.bmp"); //탱크 cmdicons0120

	AddImage("cmdicons0023", L"./Resources/Icon/cmdicons0023.bmp"); //SCVcmdicons0007
	AddImage("cmdicons0005", L"./Resources/Icon/cmdicons0005.bmp"); //SCVcmdicons0007
	AddImage("cmdicons0003", L"./Resources/Icon/cmdicons0003.bmp"); //SCVcmdicons0007
	AddImage("cmdicons0002", L"./Resources/Icon/cmdicons0002.bmp"); //SCVcmdicons0007

// SCV
	AddImage("wirefram0106", L"./Resources/Icon2/wirefram0106.bmp"); //SCVcmdicons0007
	AddImage("wirefram0000", L"./Resources/Icon2/wirefram0000.bmp"); //SCVcmdicons0007
	AddImage("wirefram0007", L"./Resources/Icon2/wirefram0007.bmp"); //wirefram0010
	AddImage("wirefram0010", L"./Resources/Icon2/wirefram0010.bmp"); //wirefram0010
	AddImage("wirefram0002", L"./Resources/Icon2/wirefram0002.bmp"); //벌쳐grpwire0002

	AddImage("cmdicons0007", L"./Resources/Icon/cmdicons0007.bmp"); //SCVcmdicons0007
	AddImage("grpwire0000", L"./Resources/Icon3/grpwire0000.bmp"); //SCVcmdicons0007
	AddImage("grpwire0010", L"./Resources/Icon3/grpwire0010.bmp"); //SCVcmdicons0007
	AddImage("grpwire0002", L"./Resources/Icon3/grpwire0002.bmp"); //벌쳐


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

	// SCV 아이콘
	AddImage("cmdicons0234", L"./Resources/Icon/cmdicons0234.bmp");
	AddImage("cmdicons0235", L"./Resources/Icon/cmdicons0235.bmp");
	AddImage("cmdicons0236", L"./Resources/Icon/cmdicons0236.bmp");
	AddImage("cmdicons0231", L"./Resources/Icon/cmdicons0231.bmp");
	AddImage("cmdicons0232", L"./Resources/Icon/cmdicons0232.bmp");


	AddImage("cmdicons0230", L"./Resources/Icon/cmdicons0230.bmp"); // attack 
	AddImage("cmdicons0229", L"./Resources/Icon/cmdicons0229.bmp"); // stop
	AddImage("cmdicons0228", L"./Resources/Icon/cmdicons0228.bmp"); // move

	// 커멘드 센터 
	AddImage("control0000", L"./Resources/Bulid/CommandCenter/control0000.bmp"); // 일반상태 
	AddImage("control0001", L"./Resources/Bulid/CommandCenter/control0001.bmp"); // 완성전단계
	AddImage("control0002", L"./Resources/Bulid/CommandCenter/control0002.bmp"); // Up 1
	AddImage("control0003", L"./Resources/Bulid/CommandCenter/control0003.bmp"); // Up 2
	AddImage("control0004", L"./Resources/Bulid/CommandCenter/control0004.bmp"); // Up 3
	AddImage("controlt", L"./Resources/Bulid/CommandCenter/controlt.bmp"); // 유닛 생성중


	// 베럭 짓는중
	AddImage("tbldlrg0000", L"./Resources/Bulid/Buliding/tbldlrg0000.bmp"); // move
	AddImage("tbldlrg0001", L"./Resources/Bulid/Buliding/tbldlrg0001.bmp"); // move
	AddImage("tbldlrg0002", L"./Resources/Bulid/Buliding/tbldlrg0002.bmp"); // move
	AddImage("grpwire0106", L"./Resources/Icon3/grpwire0106.bmp");

	//베럭 이미지 
	AddImage("cmdicons0111", L"./Resources/Icon/cmdicons0111.bmp"); // 아이콘
	AddImage("tbarrack0000", L"./Resources/Bulid/Barrack/tbarrack0000.bmp"); // 평상시
	AddImage("tbarrack0001", L"./Resources/Bulid/Barrack/tbarrack0001.bmp"); // 완성전
	AddImage("tbarrack0002", L"./Resources/Bulid/Barrack/tbarrack0002.bmp"); // 공중에 뜨기전
	AddImage("tbarrack0003", L"./Resources/Bulid/Barrack/tbarrack0003.bmp"); // 공중에 뜨기전
	AddImage("tbarrack0004", L"./Resources/Bulid/Barrack/tbarrack0004.bmp"); // 공중에 뜨기전
	AddImage("tbarrack0007", L"./Resources/Bulid/Barrack/tbarrack0007.bmp"); // 유닛 생성중
	AddImage("tbarrack0008", L"./Resources/Bulid/Barrack/tbarrack0008.bmp"); // 유닛 생성중

	AddImage("tcmdbtns0000", L"./Resources/UI2/tcmdbtns0000.bmp");

	AddImage("targg0001", L"./Resources/cursor/targg0001.bmp"); // 클릭시 위치나오는거

	AddImage("wirefram0106", L"./Resources/Icon2/wirefram0106.bmp"); //SCVcmdicons0007
	AddImage("grpwire0007", L"./Resources/Icon3/grpwire0007.bmp"); // SCV 부대지정 아이콘

	AddImage("cmdicons0106", L"./Resources/Icon/cmdicons0106.bmp"); // 커멘드센터
	AddImage("cmdicons0109", L"./Resources/Icon/cmdicons0109.bmp"); // 서플라이 디포트
	AddImage("cmdicons0110", L"./Resources/Icon/cmdicons0110.bmp"); // 가스

	AddImage("cmdicons0112", L"./Resources/Icon/cmdicons0112.bmp"); // 아카데미
	AddImage("factory0000", L"./Resources/Bulid/Factory/factory0000.bmp"); // 기본
	AddImage("factory0001", L"./Resources/Bulid/Factory/factory0001.bmp"); // 완성 전
	AddImage("factory0002", L"./Resources/Bulid/Factory/factory0002.bmp"); // 띄우기 1
	AddImage("factory0003", L"./Resources/Bulid/Factory/factory0003.bmp"); // 띄우기 2
	AddImage("factory0004", L"./Resources/Bulid/Factory/factory0004.bmp"); // 띄우기 3
	AddImage("factory0005", L"./Resources/Bulid/Factory/factory0005.bmp"); // 띄우기 4

	AddImage("cmdicons0113", L"./Resources/Icon/cmdicons0113.bmp"); // 펙토리
	AddImage("cmdicons0114", L"./Resources/Icon/cmdicons0114.bmp"); // 스타포트
	AddImage("cmdicons0122", L"./Resources/Icon/cmdicons0122.bmp"); // 엔지니어링
	AddImage("cmdicons0124", L"./Resources/Icon/cmdicons0124.bmp"); // 터렛
	AddImage("cmdicons0125", L"./Resources/Icon/cmdicons0125.bmp"); // 벙커

	// 마린 이미지 
	AddImage("cmdicons0000", L"./Resources/Icon/cmdicons0000.bmp"); // 마린 아이콘 

	// 메딕 이미지 메딕없음
	//AddImage("cmdicons0000", L"./Resources/UI/cmdicons0000.bmp"); // 마린 아이콘 

	// 파이어벳 이미지
	AddImage("cmdicons0010", L"./Resources/Icon/cmdicons0010.bmp"); // 파이어벳 아이콘 

	// 고스트 이미지
	AddImage("cmdicons0001", L"./Resources/Icon/cmdicons0001.bmp"); // 고스트 아이콘 

	AddImage("cmdicons0286", L"./Resources/Icon/cmdicons0286.bmp"); // 몹생성 어디로 생성할지

	AddImage("cmdicons0282", L"./Resources/Icon/cmdicons0282.bmp"); // 건물 띄우기
	AddImage("cmdicons0283", L"./Resources/Icon/cmdicons0283.bmp"); // 건물 내리기

	AddImage("cmdicons0236", L"./Resources/Icon/cmdicons0236.bmp"); // 캔슬

	// 이펙트
	AddImageVector("f2", L"./Resources/f2/tbangx00", 0, 13); // 건물 폭발

	AddImageVector("SCVEffect", L"./Resources/SCVEffect/scvspark00", 0, 9); // 건물 폭발

	// 마우스 커서
	AddImageVector("arrow0000", L"./Resources/cursor/arrow00", 0, 4); // 일반
	AddImage("drag0000", L"./Resources/cursor/drag0000.bmp"); // 드래그

	// 미네랄
	AddImage("min010000", L"./Resources/min/min010000.bmp"); // 미네랄 큰거
	AddImage("min010001", L"./Resources/min/min010001.bmp"); // 미네랄 중간
	AddImage("min010002", L"./Resources/min/min010002.bmp"); // 미네랄 덜중간
	AddImage("min010003", L"./Resources/min/min010003.bmp"); // 미네랄 작음

	AddImage("mul-tail", L"./Resources/lobby/Multi Player/mul-tail.png"); // 미네랄 작음

	AddImageVector("Editor", L"./Resources/lobby/Editor/editor", 0, 84, true); // 일반
	AddImageVector("Multi Player", L"./Resources/lobby/Multi Player/multi", 0, 49, true); // 일반
	AddImageVector("Exit", L"./Resources/lobby/Exit/exit", 0, 49, true); // 일반
	AddImageVector("Single Player", L"./Resources/lobby/Single Player/single", 0, 34, true); // 일반

	AddImage("marin_attack_1_1", L"./Resources/marin/Attack/marine0034.bmp"); // 마린 공격 프레임 1
	AddImage("marin_attack_1_2", L"./Resources/marin/Attack/marine0036.bmp");
	AddImage("marin_attack_1_3", L"./Resources/marin/Attack/marine0038.bmp");
	AddImage("marin_attack_1_4", L"./Resources/marin/Attack/marine0040.bmp");
	AddImage("marin_attack_1_5", L"./Resources/marin/Attack/marine0042.bmp");
	AddImage("marin_attack_1_6", L"./Resources/marin/Attack/marine0044.bmp");
	AddImage("marin_attack_1_7", L"./Resources/marin/Attack/marine0046.bmp");
	AddImage("marin_attack_1_8", L"./Resources/marin/Attack/marine0048.bmp");
	AddImage("marin_attack_1_9", L"./Resources/marin/Attack/marine0050.bmp");

	AddImage("marin_attack_2_1", L"./Resources/marin/Attack/marine0051.bmp"); // 마린 공격프레임 2
	AddImage("marin_attack_2_2", L"./Resources/marin/Attack/marine0053.bmp");
	AddImage("marin_attack_2_3", L"./Resources/marin/Attack/marine0055.bmp");
	AddImage("marin_attack_2_4", L"./Resources/marin/Attack/marine0057.bmp");
	AddImage("marin_attack_2_5", L"./Resources/marin/Attack/marine0059.bmp");
	AddImage("marin_attack_2_6", L"./Resources/marin/Attack/marine0061.bmp");
	AddImage("marin_attack_2_7", L"./Resources/marin/Attack/marine0063.bmp");
	AddImage("marin_attack_2_8", L"./Resources/marin/Attack/marine0065.bmp");
	AddImage("marin_attack_2_9", L"./Resources/marin/Attack/marine0067.bmp");

	AddImage("marin_idle_1_1", L"./Resources/marin/Idle/marine0068.bmp"); // 마린 기본 모션
	AddImage("marin_idle_1_2", L"./Resources/marin/Idle/marine0070.bmp");
	AddImage("marin_idle_1_3", L"./Resources/marin/Idle/marine0072.bmp");
	AddImage("marin_idle_1_4", L"./Resources/marin/Idle/marine0074.bmp");
	AddImage("marin_idle_1_5", L"./Resources/marin/Idle/marine0076.bmp");
	AddImage("marin_idle_1_6", L"./Resources/marin/Idle/marine0078.bmp");
	AddImage("marin_idle_1_7", L"./Resources/marin/Idle/marine0080.bmp");
	AddImage("marin_idle_1_8", L"./Resources/marin/Idle/marine0082.bmp");
	AddImage("marin_idle_1_9", L"./Resources/marin/Idle/marine0084.bmp");

	AddImage("marin_idle_action_1_1", L"./Resources/marin/Idle2/marine0000.bmp"); // 마린 대기 모션 가만히 있을때 2
	AddImage("marin_idle_action_1_2", L"./Resources/marin/Idle2/marine0002.bmp");
	AddImage("marin_idle_action_1_3", L"./Resources/marin/Idle2/marine0004.bmp");
	AddImage("marin_idle_action_1_4", L"./Resources/marin/Idle2/marine0006.bmp");
	AddImage("marin_idle_action_1_5", L"./Resources/marin/Idle2/marine0008.bmp");
	AddImage("marin_idle_action_1_6", L"./Resources/marin/Idle2/marine0010.bmp");
	AddImage("marin_idle_action_1_7", L"./Resources/marin/Idle2/marine0012.bmp");
	AddImage("marin_idle_action_1_8", L"./Resources/marin/Idle2/marine0014.bmp");
	AddImage("marin_idle_action_1_9", L"./Resources/marin/Idle2/marine0016.bmp");

	AddImage("marin_idle_action_2_1", L"./Resources/marin/Idle2/marine0018.bmp"); // 마린 대기 모션 가만히 있을때 2
	AddImage("marin_idle_action_2_2", L"./Resources/marin/Idle2/marine0020.bmp");
	AddImage("marin_idle_action_2_3", L"./Resources/marin/Idle2/marine0022.bmp");
	AddImage("marin_idle_action_2_4", L"./Resources/marin/Idle2/marine0024.bmp");
	AddImage("marin_idle_action_2_5", L"./Resources/marin/Idle2/marine0026.bmp");
	AddImage("marin_idle_action_2_6", L"./Resources/marin/Idle2/marine0028.bmp");
	AddImage("marin_idle_action_2_7", L"./Resources/marin/Idle2/marine0030.bmp");
	AddImage("marin_idle_action_2_8", L"./Resources/marin/Idle2/marine0032.bmp");
	AddImage("marin_idle_action_2_9", L"./Resources/marin/Idle2/marine0034.bmp");

	AddImage("marin_run_1_1", L"./Resources/marin/Run1/marine0085.bmp"); // 마린
	AddImage("marin_run_1_2", L"./Resources/marin/Run1/marine0087.bmp"); // 마린
	AddImage("marin_run_1_3", L"./Resources/marin/Run1/marine0089.bmp"); // 마린
	AddImage("marin_run_1_4", L"./Resources/marin/Run1/marine0091.bmp"); // 마린
	AddImage("marin_run_1_5", L"./Resources/marin/Run1/marine0093.bmp"); // 마린
	AddImage("marin_run_1_6", L"./Resources/marin/Run1/marine0095.bmp"); // 마린
	AddImage("marin_run_1_7", L"./Resources/marin/Run1/marine0097.bmp"); // 마린
	AddImage("marin_run_1_8", L"./Resources/marin/Run1/marine0099.bmp"); // 마린
	AddImage("marin_run_1_9", L"./Resources/marin/Run1/marine0101.bmp"); // 마린

	AddImage("marin_run_2_1", L"./Resources/marin/Run2/marine0102.bmp"); // 마린
	AddImage("marin_run_2_2", L"./Resources/marin/Run2/marine0104.bmp"); // 마린
	AddImage("marin_run_2_3", L"./Resources/marin/Run2/marine0106.bmp"); // 마린
	AddImage("marin_run_2_4", L"./Resources/marin/Run2/marine0108.bmp"); // 마린
	AddImage("marin_run_2_5", L"./Resources/marin/Run2/marine0110.bmp"); // 마린
	AddImage("marin_run_2_6", L"./Resources/marin/Run2/marine0112.bmp"); // 마린
	AddImage("marin_run_2_7", L"./Resources/marin/Run2/marine0114.bmp"); // 마린
	AddImage("marin_run_2_8", L"./Resources/marin/Run2/marine0116.bmp"); // 마린
	AddImage("marin_run_2_9", L"./Resources/marin/Run2/marine0118.bmp"); // 마린

	AddImage("marin_run_3_1", L"./Resources/marin/Run3/marine0119.bmp"); // 마린
	AddImage("marin_run_3_2", L"./Resources/marin/Run3/marine0121.bmp"); // 마린
	AddImage("marin_run_3_3", L"./Resources/marin/Run3/marine0123.bmp"); // 마린
	AddImage("marin_run_3_4", L"./Resources/marin/Run3/marine0125.bmp"); // 마린
	AddImage("marin_run_3_5", L"./Resources/marin/Run3/marine0127.bmp"); // 마린
	AddImage("marin_run_3_6", L"./Resources/marin/Run3/marine0129.bmp"); // 마린
	AddImage("marin_run_3_7", L"./Resources/marin/Run3/marine0131.bmp"); // 마린
	AddImage("marin_run_3_8", L"./Resources/marin/Run3/marine0133.bmp"); // 마린
	AddImage("marin_run_3_9", L"./Resources/marin/Run3/marine0135.bmp"); // 마린

	AddImage("marin_run_4_1", L"./Resources/marin/Run4/marine0136.bmp"); // 마린
	AddImage("marin_run_4_2", L"./Resources/marin/Run4/marine0138.bmp"); // 마린
	AddImage("marin_run_4_3", L"./Resources/marin/Run4/marine0140.bmp"); // 마린
	AddImage("marin_run_4_4", L"./Resources/marin/Run4/marine0142.bmp"); // 마린
	AddImage("marin_run_4_5", L"./Resources/marin/Run4/marine0144.bmp"); // 마린
	AddImage("marin_run_4_6", L"./Resources/marin/Run4/marine0146.bmp"); // 마린
	AddImage("marin_run_4_7", L"./Resources/marin/Run4/marine0148.bmp"); // 마린
	AddImage("marin_run_4_8", L"./Resources/marin/Run4/marine0150.bmp"); // 마린
	AddImage("marin_run_4_9", L"./Resources/marin/Run4/marine0152.bmp"); // 마린

	AddImage("marin_run_5_1", L"./Resources/marin/Run5/marine0153.bmp"); // 마린
	AddImage("marin_run_5_2", L"./Resources/marin/Run5/marine0155.bmp"); // 마린
	AddImage("marin_run_5_3", L"./Resources/marin/Run5/marine0157.bmp"); // 마린
	AddImage("marin_run_5_4", L"./Resources/marin/Run5/marine0159.bmp"); // 마린
	AddImage("marin_run_5_5", L"./Resources/marin/Run5/marine0161.bmp"); // 마린
	AddImage("marin_run_5_6", L"./Resources/marin/Run5/marine0163.bmp"); // 마린
	AddImage("marin_run_5_7", L"./Resources/marin/Run5/marine0165.bmp"); // 마린
	AddImage("marin_run_5_8", L"./Resources/marin/Run5/marine0167.bmp"); // 마린
	AddImage("marin_run_5_9", L"./Resources/marin/Run5/marine0169.bmp"); // 마린

	AddImage("marin_run_6_1", L"./Resources/marin/Run6/marine0170.bmp"); // 마린
	AddImage("marin_run_6_2", L"./Resources/marin/Run6/marine0172.bmp"); // 마린
	AddImage("marin_run_6_3", L"./Resources/marin/Run6/marine0174.bmp"); // 마린
	AddImage("marin_run_6_4", L"./Resources/marin/Run6/marine0176.bmp"); // 마린
	AddImage("marin_run_6_5", L"./Resources/marin/Run6/marine0178.bmp"); // 마린
	AddImage("marin_run_6_6", L"./Resources/marin/Run6/marine0180.bmp"); // 마린
	AddImage("marin_run_6_7", L"./Resources/marin/Run6/marine0182.bmp"); // 마린
	AddImage("marin_run_6_8", L"./Resources/marin/Run6/marine0184.bmp"); // 마린
	AddImage("marin_run_6_9", L"./Resources/marin/Run6/marine0186.bmp"); // 마린

	AddImage("marin_run_7_1", L"./Resources/marin/Run7/marine0187.bmp"); // 마린
	AddImage("marin_run_7_2", L"./Resources/marin/Run7/marine0189.bmp"); // 마린
	AddImage("marin_run_7_3", L"./Resources/marin/Run7/marine0191.bmp"); // 마린
	AddImage("marin_run_7_4", L"./Resources/marin/Run7/marine0193.bmp"); // 마린
	AddImage("marin_run_7_5", L"./Resources/marin/Run7/marine0195.bmp"); // 마린
	AddImage("marin_run_7_6", L"./Resources/marin/Run7/marine0197.bmp"); // 마린
	AddImage("marin_run_7_7", L"./Resources/marin/Run7/marine0199.bmp"); // 마린
	AddImage("marin_run_7_8", L"./Resources/marin/Run7/marine0201.bmp"); // 마린
	AddImage("marin_run_7_9", L"./Resources/marin/Run7/marine0203.bmp"); // 마린

	AddImage("marin_run_8_1", L"./Resources/marin/Run8/marine0204.bmp"); // 마린
	AddImage("marin_run_8_2", L"./Resources/marin/Run8/marine0206.bmp"); // 마린
	AddImage("marin_run_8_3", L"./Resources/marin/Run8/marine0208.bmp"); // 마린
	AddImage("marin_run_8_4", L"./Resources/marin/Run8/marine0210.bmp"); // 마린
	AddImage("marin_run_8_5", L"./Resources/marin/Run8/marine0212.bmp"); // 마린
	AddImage("marin_run_8_6", L"./Resources/marin/Run8/marine0214.bmp"); // 마린
	AddImage("marin_run_8_7", L"./Resources/marin/Run8/marine0216.bmp"); // 마린
	AddImage("marin_run_8_8", L"./Resources/marin/Run8/marine0218.bmp"); // 마린
	AddImage("marin_run_8_9", L"./Resources/marin/Run8/marine0220.bmp"); // 마린

	AddImage("marin_death_1", L"./Resources/marin/Death/marine0221.bmp"); // 마린
	AddImage("marin_death_2", L"./Resources/marin/Death/marine0222.bmp"); // 마린
	AddImage("marin_death_3", L"./Resources/marin/Death/marine0223.bmp"); // 마린
	AddImage("marin_death_4", L"./Resources/marin/Death/marine0224.bmp"); // 마린
	AddImage("marin_death_5", L"./Resources/marin/Death/marine0225.bmp"); // 마린
	AddImage("marin_death_6", L"./Resources/marin/Death/marine0226.bmp"); // 마린
	AddImage("marin_death_7", L"./Resources/marin/Death/marine0227.bmp"); // 마린
	AddImage("marin_death_8", L"./Resources/marin/Death/marine0228.bmp"); // 마린

	AddImage("tcmdbtns0002", L"./Resources/UI2/tcmdbtns0002.bmp"); // 마린
	AddImage("tcmdbtns0003", L"./Resources/UI2/tcmdbtns0003.bmp"); // 마린
	AddImage("tcmdbtns0004", L"./Resources/UI2/tcmdbtns0004.bmp"); // 마린
	AddImage("tcmdbtns0007", L"./Resources/UI2/tcmdbtns0007.bmp"); // 마린
	AddImage("tcmdbtns0008", L"./Resources/UI2/tcmdbtns0008.bmp"); // 마린
	AddImage("tcmdbtns0009", L"./Resources/UI2/tcmdbtns0009.bmp"); // 마린
	AddImage("tcmdbtns0010", L"./Resources/UI2/tcmdbtns0010.bmp"); // 마린
	AddImage("tcmdbtns0011", L"./Resources/UI2/tcmdbtns0011.bmp"); // 마린
	AddImage("tcmdbtns0017", L"./Resources/UI2/tcmdbtns0017.bmp"); // 마린

	AddImage("Coll", L"./Resources/UI2/Coll.bmp"); // 마린
	AddImage("coolTimeBar", L"./Resources/UI2/coolTimeBar.bmp"); // 마린


	//파뱃

	AddImage("firebat_attack_1_1", L"./Resources/firebat/fire1/firebat0000.bmp"); // 마린 공격 프레임 1
	AddImage("firebat_attack_1_2", L"./Resources/firebat/fire1/firebat0002.bmp");
	AddImage("firebat_attack_1_3", L"./Resources/firebat/fire1/firebat0004.bmp");
	AddImage("firebat_attack_1_4", L"./Resources/firebat/fire1/firebat0006.bmp");
	AddImage("firebat_attack_1_5", L"./Resources/firebat/fire1/firebat0008.bmp");
	AddImage("firebat_attack_1_6", L"./Resources/firebat/fire1/firebat0010.bmp");
	AddImage("firebat_attack_1_7", L"./Resources/firebat/fire1/firebat0012.bmp");
	AddImage("firebat_attack_1_8", L"./Resources/firebat/fire1/firebat0014.bmp");
	AddImage("firebat_attack_1_9", L"./Resources/firebat/fire1/firebat0016.bmp");

	AddImage("firebat_attack_2_1", L"./Resources/firebat/fire2/firebat0017.bmp"); // 마린 공격프레임 2
	AddImage("firebat_attack_2_2", L"./Resources/firebat/fire2/firebat0019.bmp");
	AddImage("firebat_attack_2_3", L"./Resources/firebat/fire2/firebat0021.bmp");
	AddImage("firebat_attack_2_4", L"./Resources/firebat/fire2/firebat0023.bmp");
	AddImage("firebat_attack_2_5", L"./Resources/firebat/fire2/firebat0025.bmp");
	AddImage("firebat_attack_2_6", L"./Resources/firebat/fire2/firebat0027.bmp");
	AddImage("firebat_attack_2_7", L"./Resources/firebat/fire2/firebat0029.bmp");
	AddImage("firebat_attack_2_8", L"./Resources/firebat/fire2/firebat0031.bmp");
	AddImage("firebat_attack_2_9", L"./Resources/firebat/fire2/firebat0033.bmp");

	AddImage("firebat_idle_1", L"./Resources/firebat/Idle/firebat0051.bmp"); // 마린
	AddImage("firebat_idle_2", L"./Resources/firebat/Idle/firebat0053.bmp"); // 마린
	AddImage("firebat_idle_3", L"./Resources/firebat/Idle/firebat0055.bmp"); // 마린
	AddImage("firebat_idle_4", L"./Resources/firebat/Idle/firebat0057.bmp"); // 마린
	AddImage("firebat_idle_5", L"./Resources/firebat/Idle/firebat0059.bmp"); // 마린
	AddImage("firebat_idle_6", L"./Resources/firebat/Idle/firebat0061.bmp"); // 마린
	AddImage("firebat_idle_7", L"./Resources/firebat/Idle/firebat0063.bmp"); // 마린
	AddImage("firebat_idle_8", L"./Resources/firebat/Idle/firebat0065.bmp"); // 마린
	AddImage("firebat_idle_9", L"./Resources/firebat/Idle/firebat0067.bmp"); // 마린


	AddImage("firebat_run_1_1", L"./Resources/firebat/run/firebat0034.bmp"); // 마린
	AddImage("firebat_run_1_2", L"./Resources/firebat/run/firebat0036.bmp"); // 마린
	AddImage("firebat_run_1_3", L"./Resources/firebat/run/firebat0038.bmp"); // 마린
	AddImage("firebat_run_1_4", L"./Resources/firebat/run/firebat0040.bmp"); // 마린
	AddImage("firebat_run_1_5", L"./Resources/firebat/run/firebat0042.bmp"); // 마린
	AddImage("firebat_run_1_6", L"./Resources/firebat/run/firebat0044.bmp"); // 마린
	AddImage("firebat_run_1_7", L"./Resources/firebat/run/firebat0046.bmp"); // 마린
	AddImage("firebat_run_1_8", L"./Resources/firebat/run/firebat0048.bmp"); // 마린
	AddImage("firebat_run_1_9", L"./Resources/firebat/run/firebat0050.bmp"); // 마린

	AddImage("firebat_run_2_1", L"./Resources/firebat/run2/firebat0068.bmp"); // 마린
	AddImage("firebat_run_2_2", L"./Resources/firebat/run2/firebat0070.bmp"); // 마린
	AddImage("firebat_run_2_3", L"./Resources/firebat/run2/firebat0072.bmp"); // 마린
	AddImage("firebat_run_2_4", L"./Resources/firebat/run2/firebat0074.bmp"); // 마린
	AddImage("firebat_run_2_5", L"./Resources/firebat/run2/firebat0076.bmp"); // 마린
	AddImage("firebat_run_2_6", L"./Resources/firebat/run2/firebat0078.bmp"); // 마린
	AddImage("firebat_run_2_7", L"./Resources/firebat/run2/firebat0080.bmp"); // 마린
	AddImage("firebat_run_2_8", L"./Resources/firebat/run2/firebat0082.bmp"); // 마린
	AddImage("firebat_run_2_9", L"./Resources/firebat/run2/firebat0084.bmp"); // 마린

	AddImage("firebat_run_3_1", L"./Resources/firebat/run3/firebat0085.bmp"); // 마린
	AddImage("firebat_run_3_2", L"./Resources/firebat/run3/firebat0087.bmp"); // 마린
	AddImage("firebat_run_3_3", L"./Resources/firebat/run3/firebat0089.bmp"); // 마린
	AddImage("firebat_run_3_4", L"./Resources/firebat/run3/firebat0091.bmp"); // 마린
	AddImage("firebat_run_3_5", L"./Resources/firebat/run3/firebat0093.bmp"); // 마린
	AddImage("firebat_run_3_6", L"./Resources/firebat/run3/firebat0095.bmp"); // 마린
	AddImage("firebat_run_3_7", L"./Resources/firebat/run3/firebat0097.bmp"); // 마린
	AddImage("firebat_run_3_8", L"./Resources/firebat/run3/firebat0099.bmp"); // 마린
	AddImage("firebat_run_3_9", L"./Resources/firebat/run3/firebat0101.bmp"); // 마린

	AddImage("firebat_run_4_1", L"./Resources/firebat/run4/firebat0102.bmp"); // 마린
	AddImage("firebat_run_4_2", L"./Resources/firebat/run4/firebat0104.bmp"); // 마린
	AddImage("firebat_run_4_3", L"./Resources/firebat/run4/firebat0106.bmp"); // 마린
	AddImage("firebat_run_4_4", L"./Resources/firebat/run4/firebat0108.bmp"); // 마린
	AddImage("firebat_run_4_5", L"./Resources/firebat/run4/firebat0110.bmp"); // 마린
	AddImage("firebat_run_4_6", L"./Resources/firebat/run4/firebat0112.bmp"); // 마린
	AddImage("firebat_run_4_7", L"./Resources/firebat/run4/firebat0114.bmp"); // 마린
	AddImage("firebat_run_4_8", L"./Resources/firebat/run4/firebat0116.bmp"); // 마린
	AddImage("firebat_run_4_9", L"./Resources/firebat/run4/firebat0118.bmp"); // 마린

	AddImage("firebat_run_5_1", L"./Resources/firebat/run5/firebat0119.bmp"); // 마린
	AddImage("firebat_run_5_2", L"./Resources/firebat/run5/firebat0121.bmp"); // 마린
	AddImage("firebat_run_5_3", L"./Resources/firebat/run5/firebat0123.bmp"); // 마린
	AddImage("firebat_run_5_4", L"./Resources/firebat/run5/firebat0125.bmp"); // 마린
	AddImage("firebat_run_5_5", L"./Resources/firebat/run5/firebat0127.bmp"); // 마린
	AddImage("firebat_run_5_6", L"./Resources/firebat/run5/firebat0129.bmp"); // 마린
	AddImage("firebat_run_5_7", L"./Resources/firebat/run5/firebat0131.bmp"); // 마린
	AddImage("firebat_run_5_8", L"./Resources/firebat/run5/firebat0133.bmp"); // 마린
	AddImage("firebat_run_5_9", L"./Resources/firebat/run5/firebat0135.bmp"); // 마린

	AddImage("firebat_run_6_1", L"./Resources/firebat/run6/firebat0136.bmp"); // 마린
	AddImage("firebat_run_6_2", L"./Resources/firebat/run6/firebat0138.bmp"); // 마린
	AddImage("firebat_run_6_3", L"./Resources/firebat/run6/firebat0140.bmp"); // 마린
	AddImage("firebat_run_6_4", L"./Resources/firebat/run6/firebat0142.bmp"); // 마린
	AddImage("firebat_run_6_5", L"./Resources/firebat/run6/firebat0144.bmp"); // 마린
	AddImage("firebat_run_6_6", L"./Resources/firebat/run6/firebat0146.bmp"); // 마린
	AddImage("firebat_run_6_7", L"./Resources/firebat/run6/firebat0148.bmp"); // 마린
	AddImage("firebat_run_6_8", L"./Resources/firebat/run6/firebat0150.bmp"); // 마린
	AddImage("firebat_run_6_9", L"./Resources/firebat/run6/firebat0152.bmp"); // 마린

	AddImage("firebat_run_7_1", L"./Resources/firebat/run7/firebat0153.bmp"); // 마린
	AddImage("firebat_run_7_2", L"./Resources/firebat/run7/firebat0155.bmp"); // 마린
	AddImage("firebat_run_7_3", L"./Resources/firebat/run7/firebat0157.bmp"); // 마린
	AddImage("firebat_run_7_4", L"./Resources/firebat/run7/firebat0159.bmp"); // 마린
	AddImage("firebat_run_7_5", L"./Resources/firebat/run7/firebat0161.bmp"); // 마린
	AddImage("firebat_run_7_6", L"./Resources/firebat/run7/firebat0163.bmp"); // 마린
	AddImage("firebat_run_7_7", L"./Resources/firebat/run7/firebat0165.bmp"); // 마린
	AddImage("firebat_run_7_8", L"./Resources/firebat/run7/firebat0167.bmp"); // 마린
	AddImage("firebat_run_7_9", L"./Resources/firebat/run7/firebat0169.bmp"); // 마린

	AddImage("gas", L"./Resources/ResourceIcon/gas.bmp"); // 마린
	AddImage("min", L"./Resources/ResourceIcon/min.bmp"); // 마린
	AddImage("suf", L"./Resources/ResourceIcon/suf.bmp"); // 마린

	// 벌쳐
	AddImage("Vulture_1", L"./Resources/Vulture/vulture0000.bmp"); // 마린
	AddImage("Vulture_2", L"./Resources/Vulture/vulture0002.bmp"); // 마린
	AddImage("Vulture_3", L"./Resources/Vulture/vulture0004.bmp"); // 마린
	AddImage("Vulture_4", L"./Resources/Vulture/vulture0006.bmp"); // 마린
	AddImage("Vulture_5", L"./Resources/Vulture/vulture0008.bmp"); // 마린
	AddImage("Vulture_6", L"./Resources/Vulture/vulture0010.bmp"); // 마린
	AddImage("Vulture_7", L"./Resources/Vulture/vulture0012.bmp"); // 마린
	AddImage("Vulture_8", L"./Resources/Vulture/vulture0014.bmp"); // 마린
	AddImage("Vulture_9", L"./Resources/Vulture/vulture0016.bmp"); // 마린


}
