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
public:
	CImage* m_buildImage[4];

	Barrack();
	virtual ~Barrack();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;

};

