#pragma once
#include "Object.h"
class Unit : public Object
{
protected:
	float m_direction;
	float m_speed;
	float m_defense;
	float m_attack;
	float m_dest;
	float m_attackSpeed;
	int m_tribe;
	int m_state;//중형 대형 소형 
	bool m_isAttackAble;
	bool m_isMoveAble;
public:
	Unit() {}
	virtual ~Unit() {}
	// Object을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;

	virtual void Move() = 0;
	virtual void Attack() = 0;
};

