#pragma once
#include "Object.h"
class Unit : public Object
{
protected:
	float m_direction;
	float m_speed;
	float m_defense;
	float m_attack;
	Vector2 m_dest;
	float m_attackSpeed;
	int m_tribe;
	int m_state;//���� ���� ���� 
	bool m_isAttackAble;
	bool m_isMoveAble;
	bool m_isClick;
	Vector2 m_destination = { 0,0 };
	RECT clickRect;
public:
	float rot = 0; // �̹��� ����
	float rot2 = 0; // ������ ����

	Unit() {}
	virtual ~Unit() {}
	// Object��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;

	virtual void Move() = 0;
	virtual void Attack() = 0;

	RECT GetClickRect()
	{
		return clickRect;
	}

	Vector2 GetPosition()
	{
		return position;
	}

	void SetDestPosition(Vector2 pos)
	{
		m_dest = pos;
	}
};

