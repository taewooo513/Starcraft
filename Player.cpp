#include "stdafx.h"
#include "Player.h"
#include "SpaceConstructionVehicle.h"

void Player::Init()
{
	Unit* units = new SpaceConstructionVehicle;
	m_selectUnits.push_back(units);

	//m_builds.push_back(""1);
}

