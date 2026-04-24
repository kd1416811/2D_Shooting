#pragma once
#include "../BaseObject.h"

class Player : public BaseObject
{
public:

	Player() {}
	~Player() { Release(); }

	void Update();
	void Draw();
	void Init();

private:

	void Release();
};