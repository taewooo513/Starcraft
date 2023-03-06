#include "stdafx.h"
#include "Main.h"
void Main::Init()
{
	srand(time(NULL));
	SetTimer(_hWnd, 1, 1, NULL);

	CreateDeviceD3D();
	CreateSwapChain();

	IMAGEMANAGER->Init(m_d2dContext, m_swapChain.Get());
	TIMERMANAGER->Init();
	SCENEMANAGER->Init();
	KEYMANAGER->Init();
	SOUNDMANAGER->init();
	TIMERMANAGER->Init();
	SOUNDMANAGER->addSound("main-menu", "./Resources/Sound/main-menu.mp3", false, true);
	SOUNDMANAGER->addSound("terran-2", "./Resources/Sound/starcraft-terran-theme-2.mp3", false, true);
	SOUNDMANAGER->addSound("terran-1", "./Resources/Sound/terran-theme-1.mp3", false, true);
	SOUNDMANAGER->addSound("tscdth00", "./Resources/Sound/scv/tscdth00.wav", false, false);
	SOUNDMANAGER->addSound("tacwht00", "./Resources/Sound/bldg/tacwht00.wav", false, false);
	SOUNDMANAGER->addSound("tclwht00", "./Resources/Sound/bldg/tclwht00.wav", false, false);
	SOUNDMANAGER->addSound("tcswht00", "./Resources/Sound/bldg/tcswht00.wav", false, false);
	SOUNDMANAGER->addSound("tddwht00", "./Resources/Sound/bldg/tddwht00.wav", false, false);
	SOUNDMANAGER->addSound("twpwht00", "./Resources/Sound/bldg/twpwht00.wav", false, false);
	SOUNDMANAGER->addSound("tnswht00", "./Resources/Sound/bldg/tnswht00.wav", false, false);
	SOUNDMANAGER->addSound("tmswht00", "./Resources/Sound/bldg/tmswht00.wav", false, false);

	SOUNDMANAGER->addSound("taderr00", "./Resources/Sound/advisor/taderr00.wav", false, false);
	SOUNDMANAGER->addSound("taderr02", "./Resources/Sound/advisor/taderr02.wav", false, false);

	//scv
	SOUNDMANAGER->addSound("tscwht00", "./Resources/Sound/scv/tscwht00.wav", false, false);
	SOUNDMANAGER->addSound("tscwht01", "./Resources/Sound/scv/tscwht01.wav", false, false);
	SOUNDMANAGER->addSound("tscwht02", "./Resources/Sound/scv/tscwht02.wav", false, false);
	SOUNDMANAGER->addSound("tscwht03", "./Resources/Sound/scv/tscwht03.wav", false, false);

	//marine
	SOUNDMANAGER->addSound("tmawht00", "./Resources/Sound/marine/tmawht00.wav", false, false);
	SOUNDMANAGER->addSound("tmawht01", "./Resources/Sound/marine/tmawht01.wav", false, false);
	SOUNDMANAGER->addSound("tmawht02", "./Resources/Sound/marine/tmawht02.wav", false, false);
	SOUNDMANAGER->addSound("tmawht03", "./Resources/Sound/marine/tmawht03.wav", false, false);

	SOUNDMANAGER->addSound("tmadth00", "./Resources/Sound/marine/tmadth00.wav", false, false);
	SOUNDMANAGER->addSound("tmadth01", "./Resources/Sound/marine/tmadth01.wav", false, false);
	SOUNDMANAGER->addSound("tmafir00", "./Resources/Sound/marine/tmafir00.wav", false, false);

	//firebat
	SOUNDMANAGER->addSound("tfbwht00", "./Resources/Sound/firebat/tfbwht00.wav", false, false);
	SOUNDMANAGER->addSound("tfbwht01", "./Resources/Sound/firebat/tfbwht01.wav", false, false);
	SOUNDMANAGER->addSound("tfbwht02", "./Resources/Sound/firebat/tfbwht02.wav", false, false);
	SOUNDMANAGER->addSound("tfbwht03", "./Resources/Sound/firebat/tfbwht03.wav", false, false);

	//vulture
	SOUNDMANAGER->addSound("tvuwht00", "./Resources/Sound/vulture/tvuwht00.wav", false, false);
	SOUNDMANAGER->addSound("tvuwht01", "./Resources/Sound/vulture/tvuwht01.wav", false, false);
	SOUNDMANAGER->addSound("tvuwht02", "./Resources/Sound/vulture/tvuwht02.wav", false, false);
	SOUNDMANAGER->addSound("tvuwht03", "./Resources/Sound/vulture/tvuwht03.wav", false, false);
	SOUNDMANAGER->addSound("tvufir00", "./Resources/Sound/vulture/tvufir00.wav", false, false);
	SOUNDMANAGER->addSound("tvuhit02", "./Resources/Sound/vulture/tvuhit02.wav", false, false);

	//tank
	SOUNDMANAGER->addSound("ttawht00", "./Resources/Sound/tank/ttawht00.wav", false, false);
	SOUNDMANAGER->addSound("ttawht01", "./Resources/Sound/tank/ttawht01.wav", false, false);
	SOUNDMANAGER->addSound("ttawht02", "./Resources/Sound/tank/ttawht02.wav", false, false);
	SOUNDMANAGER->addSound("ttawht03", "./Resources/Sound/tank/ttawht03.wav", false, false);
	SOUNDMANAGER->addSound("ttafir00", "./Resources/Sound/tank/ttafir00.wav", false, false);
	SOUNDMANAGER->addSound("ttafi200", "./Resources/Sound/tank/ttafi200.wav", false, false);


	srand(GetTickCount64());
	SCENEMANAGER->ChangeScene("TitleScene");
}

