#pragma once

class Effect;
class EffectManager : public Singleton<EffectManager>
{
public:
	EffectManager();
	~EffectManager();

	vector<Effect*> m_effects;

	void AddEffect(string key, Vector2 pos, int size, float timeDelay);
	void Update();
	void Render();
	void Release();
};

#define EFFECTMANAGER EffectManager::GetInstance()