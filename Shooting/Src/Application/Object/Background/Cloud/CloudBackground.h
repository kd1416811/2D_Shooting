#pragma once
#include"../BackgroundBase.h"

class CloudBackground : public BackgroundBase
{
public:

	CloudBackground();
	~CloudBackground();

	void Init()override;
	void Draw()override;
	void Update() override;

private:
};