void Main::Update()
{
	EFFECTMANAGER->Update();
	OBJECTMANAGER->Update();
	SCENEMANAGER->Update();
}

void Main::Render()
{
	m_d2dContext->BeginDraw();
	SCENEMANAGER->Render();
	OBJECTMANAGER->Render();
	EFFECTMANAGER->Render();
	OBJECTMANAGER->UIRender();
	SCENEMANAGER->UIRender();
	//TIMERMANAGER->Render();

	m_d2dContext->EndDraw();
	DXGI_PRESENT_PARAMETERS param = { 0 };
	m_swapChain->Present1(1, 0, &param);

}

void Main::Release()
{
	AVIFileExit();

	ObjectManager::ReleaseInstance();
	SceneManager::ReleaseInstance();
	ObjectManager::ReleaseInstance();

	GridManager::ReleaseInstance();
	ImageManager::ReleaseInstance();
	TIMERMANAGER->Release();
	EffectManager::ReleaseInstance();
	TimerManager::ReleaseInstance();
	KeyManager::ReleaseInstance();
	IOFillManager::ReleaseInstance();
	CSoundMgr::ReleaseInstance();
}

LRESULT Main::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		this->Render();
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void Main::CreateDeviceD3D()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(_hWnd, D2D1::SizeU(WINSIZE_X, WINSIZE_Y)), &pRT);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	ComPtr<ID3D11DeviceContext> context;
	D3D_FEATURE_LEVEL m_featureLevel;

	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&g_d3dDevice,
		&m_featureLevel,
		&context
	);

	g_d3dDevice.As(&dxgiDevice);

	pD2DFactory->CreateDevice(dxgiDevice.Get(), &g_device);

	g_device->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
		&m_d2dContext
	);
}

void Main::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = 0;                           // use automatic sizing
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // all apps must use this SwapEffect
	swapChainDesc.Flags = 0;

	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

	//dxgiFactory->CreateSwapChainForCoreWindow(
	//	g_d3dDevice.Get(),
	//	reinterpret_cast<IUnknown*>(window),
	//	&swapChainDesc,
	//	nullptr,    // allow on all displays
	//	&m_swapChain
	//);
	dxgiFactory.Get()->CreateSwapChainForHwnd(g_d3dDevice.Get(), _hWnd, &swapChainDesc, nullptr,
		nullptr, &m_swapChain);

	dxgiDevice->SetMaximumFrameLatency(1);

	ComPtr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
			WINSIZE_X,
			WINSIZE_Y
		);

	ComPtr<IDXGISurface> dxgiBackBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	m_d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		&bitmapProperties,
		&m_d2dTargetBitmap
	);

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

}

void Main::CreateContext()
{
}
