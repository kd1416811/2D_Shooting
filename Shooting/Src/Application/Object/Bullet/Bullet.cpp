#include "Bullet.h"
#include"../../Scene/gameScene.h"

void Bullet::Update()
{
	if (!m_aliveFlg)return;

	//移動
	Move();

	//当たり判定
	CheakCollision();

	//寿命、範囲チェック
	CheckLifeSpan();

	//行列更新
	UpdateMatrix();
}

void Bullet::Draw()
{
	if (!m_aliveFlg) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)BulletMargin,(int)BulletMargin }, 1.0f);
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

void Bullet::OnHit()
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
	if (m_pos.x < -(Half(SCREEN_WIDTH)) - BulletMargin || m_pos.x >(Half(SCREEN_WIDTH)) + BulletMargin ||
		m_pos.y < -(Half(SCREEN_HEIGHT)) - BulletMargin || m_pos.y >(Half(SCREEN_HEIGHT)) + BulletMargin)
	{
		m_aliveFlg = false;
	}
}

void Bullet::CheakCollision()
{
	for (auto& obj : m_owner->GetObjList())
	{
		//オブジェクトリストの中から敵とだけ当たり判定
		if (obj->GetObjType() == objectType::enemy)
		{
			// 敵の座標（ベクトル） - 弾の座標（ベクトル） = 敵へのベクトル（矢印）
			Math::Vector3 v;
			v = obj->GetPos() - m_pos;

			float HitDistance = Half(EnemyMargin) + Half(BulletMargin);

			//球判定
			if (v.Length() < HitDistance)
			{
				//Hit時の処理
				obj->OnHit();

				OnHit();
			}
		}
	}
}

void Bullet::UpdateMatrix()
{
	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = m_trans;
}
