#pragma once
#include "Scene.h"
class MainGameScene : public Scene
{
public:
	MainGameScene();
	~MainGameScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

