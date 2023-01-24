#pragma once
#include "Scene.h"
class TestScene : public Scene
{
public:
	TestScene() {}
	~TestScene() {}
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

