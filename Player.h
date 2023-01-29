#pragma once
#include "Build.h"
#include "Unit.h"
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
	Build* m_selectBuild;
	Unit* m_selectUnit;
	vector<Unit*> m_selectUnits;
	float sizeClick = 0;
	vector<Build*> m_builds;
	vector<Unit*> m_units;
	Vector2 m_rClickPos = { 0,0 };
	float m_clickStartX;
	float m_clickStartY;
	float m_clickEndX;
	float m_clickEndY;

public:

	void Init();
	void Update();
	void Render();
	void UIRender();
	void Release();
};

