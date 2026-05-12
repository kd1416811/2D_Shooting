#pragma once
#include"BaseScene.h"
class TitleScene :public BaseScene
{
public:

	TitleScene() { Init(); }
	~TitleScene() {}

	void Init()override;
	void Update()override;
	void Draw()override;//2D描画
	void Release()override;

private:

	KdTexture m_tex;
	KdTexture m_startTex;

	Math::Matrix m_mat;
	Math::Matrix m_mat2;
	Math::Matrix m_StartMat;
	Math::Matrix m_ScaleMat;

	float m_alpha = 1.0f;
	float m_addAlpha = 0.01f;
};