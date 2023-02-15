#pragma once
#include "singleton.h"
class CImage
{
private:
	float alpha;
	int	  width;
	int	  height;
	float delay;
	ID2D1Bitmap* bitmap;
public:
	CImage(ID2D1Bitmap* bitmap, int width, int height) :
		bitmap(bitmap), width(width), height(height)
	{
		alpha = 1;
	}
	CImage()
	{
		alpha = 1;
	}
	~CImage()
	{
		bitmap->Release();
	}

	float GetAlpha() { return alpha; }
	int   GetWidth() { return width; }
	int   GetHeight() { return height; }
	float GetDelay() { return delay; }
	void SetWidth(int width) { this->width = width; }
	void SetHeight(int height) { this->height = height; }
	ID2D1Bitmap* GetBitMap() { return bitmap; }
};

class vImage;
class MapReader;
class ImageManager : public Singleton<ImageManager>
{
private:
	MapReader* mapReader;
	D2D1_ELLIPSE* elipse;

	IWICImagingFactory* factory = nullptr;
	ID2D1DeviceContext* m_d2dContext;
	IDXGISwapChain1* m_swapChain;
	ID2D1Effect* blendEffect;
	IDWriteFactory3* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* brush;

	Vector2 m_cameraPosition = { 0,0 };
	IDWriteFontFile* pFontFile;
	map <string, CImage*>m_imageList;
	map <string, vImage*> m_vImageList;
	IDWriteTextLayout* pTextLayout_;
public:
	ImageManager();
	~ImageManager();
public:
	void Test(RECT rc);

	void Init(ID2D1DeviceContext* context, IDXGISwapChain1* swapChain);
	void CameraSetting();

	ID2D1Bitmap* AddBitmap(std::wstring path, UINT* Width, UINT* Height);
	CImage* AddImage(const std::string key, std::wstring path);
	CImage* FindImage(const std::string key);

	vImage* AddImageVector(const std::string key, std::wstring path, int startIndex, int endIndex, bool isPng = false);
	vImage* FindImageVector(const std::string key);
	vImage* AddImageVectorCopy(const std::string key);

	void ImageLoad();
	void DirectDrawText(wstring str, Vector2 pos, Vector2 size, D2D1_COLOR_F color = D2D1::ColorF(255, 255, 255, 1));

	void Render(CImage* img, Vector2 vec, float scale, float rot);
	void RenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot);
	void RenderBlendBlack2(CImage* img, Vector2 vec, float scale, float rot,float alpha);
	void UIRenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot);

	void CenterRenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot, bool isReverse);
	void UICenterRenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot, bool isReverse);
	void UICenterRenderBlendBlack2(CImage* img, Vector2 vec, Vector2 scale, float rot, bool isReverse);

	void MapRender();
	void UIMapRender();
	void DrawCircle(Vector2 vec, float scaleX, float scaleY, D2D1_COLOR_F color = D2D1::ColorF(255, 255, 255, 1));
	void DrawRect(Vector2 startPos, Vector2 endPos, D2D1_COLOR_F color = D2D1::ColorF(0, 255, 0, 1), int rectStyle = 0);
	void DrawRectRed(Vector2 startPos, Vector2 endPos);
	void DrawLine(Vector2 startPos, Vector2 endPos);
	void FogRender();
	void FogUpdate(Vector2 pos, float dest);

	/*
*/
	void DrawUI2(CImage* img, Vector2 vec, float scale, float rot, bool isReverse);

	void DrawRegion();
	void LoadMap();
	MapReader* GetMapReader() { return mapReader; }

	void CameraSetPosition(Vector2 vec)
	{
		m_cameraPosition.x = vec.x;
		m_cameraPosition.y = vec.y;
	}

	Vector2 GetCameraPosition()
	{
		return m_cameraPosition;
	}
};

#define IMAGEMANAGER ImageManager::GetInstance()

class vImage
{
private:
	vector<CImage*> m_images;

	bool m_isEnd = false;
	bool m_isLoop = false;

	float m_nowFrame = 0;
	float m_timeDelay = 0;

	int m_frame = 0;

public:
	vImage() {}
	~vImage() {}

	void Setting(float delayTime, bool isLoop)
	{
		m_isLoop = isLoop;
		m_timeDelay = delayTime;
		m_frame = 0;
	}

	void Reset()
	{
		m_frame = 0;
		m_nowFrame = 0;
		m_isEnd = false;
	}


	void UIRenderBlendBlack(Vector2 vec, float scale, float rot, bool isReverse)
	{
		if (m_isEnd == false)
			m_nowFrame += DELTA_TIME;

		if (m_nowFrame >= m_timeDelay)
		{
			if (m_frame >= GetImageSize() - 1)
			{
				if (m_isLoop == true)
				{
					m_frame = 0;
				}
				else
				{
					m_isEnd = true;
				}
			}
			else
			{
				m_frame++;
			}
			m_nowFrame = 0;
		}
		IMAGEMANAGER->UIRenderBlendBlack(m_images[m_frame], vec, scale, rot);
	}


	void CenterRenderBlendBlack(Vector2 vec, float scale, float rot, bool isReverse)
	{
		if (m_isEnd == false)
			m_nowFrame += DELTA_TIME;

		if (m_nowFrame >= m_timeDelay)
		{
			if (m_frame >= GetImageSize() - 1)
			{
				if (m_isLoop == true)
				{
					m_frame = 0;
				}
				else
				{
					m_isEnd = true;
				}
			}
			else
			{
				m_frame++;
			}
			m_nowFrame = 0;
		}
		IMAGEMANAGER->CenterRenderBlendBlack(m_images[m_frame], vec, scale, rot, isReverse);
	}

	int GetImageSize()
	{
		return m_images.size();
	}

	void AddImage(CImage* image)
	{
		m_images.push_back(image);
	}

	vector<CImage*> GetImages()
	{
		return m_images;
	}

	void Release()
	{
		m_images.clear();
	}

	bool GetIsEnd()
	{
		return m_isEnd;
	}
};


