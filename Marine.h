#pragma once
#include "WalkUnit.h"
class Marine : public WalkUnit
{
public:

	// WalkUnit��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Move() override;
	virtual void Attack() override;

	virtual void Render() override;
	virtual void UIRender() override;
	virtual void Release() override;
};

