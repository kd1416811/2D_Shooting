#include "TitleScene.h"
#include"SceneManager.h"

void TitleScene::Init()
{
	m_tex.Load("Textures/title.png");
	m_startTex.Load("Textures/start.png");
}

void TitleScene::Update()
{
	m_alpha += m_addAlpha;

	if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
		m_addAlpha *= -1;
	}
	else if (m_alpha < 0.0f)
	{
		m_alpha = 0.0f;
		m_addAlpha *= -1;
	}

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::Instance().SceneType::Game);
	}

	m_mat = Math::Matrix::CreateTranslation(0, 0, 0);

	m_ScaleMat = Math::Matrix::CreateScale(2.0f);
	m_StartMat = Math::Matrix::CreateTranslation(0, -300, 0);
	m_mat2 = m_ScaleMat * m_StartMat;
}

void TitleScene::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,1280,720 }, 1.0f);

	SHADER.m_spriteShader.SetMatrix(m_mat2);
	SHADER.m_spriteShader.DrawTex(&m_startTex, { 0,0,300,50 }, m_alpha);
}

void TitleScene::Release()
{}
