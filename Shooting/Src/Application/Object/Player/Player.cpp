#include "Player.h"
#include "../../Scene/gameScene.h"
#include"../Bullet/Bullet.h"
#include"../Bullet/BulletType/MoveHoming.h"
#include"../Bullet/BulletType/MoveWave.h"
#include"../Bullet/BulletType/MoveUltimate.h"
#include"../../Concept/define.h"
#include"../../Scene/SceneManager.h"
#include"../Effect/DamageEffect.h"
#include <algorithm>

std::map<int, PlayerParameter> Player::s_playerMaster;

void Player::Update()
{
	if (!m_aliveFlg) return;

	// --- 強化タイマーの更新 ---
	if (m_buffTimer > 0) 
	{
		m_buffTimer -= m_deltaTime;

		if (m_buffTimer <= 0)
		{
			m_buffTimer = 0;
			m_isBuffActive = false;
		}
	}

	// --- Rキーバフ（クリティカル）タイマーの更新 ---
	if (m_critBuffTimer > 0) 
	{
		m_critBuffTimer -= m_deltaTime;
		if (m_critBuffTimer <= 0) 
		{
			m_critBuffTimer = 0;
			m_isCritBuffActive = false;
		}
	}

	// 1. 移動処理
	UpdateMove();

	// 2. 攻撃処理
	UpdateShot();

	// 3. 画面端制御
	UpdateMaxScreenEdge();

	//アニメーション
	m_animCnt += 0.15f;
	if (m_animCnt >= 6)
	{
		m_animCnt = 0;
	}

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
	SHADER.m_spriteShader.DrawTex(&m_tex, { (int)m_animCnt * 128,0,(int)Config::PlayerMargin,(int)Config::PlayerMargin }, 1.0f);

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	DrawHpBar();
}

void Player::Init()
{
	m_tex.Load("Textures/Player/player.png");
	m_HpBarTex.Load("Textures/Enemy/HPBar.png");
	if(s_playerMaster.empty())LoadParameter();
	m_dir = { 0.0f,0.0f,0.0f };
	m_deg = 0;
	m_charaSizeRatio = 2;
	m_animCnt = 0;
	m_speed = 5.0f;
	m_shotTimer = 0.0f;
	m_deltaTime = 1.0f / 60.0f;
	m_comboCount = 0;
	m_comboTimer = 0.0f;
	//m_shotInterval = 0.3f;	//初期発射間隔(0.3秒)
	m_shotInterval = SceneManager::Instance().GetPlayerShotInterval();
	m_aliveFlg = true;

	m_objType = objectType::player;
}

void Player::OnHit(long long damage)
{
	m_PlayerParam.nowHp -= damage;

	//　--ダメージ数値表示--
	if (m_owner)
	{
		//インスタンス生成
		auto effect = std::make_shared<DamageEffect>();
		//初期化
		effect->Init();
		effect->SetOwner(m_owner);
		effect->SetDamage(damage, m_pos, false);
		effect->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		m_owner->AddObject(effect);
	}

	if (m_PlayerParam.nowHp <= 0)
	{
		m_PlayerParam.nowHp = 0;
		m_aliveFlg = false;
	}
}

void Player::SetType(int id, int stageLevel)
{	
	auto it = s_playerMaster.find(id);

	if (it != s_playerMaster.end()) 
	{
		m_PlayerParam = it->second;

		m_pos = { m_PlayerParam.startPos.x,m_PlayerParam.startPos.y,0.0f };

		// --- 等比インフレの計算 ---
		float Rate = 1.6f; // 1ステージごとに1.6倍にする設定
		float totalScale = std::pow(Rate, (float)(stageLevel - 1));

		//SceneManager によるプレイヤー強化倍率を取得
		float myAtkMult = SceneManager::Instance().GetAtkMultiplier();
		float myHpMult = SceneManager::Instance().GetHpMultiplier();
		float myDefMult = SceneManager::Instance().GetDefMultiplier();

		// 各ステータスに倍率を適用
		// HPの更新
		m_PlayerParam.maxHp = (long long)(m_PlayerParam.maxHp * (totalScale) * myHpMult);
		m_PlayerParam.nowHp = m_PlayerParam.maxHp;

		//防御更新
		m_PlayerParam.def = (int)(m_PlayerParam.def * (totalScale) * myDefMult);

		// ゲージの厚みも増やす(HPバー1本あたりの量もインフレさせる)
		m_PlayerParam.hpBar = (long long)(m_PlayerParam.hpBar * (totalScale));

		// 攻撃力の更新
		m_PlayerParam.atk = (long long)(m_PlayerParam.atk * totalScale * myAtkMult);

		m_aliveFlg = true;
	}
}

