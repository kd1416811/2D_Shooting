#include "Enemy.h"
#include"../Effect/DamageEffect.h"
#include"../../Scene/GameScene.h"
#include"../../Concept/define.h"
#include"EnemyDeathAnim.h"
#include"../Bullet/Bullet.h"
#include<string.h>
std::map<int, EnemyParameter> Enemy::s_enemyMaster;

Enemy::Enemy() = default;

void Enemy::Update()
{
	if (!m_aliveFlg)return;

	//if (m_invincibleTimer > 0) m_invincibleTimer -= m_deltaTime;
	// --- ランダム移動処理 ---
	m_moveTimer -= m_deltaTime;

	if (m_moveTimer <= 0)
	{
		// 1秒～3秒のランダムな時間をセット
		m_moveTimer = 1.0f + (float)(rand() % 200) / 100.0f;

		// XとYの移動方向をランダムに決定 (-1.0 ～ 1.0)
		float randX = (float)(rand() % 201 - 100) / 100.0f;
		float randY = (float)(rand() % 201 - 100) / 100.0f;

		m_moveDir = { randX, randY, 0.0f };

		// 斜め移動でも速さが変わらないように正規化
		if (m_moveDir.LengthSquared() > 0) {
			m_moveDir.Normalize();
		}
	}
	m_pos += m_moveDir * m_moveSpeed;

	float waveY = sin(m_animCnt * 1.5f) * 5.0f;

	// --- 画面外に行かないように制限（オプション） ---
	// 画面サイズに合わせて数値を調整してください
	m_pos.x = std::clamp(m_pos.x, 0.0f, 600.0f);
	m_pos.y = std::clamp(m_pos.y, -200.0f, 200.0f);

	//アニメーション
	m_animCnt += 0.14f;
	if (m_animCnt >= 4)
	{
		m_animCnt = 0;
	}

	// --- 攻撃タイマーとアニメーションの更新 ---
	UpdateShot();

	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y + waveY, m_pos.z);
	m_scale = Math::Matrix::CreateScale(2);
	m_mat = m_scale * m_trans;
}

void Enemy::Draw()
{
	if (!m_aliveFlg)return;
	
	if(m_isAttacking)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex(&m_attackTex, { (int)m_attackAnimCnt * (int)Config::EnemyMargin_x,0,(int)Config::EnemyMargin_x,(int)Config::EnemyMargin_y }, 1.0f);
	}
	else
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex(&m_tex, { (int)m_animCnt * (int)Config::EnemyMargin_x,0,(int)Config::EnemyMargin_x,(int)Config::EnemyMargin_y }, 1.0f);
	}

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	DrawHpBar();
}

void Enemy::Init()
{
	if (s_enemyMaster.empty()) LoadParameter();
	m_NumberTex.Load("Textures/number2.png");
	m_animCnt = 0;
	m_moveSpeed = 2.0f;
	m_deltaTime = 1.0f / 60.0f;
	m_shotTimer = 0.0f;
	m_shotInterval = 2.5f;
	// 初期タイマーと初期方向
	m_moveTimer = 0.0f;
	m_moveDir = { 0, 0, 0 };
	m_objType = objectType::enemy;
}

void Enemy::OnHit(long long damage, bool b_critical)
{
	if (IsInvincible()) return; // 無敵中なら当たらない

	m_EnemyParam.nowHp -= damage;
	m_invincibleTimer = 0.0f; // ヒット後 0.1秒間は当たらない（多段ヒットの速さ調整）

	//　--ダメージ数値表示--
	if (m_owner)
	{
		//インスタンス生成
		auto effect = std::make_shared<DamageEffect>();
	//初期化
		effect->Init();
		effect->SetOwner(m_owner);
		effect->SetDamage(damage, m_pos,b_critical);
		effect->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		m_owner->AddObject(effect);
	}

	if (m_EnemyParam.nowHp <= 0)
	{
		m_EnemyParam.nowHp = 0;

		// ★敵が死んだ瞬間
		if (m_owner)
		{
			// 死亡アニメーションオブジェクトを生成
			auto deathAnim = std::make_shared<EnemyDeathAnim>();
			deathAnim->Init();
			// ★敵の今の場所をセット
			deathAnim->SetPos(m_pos);
			// ゲームシーンに追加
			m_owner->AddObject(deathAnim);
		}

		m_aliveFlg = false;
	}
}

