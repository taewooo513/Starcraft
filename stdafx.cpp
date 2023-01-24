#include "stdafx.h"
HWND _hWnd;
POINT _ptMouse;
float g_TimeScale = 1;
ID2D1Device* g_device;
ComPtr <ID3D11Device> g_d3dDevice;
HINSTANCE hInst;

