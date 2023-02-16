#pragma once
#include "Scene.h"
class TitleScene  : public Scene
{
public:
	vImage* vimage[4];
	vImage* vImage2[3];
	TitleScene();
	~TitleScene();

	RECT button[4]; 
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

