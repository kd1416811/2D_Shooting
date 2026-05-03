#include "Player.h"
#include "../../Scene/gameScene.h"
#include"../Bullet/Bullet.h"

void Player::Update()
{
	if (!m_aliveFlg) return;

	// 1. 移動処理
	UpdateMove();

	// 2. 攻撃処理
	UpdateShot();

	// 3. 当たり判定
	UpdateCheakCollision();

	// 4. 画面端制御
	UpdateMaxScreenEdge();

	// 5. 行列の更新
	UpdateMatrix();

}

void Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)PlayerMargin,(int)PlayerMargin }, 1.0f);
}

void Player::Init()
{
	m_tex.Load("Textures/Player/player.png");
	m_pos = { -600.0f,0.0f,0.0f };
	m_dir = { 0.0f,0.0f,0.0f };
	m_deg = 270;
	m_speed = 5.0f;
	m_shotTimer = 0.0f;
	m_deltaTime = 1.0f / 60.0f;
	m_aliveFlg = true;

	m_objType = objectType::player;
}

void Player::OnHit()
{

}

void Player::Release()
{

}

void Player::Shoot()
{
	std::shared_ptr<Bullet> newBullet = nullptr;

	switch (m_currentShotType)
	{
	case BulletType::Normal:

		// 弾を新しく生成
		newBullet = std::make_shared<Bullet>();

		break;
	}

	if (newBullet)
	{
		newBullet->Init();
		newBullet->SetPos(m_pos);
		newBullet->SetOwner(m_owner);

		// リストに追加
		m_owner->AddObject(newBullet);
	}
}

void Player::UpdateMove()
{
	//移動入力を一旦リセット
	m_dir = { 0.0f,0.0f,0.0f };

	// 移動
	if (GetAsyncKeyState('A') & 0x8000) m_dir.x -= 1.0f;
	if (GetAsyncKeyState('D') & 0x8000) m_dir.x += 1.0f;
	if (GetAsyncKeyState('W') & 0x8000) m_dir.y += 1.0f;
	if (GetAsyncKeyState('S') & 0x8000) m_dir.y -= 1.0f;

	//斜め移動が速くならないように
	if (m_dir.LengthSquared() > 0.0f)
	{
		m_dir.Normalize();

		m_pos += m_dir * m_speed;
	}
}

void Player::UpdateShot()
{
	// 通常は 1.0f 倍で進むタイマーを、移動方向によって増減させる
	// 前進中は進みが遅くなり、後退中は速くなる
	float timeScale = NormalTimeRatio - (m_dir.x * Half(NormalTimeRatio));
	m_shotTimer += m_deltaTime * timeScale;

	if (m_shotTimer >= ShotInterval)
	{
		Shoot();
		m_shotTimer = 0.0f;
	}
}

void Player::UpdateCheakCollision()
{
	for (auto& obj : m_owner->GetObjList())
	{
		//オブジェクトリストの中から敵とだけ当たり判定
		if (obj->GetObjType() == objectType::enemy)
		{
			// 敵の座標（ベクトル） - 自機の座標（ベクトル） = 敵へのベクトル（矢印）
			Math::Vector3 v;
			v = obj->GetPos() - m_pos;

			float HitDistance = Half(PlayerMargin) + Half(EnemyMargin);

			//球判定
			if (v.Length() < HitDistance)
			{
				//Hit時の処理
				obj->OnHit();

				// プレイヤー自身もダメージを受けるなら

			}
		}
	}
}

void Player::UpdateMaxScreenEdge()
{
	//自機が画面外に行かないようにする
	m_pos.x = (std::clamp)(m_pos.x, -(Half(SCREEN_WIDTH)) + Half(PlayerMargin), Half(SCREEN_WIDTH) - Half(PlayerMargin));
	m_pos.y = (std::clamp)(m_pos.y, -(Half(SCREEN_HEIGHT)) + Half(PlayerMargin), Half(SCREEN_HEIGHT) - Half(PlayerMargin));
}

void Player::UpdateMatrix()
{
	m_rotation = Math::Matrix::CreateRotationZ(ToRadians(m_deg));
	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = m_rotation * m_trans;
}
