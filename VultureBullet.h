#pragma once
#include "Object.h"
class VultureBullet : public Object 
{

public:
	Vector2 destPos;
	float rot = 0;
	vImage* img;
	VultureBullet();
	~VultureBullet();
	// Object��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