void Enemy::Release()
{
	m_tex.Release();
}

void Enemy::SetType(int id,int stageLevel)
{
	// 名簿にIDがあるかチェックしてコピー
	auto it = s_enemyMaster.find(id);

	if (it != s_enemyMaster.end())
	{
		// 構造体の値をまるごとコピー
		m_EnemyParam = it->second;

		m_pos = { m_EnemyParam.startPos.x,m_EnemyParam.startPos.y,0.0f };

		m_tex.Load("Textures/Enemy/EnemyAnim/FLYING.png");
		m_attackTex.Load("Textures/Enemy/EnemyAnim/ATTACK.png");
		m_HpBarTex.Load("Textures/Enemy/HPBar.png");

		// --- 等比インフレの計算 ---
		float Rate = 3.65f; // 1ステージごとに2.2倍にする設定
		float totalScale = std::pow(Rate, (float)(stageLevel - 1));

		// 各ステータスに倍率を適用
		// HPの更新
		m_EnemyParam.maxHp = (long long)(m_EnemyParam.maxHp * (totalScale + 0.25f));
		m_EnemyParam.nowHp = m_EnemyParam.maxHp;

		//防御の更新
		m_EnemyParam.def = (int)(m_EnemyParam.def * (totalScale + 0.25f));

		// ゲージの厚みも増やす(HPバー1本あたりの量もインフレさせる)
		m_EnemyParam.hpBar = (long long)(m_EnemyParam.hpBar * (totalScale + 0.25f));
		
		// 攻撃力の更新
		m_EnemyParam.attack = (int)(m_EnemyParam.attack * totalScale);

		m_aliveFlg = true;
	}
}

void Enemy::LoadParameter()
{
	FILE* fp;

	if (fopen_s(&fp, "Data/Enemy/Enemy.csv", "r") == 0)
	{
		char dummy[255];
		int	 LoadingNum = 8;	//読み込む数だけ増やす
		int  id = 0, atk = 0, def = 0, Resistance = 0;
		float x = 0, y = 0;
		long long hp = 0, hpBar = 0;

		// ヘッダー（1行目）を読み飛ばす
		fgets(dummy, sizeof(dummy), fp);

		while (fscanf_s(fp, "%d,%f,%f,%lld,%lld,%d,%d,%d,%s",
			&id, &x, &y, &hp, &hpBar, &atk ,&def,&Resistance) == LoadingNum)
		{
			EnemyParameter p;
			p.id = id;
			p.startPos.x = x;
			p.startPos.y = y;
			p.maxHp = hp;
			p.nowHp = hp;
			p.hpBar = hpBar;
			p.attack = atk;
			p.def = def;
			p.Resistance = Resistance;

			// マップに保存（ID 0 のデータ、ID 1 のデータ...）
			s_enemyMaster[id] = p;
		}

		fclose(fp);
	}
}

