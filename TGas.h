#pragma once
#include "Build.h"
class TGas : public Build
{
public:
	TGas();
	~TGas();
public:
	CImage* m_buildImage[4];

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

