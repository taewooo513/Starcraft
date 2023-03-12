#pragma once
#include "Build.h"
class CommandCenter : public Build
{
private:
	CImage* idle;
	CImage* buildImage[4];

	struct AddUnit
	{
		float unit;
		float timeNow;
		float maxTime;
	};
	bool isWork = false;
	float workTimer = 0;
	vector<AddUnit> addUnitQueue;
public:
	CommandCenter();
	virtual ~CommandCenter();
	vImage* idleP[4];
	int ac = 0;
	int randImgaeP = 0;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
	
};

