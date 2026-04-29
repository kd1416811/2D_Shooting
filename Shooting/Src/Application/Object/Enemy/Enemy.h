#pragma once
#include"../BaseObject.h"

class Enemy:public BaseObject
{
public:

	Enemy();
	~Enemy()override { Release(); }

	void Update()override;
	void Draw()	 override;
	void Init()	 override;

private:

	void Release() override;
};