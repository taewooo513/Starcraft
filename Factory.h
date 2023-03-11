#pragma once
#include "Build.h"
class Machines;
class Factory : public Build
{
	struct AddUnit
	{
		float unit;
		float timeNow;
		float maxTime;
	};
	vImage* idleAnimation;
	vector<AddUnit> addUnitQueue;
	bool asdf = false;
public:
	CImage* m_buildImage[4];

	Factory();
	virtual ~Factory();
	vImage* idleP[4];
	int randImgaeP = 0;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;

	Machines* mac = nullptr;
};

