#pragma once
#include"../BackgroundBase.h"

class AbyssBackground :public BackgroundBase
{
public:

	AbyssBackground();
	~AbyssBackground();

	void Init() override;
	void Draw() override;
	void Update() override;

private:
};