#include "Player.h"

void Player::Update()
{
	// 移動
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pos.x -= 5;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pos.x += 5;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pos.y += 5;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pos.y -= 5;
	}

	m_rotation = Math::Matrix::CreateRotationZ(ToRadians(270));
	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = m_rotation * m_trans;
}

void Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,64,64 }, 1.0f);
}

void Player::Init()
{
	m_tex.Load("Textures/player.png");
	m_pos = {-600,0};
	m_aliveFlg = true;
}

void Player::Release()
{
	
}
