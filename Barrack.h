#pragma once
#include "Build.h"
class Barrack : public Build
{
private:
	struct AddUnit
	{
		float unit;
		float timeNow;
		float maxTime;
	};
	vImage* workImage;
	vector<AddUnit> addUnitQueue;
	bool sadf = false;
public:
	CImage* m_buildImage[4];
	CImage* m_shadImage[4];

	Barrack();
	virtual ~Barrack(); vImage* idleP[4];
	int randImgaeP = 0;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;

};

