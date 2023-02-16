#pragma once
#include "WalkUnit.h"
#include "Effect.h"
#include "Player.h"

class Build;
class SpaceConstructionVehicle : public WalkUnit
{
private:
	float imgRot = 0;
	float timer = 0;
	float m_dir = 0;
	CImage* m_idleImage[9];
	CImage* m_actionImage_1[9];
	CImage* m_actionImage_2[9];
	CImage* m_buttom;
	Vector2 d;
	float mineTimer = 0;
	bool isMine = false;
	float mineResertTimer = 0;
	float m_ImageDirection = 0;
	Effect* m_spark;
	float astarTimer;
	enum eBuildClass
	{
		eBegin,
		eCommandCenter,
		eFactory,
		eBarrack,
		eAcademy,
		eEngin,
		eDepot,
		eStarport,
		eArmory,
		eScience
	};
	float randomMoveRot;
	float randomMoveTime;
	bool isa = false;
	int index = 0;
	
public:
	int page = 0;
	int buildIndex = 0;
	bool m_isBuild = false;
	Build* m_nowBuild;
	Build* m_command = nullptr;

	SpaceConstructionVehicle();
	~SpaceConstructionVehicle();
	virtual void Init() override;
	virtual void Move() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;
	virtual void Attack() override;
	virtual void ResetParam() override;
	Mineral* me;

	void ImageDirection();
	void BuildObject();
	void CollisionUpdate();
	void BuildingConstruction();
	void BuildCommandUI();
	void BuildActive();
	void Command();
};

