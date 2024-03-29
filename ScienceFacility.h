#pragma once
#include "Build.h"
class ScienceFacility : public Build
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

	ScienceFacility();
	virtual ~ScienceFacility();
	vImage* idleP[4];
	int randImgaeP = 0;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

