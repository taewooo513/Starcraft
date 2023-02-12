#pragma once
#include "Unit.h"
#include "Player.h"

class SpaceConstructionVehicle;
class Build : public Object
{
protected:
	SpaceConstructionVehicle* buildUnit = nullptr;
	float m_hp;
	float m_costM;
	float m_costG;
	float m_maxHp;
	queue<Unit> m_unitQueue;
	float m_maxCompleteTime = 50.5f;
	float m_completeTime;
	int m_buildIndex;
	RECT clickRect;

public:
	bool isComplete = false;

	Build();
	virtual ~Build();
	// Object을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
	virtual void AddBuild(SpaceConstructionVehicle * spaceConstructionVehicle);
	virtual void SelectBuild();
	virtual void SelectBuildCommand();

	Player* player;
	void SetPlayer(Player* player) { this->player = player; }

	RECT GetClickRect()
	{
		return clickRect;
	}

	void AddComplete()
	{
		m_completeTime = m_maxCompleteTime;
		m_hp = m_maxHp;
		m_buildIndex = 4;
		isComplete = true;
	}
};