void Player::ShootSkill(SkillType type)
{
	std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>();
	newBullet->Init();
	newBullet->SetPos(m_pos);
	newBullet->SetOwner(m_owner);

	newBullet->UpdateMatrix();

	//BulletConfig config;

	//初期スキル倍率
	float damageMultiplier = 1.0f;

	// スキルに応じて「動きの部品」をプレゼントする
	switch (type) {
	case SkillType::Q:

		// ホーミング弾
		//config = { "Textures/Player/HomingAnime.png", 30, 0.08f, 1.0f, true ,960,32 };
		newBullet->SetStrategy(std::make_unique<MoveHoming>());
		damageMultiplier = 1.5f;
		break;
	case SkillType::W:
		// ウェイブ弾
		//config = { "Textures/Bullet/FlameBullet.png", 1,0.08f,1.0f,false,48,32 };
		newBullet->SetStrategy(std::make_unique<MoveWave>());
		damageMultiplier = 2.0f;
		break;

	case SkillType::E:
		
		//攻撃強化バフ
		m_buffTimer = 2.0f;
		m_isBuffActive = true;
		
		break;

	case SkillType::R:

		//クリティカル増加バフ
		m_critBuffTimer = 4.0f;    // 4秒間
		m_isCritBuffActive = true;

		break;

	case SkillType::ULT:
		// ULT用の超複雑な動きのクラスを作ってセット
		newBullet->SetStrategy(std::make_unique<MoveUltimate>());

		damageMultiplier = 7.5f;
		break;
	}

	//newBullet->SetBulletConfig(config);
	newBullet->SetAtk((long long)(m_PlayerParam.atk * damageMultiplier));
	m_owner->AddObject(newBullet);
}

void Player::Release()
{

}

void Player::Shoot()
{
	
	std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>();
	newBullet->Init();

	//BulletConfig config;
	//config.texPath = "Textures/Bullet/FlameBullet.png";
	//config.texW = 48;
	//config.texH = 32;
	//config.animeMax = 1;
	//config.scale = 1.0f;
	//config.isAddBlend = false;

	//newBullet->SetBulletConfig(config);
	newBullet->SetPos(m_pos);
	newBullet->SetOwner(m_owner);
	newBullet->UpdateMatrix();

	float skillRatio = 1.1f; //スキル倍率

	

	// ★もし強化中ならダメージをさらに2倍にする
	if (m_isBuffActive)
	{
		newBullet->SetAtk(m_PlayerParam.atk * skillRatio * 2.0f);
	}
	else
	{
		// 自分の攻撃力を弾に受け渡す！
		newBullet->SetAtk(m_PlayerParam.atk * skillRatio);
	}

	// リストに追加
	m_owner->AddObject(newBullet);
}

void Player::UpdateMove()
{
	//移動入力を一旦リセット
	m_dir = { 0.0f,0.0f,0.0f };

	// 移動
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) m_dir.x -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_dir.x += 1.0f;
	if (GetAsyncKeyState(VK_UP) & 0x8000) m_dir.y += 1.0f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) m_dir.y -= 1.0f;

	//斜め移動が速くならないように
	if (m_dir.LengthSquared() > 0.0f)
	{
		m_dir.Normalize();

		m_pos += m_dir * m_speed;
	}
}

void Player::UpdateShot()
{
	bool isEnemyAlive = false;
	if (m_owner) {
		const auto& objList = m_owner->GetObjList();
		for (const auto& obj : objList) {
			if (obj->GetObjType() == BaseObject::objectType::enemy && obj->GetAliveFlg()) {
				isEnemyAlive = true;
				break;
			}
		}
	}

	// 通常は 1.0f 倍で進むタイマーを、移動方向によって増減させる
	// 前進中は進みが遅くなり、後退中は速くなる
	float HalfTime = NormalTimeRatio * 0.5f;
	float timeScale = NormalTimeRatio - (m_dir.x * HalfTime);
	m_shotTimer += m_deltaTime * timeScale;

	if (m_shotTimer >= m_shotInterval)
	{
		if (isEnemyAlive) {
			//////////////////
		//--通常弾発射--
			Shoot();
			//////////////////
		}
		
		m_shotTimer = 0.0f;
	}


}