void Enemy::DrawHpBar()
{
	int BarWidth = 800, BarHeight = 20;

	//現在のHPを1本あたりのHPで割った数
	int currentBarIdx = (int)(m_EnemyParam.nowHp / m_EnemyParam.hpBar);

	float ratio = (float)((double)(m_EnemyParam.nowHp % m_EnemyParam.hpBar) / m_EnemyParam.hpBar);
	if (m_EnemyParam.nowHp > 0 && ratio == 0.0f)
	{
		ratio = 1.0f;
		currentBarIdx--;
	}
	//画面中央の一番上に表示
	Math::Vector3 barPos = { -410.0f, 300.0f, 0.0f };
	Math::Vector2 pivotLeft = { 0.0f, 1.0f }; // 左端を基準にする

	// 色リスト
	Math::Color colors[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f }, // 赤
		{ 1.0f, 0.5f, 0.0f, 1.0f }, // 橙
		{ 1.0f, 1.0f, 0.0f, 1.0f }, // 黄
		{ 0.0f, 1.0f, 0.0f, 1.0f }, // 緑
		{ 0.0f, 1.0f, 1.0f, 1.0f }  // 水
	};

	Math::Color currentColor = colors[currentBarIdx % 5];
	Math::Color bgColor = colors[(currentBarIdx - 1 + 5) % 5];
	Math::Color black = { 0.2f, 0.2f, 0.2f, 1.0f };

	//-- 残りのHPbarの本数表示 --
	int numW = 64;	//1文字の横幅
	int numH = 64;	//1文字の高さ
	int displayCount = currentBarIdx + 1;// 残りの本数を算出
	std::string countStr = std::to_string(displayCount);

	Math::Vector3 textPos = { barPos.x + BarWidth + 15.0f, barPos.y, 0.0f };
	
	// --- 描画処理 ---

	// 土台 (黒)
	SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)barPos.x, (int)barPos.y, BarWidth, BarHeight, nullptr, &black, pivotLeft);

	// 下の層 (背景色。2本目以降がある場合)
	if (currentBarIdx > 0) {
		SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)barPos.x, (int)barPos.y, BarWidth, BarHeight, nullptr, &bgColor, pivotLeft);
	}

	// 現在の層 (割合に応じて幅を可変させる)
	int drawWidth = (int)(BarWidth * ratio);

	// 画像のサイズに合わせてSrcRectを作る
	Math::Rectangle srcRect = { 0, 0, (long)(BarWidth * ratio), (long)BarHeight };

	SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)barPos.x, (int)barPos.y, drawWidth, BarHeight, &srcRect, &currentColor, pivotLeft);

	// 最大HPが1本分より多いときだけ「×」を表示
	if (m_EnemyParam.maxHp > m_EnemyParam.hpBar)
	{
		// 「×」マークを描画
		Math::Rectangle srcRect = { 0, 0, numW, numH };
		SHADER.m_spriteShader.DrawTex(&m_NumberTex, textPos.x, textPos.y, One_Half(numW), One_Half(numH), &srcRect, &currentColor, pivotLeft);

		// 次の文字の描画位置をずらす
		textPos.x += (numW * 0.5f); // 文字間隔を少し詰める調整

		for (char c : countStr)
		{
			//char型からint型に型変換
			int digit = c - '0';

			int srcX = (digit + 1) * numW;

			Math::Rectangle srcRect = { (long)srcX, 0, numW, numH };

			SHADER.m_spriteShader.DrawTex(&m_NumberTex, textPos.x, textPos.y, One_Half(numW), One_Half(numH), &srcRect, &currentColor, pivotLeft);

			textPos.x += (numW * 0.5f);
		}
	}
}

void Enemy::UpdateShot()
{
	m_shotTimer += m_deltaTime;

	if (m_shotTimer >= m_shotInterval) 
	{
		m_isAttacking = true; // 攻撃モード開始
		m_attackAnimCnt = 0;
		m_shotTimer = 0;
	}

	if (m_isAttacking)
	{
		m_attackAnimCnt += 0.15f;
		// 例：攻撃アニメーションの4枚目で弾を生成
		if (m_attackAnimCnt >= 3.0f && m_attackAnimCnt < 3.15f) {
			Shoot();
		}
		// アニメーションが終わったら通常に戻る
		if (m_attackAnimCnt >= 8.0f) {
			m_isAttacking = false;
		}
	}
}

void Enemy::Shoot()
{
	if (!m_owner) return;

	// 敵用の弾（Bulletクラスを流用、またはEnemyBulletクラスを作成）
	auto bullet = std::make_shared<Bullet>();
	bullet->Init();

	// 敵の弾であることを設定（当たり判定で必要）
	bullet->SetObjType(BaseObject::objectType::enemyBullet);

	// 左（プレイヤー方向）に飛ぶように設定
	bullet->SetPos(m_pos);
	bullet->SetDir({ -1.0f, 0.0f, 0.0f });
	bullet->SetAtk(m_EnemyParam.attack);
	bullet->SetOwner(m_owner);

	m_owner->AddObject(bullet);
}
