#pragma once
#include "WalkUnit.h"
#include "Build.h"
#include "Effect.h"
class SpaceConstructionVehicle : public WalkUnit
{
private:
	float m_dir = 0;
	CImage* m_idleImage[9];
	CImage* m_actionImage_1[9];
	CImage* m_actionImage_2[9];
	CImage* m_buttom;
	Effect* m_spark;
	enum eBuildClass
	{
		eBegin,
		eCommandCenter,
		eBarrack,
	};

	int page = 0;
	int index = 0;
public:
	int buildIndex = 0;
	bool m_isBuild = false;
	Build* m_nowBuild;
	SpaceConstructionVehicle();
	~SpaceConstructionVehicle();
	virtual void Init() override;
	virtual void Move() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;
	virtual void Attack() override;

	void BuildingConstruction();
};

