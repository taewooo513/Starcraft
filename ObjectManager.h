#pragma once
#include "singleton.h"
#include "Object.h"

class BossObject;
class Item;
class Player;
class ObjectManager : public Singleton<ObjectManager>
{
public:
	Player* m_player;
	BossObject* m_boss;  
	bool stop_all;
	Item* m_item;
	ObjectManager();
	~ObjectManager();
public:
	void ThreadPool();
	vector<Object*> m_objects[eEndTag];
	Object* AddObject(Object* _obj, string name, float x, float y, int tagNum);
	concurrency::concurrent_queue<function<void()>> astarQueue;
	condition_variable cv_job_q_;
	std::mutex m_job_q_;
	void Update();
	void Render();
	void UIRender();
	void Release();
	int GetEnemyCount() { return m_objects[eEnemy].size(); }
};

#define OBJECTMANAGER ObjectManager::GetInstance()