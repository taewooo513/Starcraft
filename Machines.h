#pragma once
#include "Build.h"
class Factory;
class Machines : public Build
{
	struct AddUnit
	{
		float unit;
		float timeNow;
		float maxTime;
	};
	vImage* idleAnimation;
	vector<AddUnit> addUnitQueue;
public:
	CImage* m_buildImage[4];
	vImage* vi;
	Machines();
	virtual ~Machines();
	Factory * fac;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};
