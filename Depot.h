#pragma once
#include "Build.h"
class Depot : public Build
{
private:
	struct AddUnit
	{
		float unit;
		float timeNow;
		float maxTime;
	};
	bool isonce = false;
	vImage* idle;
	vector<AddUnit> addUnitQueue;
public:
	CImage* m_buildImage[4];

	Depot();
	virtual ~Depot();
	vImage* idleP[4];
	int randImgaeP = 0;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

