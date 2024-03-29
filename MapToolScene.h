#pragma once
#include "Scene.h"
class MapToolScene : public Scene
{
public:
	vector<ObjectData*> objdatas1;
	vector<ObjectData*> objdatas2;
	int team = 0;
	int objTag = -1; 
	int pageTag;
	MapToolScene();
	~MapToolScene();
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

