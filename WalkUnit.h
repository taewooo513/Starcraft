#pragma once
#include "Unit.h"
class WalkUnit : public Unit
{
public:
	WalkUnit();
	virtual ~WalkUnit();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;
	virtual void Release() override;
	// Unit을(를) 통해 상속됨
	virtual void Move() override;
	virtual void Attack() override;
};

