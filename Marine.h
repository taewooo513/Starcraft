#pragma once
#include "WalkUnit.h"
class Marine : public WalkUnit
{
public:
	CImage* idleImage[9];
	CImage* fire1Image[9];
	CImage* fire2Image[9];

	CImage* run1Image[9];
	CImage* run2Image[9];
	CImage* run3Image[9];
	CImage* run4Image[9];
	CImage* run5Image[9];
	CImage* run6Image[9];
	CImage* run7Image[9];
	CImage* run8Image[9];
	CImage* deathImage[8];

	bool nowFire = false;
	float fireImageDel = 0;
	float fireImageDel2 = 0;
	bool isFire = true;
	bool isDeath = false;
	bool isFire2 =false;
	int fireCount = 0;
	int runFrame = 0;
	float runImageTimeDelay;
	float deathImageTimeDelay;
	int deathFrame = 0;
	// WalkUnit을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Move() override;
	virtual void Attack() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;
	virtual void Release() override;

	void CollisionUpdate();

	Vector2 d = { 0,0 };
	float astarTimer = 0;
};

