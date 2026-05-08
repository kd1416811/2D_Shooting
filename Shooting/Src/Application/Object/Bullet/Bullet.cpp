#include "Bullet.h"

#include"../../Concept/define.h"

void Bullet::Update()
{
	if (!m_aliveFlg)return;

	//移動
	Move();

	//寿命、範囲チェック
	CheckLifeSpan();

	//行列更新
	UpdateMatrix();
}

void Bullet::Draw()
{
	if (!m_aliveFlg) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)Config::BulletMargin,(int)Config::BulletMargin }, 1.0f);
}

void Bullet::Init()
{
	m_aliveFlg = true;
	m_speed = 10.0f;
	m_pos = { 0,0,0 };
	m_dir = { 1.0f,0.0f,0.0f };//右に弾が出る

	m_objType = objectType::bullet;

	m_tex.Load("Textures/Bullet/bullet.png");
}

void Bullet::OnHit(long long damage)
{
	m_aliveFlg = false;
}

void Bullet::Release()
{}

void Bullet::Move()
{
	//弾の移動処理
	m_pos += m_dir * m_speed;
}

void Bullet::CheckLifeSpan()
{
	int screenHalf_W = One_Half(Config::SCREEN_WIDTH);
	int screenHlaf_H = One_Half(Config::SCREEN_HEIGHT);

	if (m_pos.x < -(screenHalf_W) - Config::BulletMargin ||
		m_pos.x >(screenHalf_W) + Config::BulletMargin ||
		m_pos.y < -(screenHlaf_H) - Config::BulletMargin ||
		m_pos.y >(screenHlaf_H) + Config::BulletMargin)
	{
		m_aliveFlg = false;
	}
}

void Bullet::UpdateMatrix()
{
	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = m_trans;
}
