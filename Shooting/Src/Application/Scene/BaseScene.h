#pragma once

class BaseScene
{
public:

	BaseScene() { Init(); }
	virtual ~BaseScene() {}

	virtual void Init();
	virtual void Update();
	virtual void Draw();//2D描画
	virtual void Release();

private:
};