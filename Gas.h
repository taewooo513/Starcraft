#pragma once
#include "Object.h"
#include "Unit.h"

class Gas : public Object
{
private:
	CImage* img;
	const int maxCount = 1500;
	int nowCount = 0;
public:
	Gas();
	~Gas();
	Unit* nowMineUnit;
	// Object을(를) 통해 상속됨
	void MineGas();


	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;

};

