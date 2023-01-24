#pragma once
#include "WalkUnit.h"
class SpaceConstructionVehicle : public WalkUnit
{
private:
	float m_dir = 0;
public:
	CImage* m_idleImage[9];
	CImage* m_actionImage_1[9];
	CImage* m_actionImage_2[9];
	CImage* m_buttom;
	int page = 0;
	SpaceConstructionVehicle();
	~SpaceConstructionVehicle();
	virtual void Init() override;
	virtual void Move() override;
	virtual void Render() override;
	virtual void UIRender() override;
	virtual void Attack() override;
};

