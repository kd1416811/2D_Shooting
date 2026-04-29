#include "Bullet.h"

Bullet::Bullet() = default;

void Bullet::Init(const Math::Vector2& startPos, const Math::Vector2& dir, float speed)
{
	m_pos = startPos;
	m_dir = dir;
	m_speed = speed;
	m_aliveFlg = true;

	m_tex.Load("Textures/Bullet/bullet.png");
}

void Bullet::Update()
{
	if (!m_aliveFlg)return;

	//移動
	Move();

	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = m_trans;

	//寿命、範囲チェック
	CheckLifeSpan();
}

void Bullet::Draw()
{
	if (!m_aliveFlg) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)BulletMargin,(int)BulletMargin }, 1.0f);
}

void Bullet::Release()
{}

void Bullet::Move()
{
	m_pos += m_dir * m_speed;
}

void Bullet::CheckLifeSpan()
{
	if (m_pos.x < -(Half(SCREEN_WIDTH))- BulletMargin || m_pos.x >(Half(SCREEN_WIDTH)) + BulletMargin ||
		m_pos.y < -(Half(SCREEN_HEIGHT)) - BulletMargin || m_pos.y >(Half(SCREEN_HEIGHT)) + BulletMargin)
	{
		m_aliveFlg = false;
	}
}
