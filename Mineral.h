#pragma once
class Mineral : public Object
{
private:
	const int maxCount = 1500;
	int nowCount = 0;
public:
	CImage* resourceImage[4];
	Mineral();
	~Mineral();

	// Object��(��) ���� ��ӵ�
	void MineMineral();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void UIRender() override;
};

