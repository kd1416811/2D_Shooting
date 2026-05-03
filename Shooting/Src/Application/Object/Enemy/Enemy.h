#pragma once 
#include"../BaseObject.h"

class Enemy :public BaseObject
{
public:

	Enemy();
	~Enemy()override { Release(); }

	void Update() override;
	void Draw()override;
	void Init()override;

	void OnHit() override;

private:

	void Release()override;


};