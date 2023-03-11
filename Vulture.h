#pragma once
#include "WalkUnit.h"
class Vulture : public WalkUnit
{
public:
	CImage* img[9];
	float imgRot = 0;
	float astarTimer = 0;
	Vulture();
	bool isDeath = false;
	virtual ~Vulture();
	Vector2 d = { 0,0 };
	float fireImageDel2 = 0;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;
	virtual void Release() override;
	// Unit을(를) 통해 상속됨
	virtual void Move() override;
	virtual void Attack() override;
	vImage* idleP[4];
	int randImgaeP = 0;
	void CollisionUpdate();
};

