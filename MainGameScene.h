#pragma once
#include "Scene.h"
#include "Player.h"
#include "Mineral.h"
#include "Player2.h"
class MainGameScene : public Scene
{
private:
	Player* player1;
	Player2* player2;

public:
	MainGameScene();
	~MainGameScene();

	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

