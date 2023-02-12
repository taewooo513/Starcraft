#pragma once
#include "Build.h"
#include "Player.h"
#include "Unit.h"
#include "MapReader.h"

class Build;
class Player
{
private:
	RECT mapRect;
	vImage* m_cursorImage;
	float m_clickRad = 0;
	int m_population;
	int m_maxPopulation;
	bool m_isClick;
	bool m_isCameraClick;
	float sizeClick = 0;
	Vector2 m_rClickPos = { 0,0 };
	float m_clickStartX;
	float m_clickStartY;
	float m_clickEndX;
	float m_clickEndY;
public:
	int m_gas = 0;
	int m_mineral = 50;
	int m_suff = 0;
	int m_maxSuff = 0;
	Build* m_selectBuild;
	vector<Unit*> m_selectUnits;
	Unit* m_selectUnit;
	vector<Build*> m_builds;
	vector<Unit*> m_units;
	void Astar(Vector2 startPos, Vector2 endPos, Unit* unit);
	void Init();
	void Update();
	void Render();
	vector<pair<Vector2, Vector2>> testDraw;
	void UIRender();
	void Release();

	struct comp {
		bool operator()(pair<pair<float, float>, MapRegions*>A, pair<pair<float, float>, MapRegions*>B)
		{
			if (A.first.second > B.first.second)
				return true;
			else if (A.first.second == B.first.second)
				return A > B;
			return false;
		}
	};
	void AddBuild(Build* build)
	{
		m_builds.push_back(build);
	}
	void AddUnit(Unit* unit)
	{
		m_units.push_back(unit);
	}
};

