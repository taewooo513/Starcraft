#include "stdafx.h"
#include "Player2.h"
#include "SpaceConstructionVehicle.h"
#include "CommandCenter.h"
#include "Marine.h"
#include "FireBat.h"
#include "Vulture.h"

Player2::Player2()
{
}

Player2::~Player2()
{
}

void Player2::Init()
{
	string unitsStr;
	unitsStr = FILEMANAGER->GetFileData("Player2", "Batch");
	vector<ObjectData*> objs;
	MY_UTILITY::ConvertStructureString2Vec(&objs, unitsStr);

	for (auto iter : objs)
	{
		switch (iter->key)
		{
		case 0:
		{
			Object* me = new Mineral;
			OBJECTMANAGER->AddObject(me, "mimeral", iter->x, iter->y, 0);
			resrouces.push_back(me);
		}
		break;
		case 1:
		{
			Build* commandCenter = new CommandCenter;
			commandCenter->SetPlayer(this);
			OBJECTMANAGER->AddObject(commandCenter, "Build", iter->x, iter->y, 0);
			commandCenter->AddComplete();
		}
		break;
		case 6:
		{
			Marine* marine = new Marine;
			marine->SetPlayer(this);
			OBJECTMANAGER->AddObject(marine, "marine", iter->x, iter->y, 0);
		}
		break;
		case 7:
		{
			Unit* scv = new SpaceConstructionVehicle;
			scv->SetPlayer(this);
			OBJECTMANAGER->AddObject(scv, "Unit", iter->x, iter->y, 1);
		}
		break;
		case 8:
		{
			Vulture* marine = new Vulture;
			marine->SetPlayer(this);
			OBJECTMANAGER->AddObject(marine, "marine", iter->x, iter->y, 0);
		}
		break;
		case 9:
		{
			Marine* marine = new Marine;
			marine->SetPlayer(this);
			OBJECTMANAGER->AddObject(marine, "marine", iter->x, iter->y, 0);
		}
		break;
		}
	}
}
