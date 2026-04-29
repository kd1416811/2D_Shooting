#include "Enemy.h"

Enemy::Enemy() = default;

void Enemy::Update()
{
	if (!m_aliveFlg)return;

	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = m_trans;
}

void Enemy::Draw()
{
	if (!m_aliveFlg)return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,64,64 }, 1.0f);
}

void Enemy::Init()
{
	m_tex.Load("Textures/Enemy/enemy.png");
	m_pos = { 400,0 };
	m_aliveFlg = true;
}

void Enemy::Release()
{}