void Player::UpdateMaxScreenEdge()
{
	//自機が画面外に行かないようにする
	float minX = -(One_Half(Config::SCREEN_WIDTH)) + One_Half(Config::PlayerMargin);
	float maxX = One_Half(Config::SCREEN_WIDTH) - One_Half(Config::PlayerMargin);
	m_pos.x = (std::clamp)(m_pos.x, minX, maxX);

	float minY = -(One_Half(Config::SCREEN_HEIGHT)) + One_Half(Config::PlayerMargin);
	float maxY = One_Half(Config::SCREEN_HEIGHT) - One_Half(Config::PlayerMargin);
	m_pos.y = (std::clamp)(m_pos.y, minY, maxY);
}

void Player::UpdateMatrix()
{
	m_rotation = Math::Matrix::CreateRotationZ(ToRadians(m_deg));
	m_scale = Math::Matrix::CreateScale(m_charaSizeRatio);
	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = m_scale * m_rotation * m_trans;
}

void Player::LoadParameter()
{
	FILE* fp;

	if (fopen_s(&fp, "Data/Player/Player.csv", "r") == 0)
	{
		char dummy[255];
		int	 LoadingNum = 8;	//読み込む数だけ増やす
		long long hp = 0, atk = 0, hpBar = 0;
		float x = 0, y = 0;
		int def = 0, id = 0, Lv = 0;

		// ヘッダー（1行目）を読み飛ばす
		fgets(dummy, sizeof(dummy), fp);

		while (fscanf_s(fp, "%d,%f,%f,%lld,%lld,%lld,%d,%d",
			&id,&x, &y, &hp,&hpBar,&atk ,&def,&Lv)== LoadingNum)
		{
			PlayerParameter p;
			p.id = id;
			p.startPos.x = x;
			p.startPos.y = y;
			p.maxHp = hp;
			p.nowHp = hp;
			p.hpBar = hpBar;
			p.atk = atk;
			p.def = def;
			p.Lv = Lv;
			s_playerMaster[id] = p; // IDをキーに保存
		}

		fclose(fp);
	}
}

void Player::DrawHpBar()
{
	int BarWidth = 60, BarHeight = 5;

	//現在のHPを1本あたりのHPで割った数
	int currentBarIdx = (int)(m_PlayerParam.nowHp / m_PlayerParam.hpBar);

	float ratio = (float)((double)(m_PlayerParam.nowHp % m_PlayerParam.hpBar) / m_PlayerParam.hpBar);
	if (m_PlayerParam.nowHp > 0 && ratio == 0.0f)
	{
		ratio = 1.0f;
		currentBarIdx--;
	}
	
	//プレイヤーの下に表示
	Math::Vector2 pivotLeft = { 0.0f, 1.0f }; // 左端を基準にする

	// 色リスト
	Math::Color color = { 0.0f, 1.0f, 0.0f, 1.0f }; // 緑
	Math::Color black = { 0.0f, 0.0f, 0.0f, 1.0f };	//黒

	Math::Vector3 textPos = { m_pos.x - HpBarAddPos.x,m_pos.y - HpBarAddPos.y,HpBarAddPos.z };

	// --- 描画処理 ---

	// 土台 (黒)
	SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)textPos.x, (int)textPos.y, BarWidth, BarHeight, nullptr, &black, pivotLeft);

	// 下の層 (背景色。2本目以降がある場合)
	if (currentBarIdx > 0) {
		SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)textPos.x, (int)textPos.y, BarWidth, BarHeight, nullptr, &color, pivotLeft);
	}

	// 現在の層 (割合に応じて幅を可変させる)
	int drawWidth = (int)(BarWidth * ratio);

	// 画像のサイズに合わせてSrcRectを作る
	Math::Rectangle srcRect = { 0, 0, (long)(BarWidth * ratio), (long)BarHeight };

	SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)textPos.x, (int)textPos.y, drawWidth, BarHeight, &srcRect, &color, pivotLeft);
}
