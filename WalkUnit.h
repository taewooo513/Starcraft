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
	// Unit��(��) ���� ��ӵ�
	virtual void Move() override;
	virtual void Attack() override;
};

