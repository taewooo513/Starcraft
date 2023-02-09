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
	cout << col->GetFontFamilyCount();
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
	cout << name;
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
	m_d2dContext->DrawEllipse(elipse, brush.Get(), 0.1f);
}

void ImageManager::DrawRect(Vector2 startPos, Vector2 endPos, D2D1_COLOR_F color)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matP = D2D1::Matrix3x2F::Translation(0, 0);
	matW = matP;

	m_d2dContext->SetTransform(matW);
	m_d2dContext->CreateSolidColorBrush({ 0,255,0,255 }, &brush);
	m_d2dContext->DrawRectangle({ startPos.x,startPos.y ,endPos.x ,endPos.y }, brush, 1.5);
}

void ImageManager::DrawRectRed(Vector2 startPos, Vector2 endPos)
{
	D2D1_MATRIX_3X2_F matW, matR, matS, matP;

	matP = D2D1::Matrix3x2F::Translation(0, 0);
	matW = matP;

	m_d2dContext->SetTransform(matW);

	ComPtr<ID2D1SolidColorBrush> brush;
	m_d2dContext->CreateSolidColorBrush({ 255,0,0,255 }, &brush);
	m_d2dContext->DrawRectangle({ startPos.x,startPos.y ,endPos.x ,endPos.y }, brush.Get(), 1.5);
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

vImage* ImageManager::AddImageVector(const std::string key, std::wstring path, int startIndex, int endIndex)
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
				_path = path + L"0" + std::to_wstring(i).c_str() + L".bmp";
			}
			else
			{
				_path = path + std::to_wstring(i).c_str() + L".bmp";
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


	// 미네랄

	// SCV
	AddImage("wirefram0106", L"./Resources/Icon2/wirefram0106.bmp"); //SCVcmdicons0007
	AddImage("cmdicons0007", L"./Resources/Icon/cmdicons0007.bmp"); //SCVcmdicons0007

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

}
