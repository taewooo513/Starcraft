#pragma once
#include "Build.h"

class Starport : public Build
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

	Starport();
	virtual ~Starport();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

