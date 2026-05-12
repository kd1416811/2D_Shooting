#include "explosion.h"

void Explosion::Init()
{
	m_frame = 0;
	m_animeIdx = 0;
	m_pos = { 0, 0, 0 };
	m_aliveFlg = true;
	m_objType = objectType::None;

	m_tex.Load("Textures/Player/explosion.png");
}

void Explosion::Update()
{

	// 1フレームごとにコマを進める
	if (m_frame % 1 == 0)
	{
		m_animeIdx++;
	}

	if (m_animeIdx >= ANIME_MAX)
	{
		m_animeIdx = 0;
		m_aliveFlg = false; // アニメーションが終わったら消滅
	}

	m_frame++;

	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}

void Explosion::Draw()
{
	int size = 32; // 1コマのサイズ

	Math::Rectangle src = { m_animeIdx * size, 0, size, size };

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, 0, 0, size, size, &src);
}
