#pragma once
#include "Build.h"
#include "Unit.h"
class Player
{
public:
	int m_population;
	int m_maxPopulation;

	Build* m_selectBuild;
	vector<Unit> m_selectUnits;

	vector<Build> m_builds;
	vector<Unit> m_units;
};

