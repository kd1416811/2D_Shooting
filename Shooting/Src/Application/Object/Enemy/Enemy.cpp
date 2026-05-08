#include "Enemy.h"
#include"../Effect/DamageEffect.h"
#include"../../Scene/GameScene.h"
#include"../../Concept/define.h"
#include<string.h>
std::map<int, EnemyParameter> Enemy::s_enemyMaster;

Enemy::Enemy() = default;

void Enemy::Update()
{
	if (!m_aliveFlg)return;

	//m_param.nowHp;

	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = m_trans;
}

void Enemy::Draw()
{
	if (!m_aliveFlg)return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)Config::EnemyMargin,(int)Config::EnemyMargin }, 1.0f);
	
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	DrawHpBar();
}

void Enemy::Init()
{
	if (s_enemyMaster.empty()) LoadParameter();
	m_NumberTex.Load("Textures/number2.png");
	m_objType = objectType::enemy;
}

void Enemy::OnHit(long long damage, bool b_critical)
{
	m_EnemyParam.nowHp -= damage;

	//　--ダメージ数値表示--
	if (m_owner)
	{
		//インスタンス生成
		auto effect = std::make_shared<DamageEffect>();
	//初期化
		effect->Init();
		effect->SetOwner(m_owner);
		effect->SetDamage(damage, m_pos,b_critical);
		m_owner->AddObject(effect);
	}

	if (m_EnemyParam.nowHp <= 0)
	{
		m_EnemyParam.nowHp = 0;
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

		std::string fullPath = "Textures/Enemy/" + std::string(m_EnemyParam.texName);
		m_tex.Load(fullPath.c_str());
		m_HpBarTex.Load("Textures/Enemy/HPBar.png");

		// --- 等比インフレの計算 ---
		float Rate = 1.35f; // 1ステージごとに1.35倍にする設定
		float totalScale = std::pow(Rate, (float)(stageLevel - 1));

		// 各ステータスに倍率を適用
		// HPの更新
		m_EnemyParam.maxHp = (long long)(m_EnemyParam.maxHp * (totalScale + 0.25f));//1.6倍
		m_EnemyParam.nowHp = m_EnemyParam.maxHp;

		// ゲージの厚みも増やす(HPバー1本あたりの量もインフレさせる)
		m_EnemyParam.hpBar = (long long)(m_EnemyParam.hpBar * (totalScale + 0.25f));//1.6倍
		
		// 攻撃力の更新
		m_EnemyParam.attack = (int)(m_EnemyParam.attack * totalScale);//1.35倍

		m_aliveFlg = true;
	}
}

void Enemy::LoadParameter()
{
	FILE* fp;

	if (fopen_s(&fp, "Data/Enemy/Enemy.csv", "r") == 0)
	{
		char dummy[255];
		int	 LoadingNum = 9;	//読み込む数だけ増やす
		int  id = 0, atk = 0, def = 0, Resistance = 0;
		float x = 0, y = 0;
		long long hp = 0, hpBar = 0;
		char tName[64];

		// ヘッダー（1行目）を読み飛ばす
		fgets(dummy, sizeof(dummy), fp);

		while (fscanf_s(fp, "%d,%f,%f,%lld,%lld,%d,%d,%d,%s",
			&id, &x, &y, &hp, &hpBar, &atk ,&def,&Resistance, tName, (unsigned int)(sizeof(tName) - 1)) == LoadingNum)
		{
			tName[sizeof(tName) - 1] = '\0';

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
			strcpy_s(p.texName, sizeof(p.texName), tName);

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
	Math::Vector3 barPos = { -380.0f, 300.0f, 0.0f };
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
	Math::Color black = { 0.0f, 0.0f, 0.0f, 1.0f };

	//-- 残りのHPbarの本数表示 --
	int numW = 64;	//1文字の横幅
	int numH = 64;	//1文字の高さ
	int halfNumW = numW * 0.5f;
	int halfNumH = numH * 0.5f;
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
	int texW = 800;
	int texH = 20;
	Math::Rectangle srcRect = { 0, 0, (long)(texW * ratio), (long)texH };

	SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)barPos.x, (int)barPos.y, drawWidth, BarHeight, &srcRect, &currentColor, pivotLeft);

	// 最大HPが1本分より多いときだけ「×」を表示
	if (m_EnemyParam.maxHp > m_EnemyParam.hpBar)
	{
		// 「×」マークを描画
		Math::Rectangle srcRect = { 0, 0, numW, numH };
		SHADER.m_spriteShader.DrawTex(&m_NumberTex, textPos.x, textPos.y, halfNumW, halfNumH, &srcRect, &currentColor, pivotLeft);

		// 次の文字の描画位置をずらす
		textPos.x += (numW * 0.5f); // 文字間隔を少し詰める調整

		for (char c : countStr)
		{
			//char型からint型に型変換
			int digit = c - '0';

			int srcX = (digit + 1) * numW;

			Math::Rectangle srcRect = { (long)srcX, 0, numW, numH };

			SHADER.m_spriteShader.DrawTex(&m_NumberTex, textPos.x, textPos.y, halfNumW, halfNumH, &srcRect, &currentColor, pivotLeft);

			textPos.x += (numW * 0.5f);
		}
	}
}
