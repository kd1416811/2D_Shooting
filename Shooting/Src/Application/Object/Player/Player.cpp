#include <algorithm>

#include "Player.h"
#include"../Bullet/Bullet.h"
#include"../Bullet/BulletManager.h"


void Player::Update()
{
	//移動入力を一旦リセット
	m_dir = { 0.0f,0.0f };

	// 移動
	if (GetAsyncKeyState('A') & 0x8000) m_dir.x -= 1.0f;
	if (GetAsyncKeyState('D') & 0x8000) m_dir.x += 1.0f;
	if (GetAsyncKeyState('W') & 0x8000) m_dir.y += 1.0f;
	if (GetAsyncKeyState('S') & 0x8000) m_dir.y -= 1.0f;

	//斜め移動が速くならないように
	if (m_dir.LengthSquared() > 0.0f)
	{
		m_dir.Normalize();

		m_speed = 5.0f;
		m_pos += m_dir * m_speed;
	}

	//自機が画面に行かないようにする
	m_pos.x = (std::clamp)(m_pos.x, -(Half(SCREEN_WIDTH)) + Half(PlayerMargin), Half(SCREEN_WIDTH) - Half(PlayerMargin));
	m_pos.y = (std::clamp)(m_pos.y, -(Half(SCREEN_HEIGHT)) + Half(PlayerMargin), Half(SCREEN_HEIGHT) - Half(PlayerMargin));


	// 通常は 1.0f 倍で進むタイマーを、移動方向によって増減させる
	// 前進中は進みが遅くなり、後退中は速くなる
	float timeScale = NormalTimeRatio - (m_dir.x * Half(NormalTimeRatio));
	m_shotTimer += m_deltaTime * timeScale;

	if (m_shotTimer >= SHOT_INTERVAL)
	{
		Shoot();
		m_shotTimer = 0.0f;
	}

	m_rotation = Math::Matrix::CreateRotationZ(ToRadians(270));
	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = m_rotation * m_trans;
}

void Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)PlayerMargin,(int)PlayerMargin }, 1.0f);
}

void Player::Init()
{

	m_tex.Load("Textures/PLayer/player.png");
	m_pos = {-600,0};
	m_dir = { 0.0f,0.0f };

	m_shotTimer = 0.0f;
	m_deltaTime = 1.0f / 60.0f;
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
		Math::Vector2 shotDir = { 1.0f,0.0f };
		float shotSpeed = 10.0f;

		newBullet->Init(m_pos, shotDir, shotSpeed);

		// リストに追加
		m_bulletManager->AddBullet(newBullet);
	}
}

void Player::Release()
{
	
}
