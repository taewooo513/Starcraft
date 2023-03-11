#pragma once
#include "WalkUnit.h"
class Tank : public WalkUnit
{
public:
	CImage* img[9];
	CImage* img1[9];
	CImage* img2[9];
	CImage* simg[9];
	CImage* simg1[9];
	CImage* simg2[9];

	CImage* imgT[9];
	CImage* imgsT[9];

	CImage* sModeImg;
	CImage* sModeTImg[9];

	float astarTimer = 0;
	vImage* sMode;
	vImage* sModeT;

	float imgRot = 0;
	float astarTimer2 = 0;
	Tank();
	int moveFrame = 0;
	float fireImageDel2 = 0;
	float moveFrameTime = 0;
	bool isDeath = false;
	virtual ~Tank();
	Vector2 d = { 0,0 };
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void UIRender() override;
	vImage* idleP[4];
	int randImgaeP = 0;
	virtual void Release() override;
	// Unit을(를) 통해 상속됨
	virtual void Move() override;
	virtual void Attack() override;
	void CollisionUpdate();
	bool isSMode = false;
};

