#pragma once
#include "Scene.h"
class TestScene : public Scene
{
public:
	TestScene() {}
	~TestScene() {}
	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

