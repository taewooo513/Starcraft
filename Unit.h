#pragma once
#include "Object.h"
#include <stack>
class Unit : public Object
{
protected:
	float m_direction;
	float m_speed;
	float m_speed2 = 0;

	float m_defense;
	float m_attack;
	float m_attackSpeed;
	int m_tribe;
	int m_state;//중형 대형 소형 
	bool m_isAttackAble;
	float m_maxHp;
	float range = 0;

	bool m_isMoveAble;
	Vector2 m_destination = { 0,0 };
public:
	Object* attackObject;

	Vector2 m_dest;
	bool m_isClick;
	float rot = 0; // 이미지 각도
	float rot2 = 0; // 움직임 각도

	Unit() {}
	virtual ~Unit() {}
	virtual void ResetParam() {}
	// Object을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;

	stack<MoveNode*> moveNodeStack;

	Player* player;
	void SetPlayer(Player* player) { this->player = player; }

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

