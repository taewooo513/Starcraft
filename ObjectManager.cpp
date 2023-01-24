#include "stdafx.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	Release();
}

Object* ObjectManager::AddObject(Object* _obj, string name, float x, float y, int tagNum)
{
	if (tagNum < eEndTag)
	{
		_obj->SetName(name);
		_obj->SetPosition({ x,y });
		m_objects[tagNum].push_back(_obj);
		_obj->Init();
		return _obj;
	}
	cout << "태그가 끝을 넘어서 버그남 tagNum: " << tagNum << " eEndTag: " << eEndTag;
	return nullptr;
}

void ObjectManager::Update()
{
	for (int i = 0; i < eEndTag; i++)
	{
		for (auto iter : m_objects[i])
		{
			(iter)->Update();
		}
	}
	for (int i = 0; i < eEndTag; i++)
	{
		for (auto iter = m_objects[i].begin(); iter != m_objects[i].end();)
		{
			if ((*iter)->GetIsObjectDestroyed() == true)
			{
				(*iter)->Release();
				SAFE_DELETE(*iter);
				*iter = nullptr;
				iter = m_objects[i].erase(iter);
			}
			else
				iter++;
		}
	}
}

void ObjectManager::Render()
{
	for (int i = 0; i < eEndTag; i++) // 먼저 그려주면 뒤에 그려지니까 렌더 순서를 정해줌 eEndTag F12눌러서 보십쇼
	{
		for (auto iter : m_objects[i])
		{
			iter->Render();
		}
	}
}

void ObjectManager::UIRender()
{
	for (int i = 0; i < eEndTag; i++) // 먼저 그려주면 뒤에 그려지니까 렌더 순서를 정해줌 eEndTag F12눌러서 보십쇼
	{
		for (auto iter : m_objects[i])
		{
			iter->UIRender();
		}
	}
}

void ObjectManager::Release()
{
	for (int i = 0; i < eEndTag; i++)
	{
		for (auto iter : m_objects[i])
		{
			iter->Release();
			SAFE_DELETE(iter)
		}
		m_objects[i].clear();
	}
}
