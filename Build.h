#pragma once
#include "Unit.h"
class Build : public Object
{
protected:
	float m_hp;
	float m_costM;
	float m_costG;

	float m_makeDelay;
	queue<Unit> m_unitQueue;

public:

	Build();
	~Build();
	// Object을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

