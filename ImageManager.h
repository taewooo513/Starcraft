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

class ImageManager : public Singleton<ImageManager>
{
private:
	IWICImagingFactory* factory = nullptr;
	ID2D1DeviceContext* m_d2dContext;
	IDXGISwapChain1* m_swapChain;
	ID2D1Effect* blendEffect;

	map <string, CImage*>m_imageList;
	map <string, vImage*> m_vImageList;

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

	vImage* AddImageVector(const std::string key, std::wstring path, int startIndex, int endIndex);
	vImage* FindImageVector(const std::string key);
	vImage* AddImageVectorCopy(const std::string key);

	void ImageLoad();

	void Render(CImage* img, Vector2 vec, float scale, float rot);
	void RenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot);
	void CenterRenderBlendBlack(CImage* img, Vector2 vec, float scale, float rot, bool isReverse);


};

#define IMAGEMANAGER ImageManager::GetInstance()

class vImage
{
private:

	vector<CImage*> m_images;

	bool m_isEnd = false;
	bool m_isLoop = false;

	float m_nowFrame;
	float m_timeDelay;

	int m_frame;

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
		for (auto iter : m_images)
		{
			SAFE_DELETE(iter);
		}
		m_images.clear();
	}

	bool GetIsEnd()
	{
		return m_isEnd;
	}
};


