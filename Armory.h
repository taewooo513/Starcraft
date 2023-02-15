#pragma once
#include "Build.h"
class Armory : public Build
{
	struct AddUnit
	{
		float unit;
		float timeNow;
		float maxTime;
	};

	vector<AddUnit> addUnitQueue;
public:
	CImage* m_buildImage[4];

	Armory();
	virtual ~Armory();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};
