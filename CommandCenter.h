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

	vector<AddUnit> addUnitQueue;
public:
	CommandCenter();
	virtual ~CommandCenter();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
	
};

