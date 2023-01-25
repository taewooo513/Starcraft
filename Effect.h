#pragma once
class Effect
{
private:
	float m_scale;
	Vector2 position;
	vImage* m_image;
public:
	bool m_isDestroy;
	bool m_isEnd;
	Effect() {}
	virtual ~Effect() {}
	void Setting(string key, Vector2 pos, float scale, float timeDelay);

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	bool GetDeostroy()
	{
		return m_isDestroy;
	}
};

