#include "Player.h"
#include "../../Scene/gameScene.h"
#include"../Bullet/Bullet.h"
#include"../../Concept/define.h"
#include <algorithm>

std::map<int, PlayerParameter> Player::s_playerMaster;

void Player::Update()
{
	if (!m_aliveFlg) return;

	// 1. 移動処理
	UpdateMove();

	// 2. 攻撃処理
	UpdateShot();

	// 3. 画面端制御
	UpdateMaxScreenEdge();


	//コンボリセット
	if (m_comboTimer > 0)
	{
		m_comboTimer -= m_deltaTime;

		if (m_comboTimer <= 0) ResetCombo();
	}

	// 5. 行列の更新
	UpdateMatrix();
}

void Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)Config::PlayerMargin,(int)Config::PlayerMargin }, 1.0f);
}

void Player::Init()
{
	m_tex.Load("Textures/Player/player.png");
	if(s_playerMaster.empty())LoadParameter();
	m_dir = { 0.0f,0.0f,0.0f };
	m_deg = 270;
	m_speed = 5.0f;
	m_shotTimer = 0.0f;
	m_deltaTime = 1.0f / 60.0f;
	m_comboCount = 0;
	m_comboTimer = 0.0f;
	m_shotInterval = 0.3f;	//初期発射間隔(0.3秒)
	m_aliveFlg = true;

	m_objType = objectType::player;
}

void Player::OnHit(long long damage)
{
	m_PlayerParam.nowHp -= damage;

	if (m_PlayerParam.nowHp <= 0)
	{
		m_PlayerParam.nowHp = 0;
		m_aliveFlg = false;
	}
}

void Player::SetType(int id)
{	
	auto it = s_playerMaster.find(id);

	if (it != s_playerMaster.end()) 
	{
		m_PlayerParam = it->second;

		m_pos = { m_PlayerParam.startPos.x,m_PlayerParam.startPos.y,0.0f };

		m_aliveFlg = true;
	}
}

void Player::Release()
{

}

void Player::Shoot()
{
	std::shared_ptr<Bullet> newBullet = nullptr;
	float skillRatio = 1.0f; //スキル倍率

	switch (m_currentShotType)
	{
	case BulletType::Normal:

		// 弾を新しく生成
		newBullet = std::make_shared<Bullet>();
		skillRatio = 1.1f;

		break;
	}

	if (newBullet)
	{
		newBullet->Init();
		newBullet->SetPos(m_pos);
		newBullet->SetOwner(m_owner);

		// 自分の攻撃力を弾に受け渡す！
		newBullet->SetAtk(m_PlayerParam.atk * skillRatio);

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
	float HalfTime = NormalTimeRatio * 0.5f;
	float timeScale = NormalTimeRatio - (m_dir.x * HalfTime);
	m_shotTimer += m_deltaTime * timeScale;

	if (m_shotTimer >= m_shotInterval)
	{
		Shoot();
		m_shotTimer = 0.0f;
	}
}

void Player::UpdateMaxScreenEdge()
{
	//自機が画面外に行かないようにする
	float screenHalf_W = Config::SCREEN_WIDTH * 0.5f;
	float screenHalf_H = Config::SCREEN_HEIGHT * 0.5f;
	float playerHalf = Config::PlayerMargin * 0.5f;

	float minX = -(screenHalf_W) + playerHalf;
	float maxX = screenHalf_W - playerHalf;
	m_pos.x = (std::clamp)(m_pos.x, minX, maxX);

	float minY = -(screenHalf_H) + playerHalf;
	float maxY = screenHalf_H - playerHalf;
	m_pos.y = (std::clamp)(m_pos.y, minY, maxY);
}

void Player::UpdateMatrix()
{
	m_rotation = Math::Matrix::CreateRotationZ(ToRadians(m_deg));
	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = m_rotation * m_trans;
}

void Player::LoadParameter()
{
	FILE* fp;

	if (fopen_s(&fp, "Data/Player/Player.csv", "r") == 0)
	{
		char dummy[255];
		int	 LoadingNum = 7;	//読み込む数だけ増やす
		long long hp = 0, atk = 0;
		float x = 0, y = 0;
		int def = 0, id = 0, Lv = 0;

		// ヘッダー（1行目）を読み飛ばす
		fgets(dummy, sizeof(dummy), fp);

		while (fscanf_s(fp, "%d,%f,%f,%lld,%lld,%d,%d",
			&id,&x, &y, &hp,&atk ,&def,&Lv)== LoadingNum)
		{
			PlayerParameter p;
			p.id = id;
			p.startPos.x = x;
			p.startPos.y = y;
			p.maxHp = hp;
			p.nowHp = hp;
			p.atk = atk;
			p.def = def;
			p.Lv = Lv;
			s_playerMaster[id] = p; // IDをキーに保存
		}

		fclose(fp);
	}
}
