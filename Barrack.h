#pragma once
#include "Build.h"
class Barrack : public Build
{
private:

public:
	CImage* m_buildImage[4];

	Barrack();
	~Barrack();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

