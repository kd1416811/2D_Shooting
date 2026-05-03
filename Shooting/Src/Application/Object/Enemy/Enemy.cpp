#include "Enemy.h"

Enemy::Enemy() = default;

void Enemy::Update()
{
	if (!m_aliveFlg)return;

	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = m_trans;
}

void Enemy::Draw()
{
	if (!m_aliveFlg)return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)EnemyMargin,(int)EnemyMargin }, 1.0f);
}

void Enemy::Init()
{
	m_tex.Load("Textures/Enemy/enemy.png");

	m_pos = { 500.0f,0.0f,0.0f };
	m_aliveFlg = true;

	m_objType = objectType::enemy;
}

void Enemy::OnHit()
{
	m_aliveFlg = false;
}

void Enemy::Release()
{}
