#pragma once
class Object
{
public:
	Vector2 position;

	Vector2 UIPosition[9] = {
		{847,606},
		{920, 606},
		{993, 606},
		{847,670},
		{920, 670},
		{993, 670},
		{847, 734},
		{920, 734},
		{993, 734},
	};
private:

	string name;
	float m_hp;
	bool m_isObjectDestroyed = false;

public:
	Object() {}
	virtual ~Object() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void UIRender() = 0;

	void SetPosition(Vector2 vec) { position = vec; }
	void SetName(string str) { name = str; }

	string GetName() { return name; }

	bool GetIsObjectDestroyed() { return m_isObjectDestroyed; }
	void ObjectDestroyed() { m_isObjectDestroyed = true; }


};

