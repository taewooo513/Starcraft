#pragma once
#include "Unit.h"
class FlyUnit : public Unit
{
public:
	FlyUnit();
	virtual ~FlyUnit();
	virtual void Init() override;

	virtual void Move() override;

	virtual void Attack() override;

};

