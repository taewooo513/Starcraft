#pragma once
//#include  ""
class Main
{
private:
	ID2D1HwndRenderTarget* pRT = NULL;
	ID2D1Factory1* pD2DFactory;
	ID2D1DeviceContext* m_d2dContext;
	ComPtr<IDXGIDevice1> dxgiDevice;
	ComPtr<ID3D11Device> device;
	ComPtr<IDXGISwapChain1> m_swapChain;
	ComPtr<ID2D1Bitmap1> m_d2dTargetBitmap;
public:
	Main() {}
	~Main() {}
public:
	void Init();
	void Update();
	void Render();
	void Release();
	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


	void CreateDeviceD3D();
	void CreateSwapChain();
	void CreateContext();
};

