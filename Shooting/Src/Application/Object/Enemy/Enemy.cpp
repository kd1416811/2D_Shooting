#include "Enemy.h"
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
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)EnemyMargin,(int)EnemyMargin }, 1.0f);


	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	DrawHpBar();
	
}

void Enemy::Init()
{
	if (s_enemyMaster.empty()) LoadParameter();

	m_HpBarTex.Load("Textures/Enemy/HPBar.png");

	m_objType = objectType::enemy;
}

void Enemy::OnHit()
{
	m_aliveFlg = false;
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
		m_param = it->second; 

		std::string fullPath = "Textures/Enemy/" + std::string(m_param.texName);
		m_tex.Load(fullPath.c_str());


		// --- 等比インフレの計算 ---
		float Rate = 1.35f; // 1ステージごとに1.2倍にする設定
		float totalScale = std::pow(Rate, (float)(stageLevel - 1));

		// 各ステータスに倍率を適用
		// HPの更新
		m_param.maxHp = (long long)(m_param.maxHp * totalScale);
		m_param.nowHp = m_param.maxHp;

		// ゲージの厚みも増やす(HPバー1本あたりの量もインフレさせる)
		m_param.hpBar = (long long)(m_param.hpBar * totalScale); 
		
		// 攻撃力の更新
		m_param.attack = (int)(m_param.attack * totalScale);

		m_aliveFlg = true;
	}
}

void Enemy::LoadParameter()
{
	FILE* fp;

	if (fopen_s(&fp, "Data/Enemy/Enemy.csv", "r") == 0)
	{
		char dummy[255];
		int	 LoadingNum = 7;	//読み込む数だけ増やす
		int  id = 0, atk = 0;
		float x = 0, y = 0;
		long long hp = 0, hpBar = 0;
		char tName[64];

		// ヘッダー（1行目）を読み飛ばす
		fgets(dummy, sizeof(dummy), fp);

		while (fscanf_s(fp, "%d,%f,%f,%lld,%lld,%d,%s",
			&id, &x, &y, &hp, &hpBar, &atk, tName, (unsigned int)(sizeof(tName) - 1)) == LoadingNum)
		{
			tName[sizeof(tName) - 1] = '\0';

			EnemyParameter p;
			p.id = id;
			m_pos = { x,y,0.0f };
			p.maxHp = hp;
			p.nowHp = hp;
			p.hpBar = hpBar;
			p.attack = atk;
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

	int currentBarIdx = (int)(m_param.nowHp / m_param.hpBar);
	float ratio = (float)(m_param.nowHp % m_param.hpBar) / m_param.hpBar;
	if (m_param.nowHp > 0 && ratio == 0.0f)
	{
		ratio = 1.0f;
		currentBarIdx--;
	}
	//画面中央の一番上に表示
	Math::Vector3 barPos = { -380.0f, 300.0f, 0.0f };
	Math::Vector2 pivotLeft = { 0.0f, 0.5f }; // 左端を基準にする

	// 色リスト (Math::Color型で定義し直す必要があります)
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

	// --- 3. 描画処理 ---

	// 行列をリセット（画面固定UIにする場合）
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);

	// ① 土台 (黒)
	SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)barPos.x, (int)barPos.y, BarWidth, BarHeight, nullptr, &black, pivotLeft);

	// ② 下の層 (背景色。2本目以降がある場合)
	if (currentBarIdx > 0) {
		SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)barPos.x, (int)barPos.y, BarWidth, BarHeight, nullptr, &bgColor, pivotLeft);
	}

	// ③ 現在の層 (割合に応じて幅を可変させる)
	int drawWidth = (int)(BarWidth * ratio);

	// 元画像のサイズ（テクスチャの実際の解像度）に合わせてSrcRectを作る
	int texW = 800;
	int texH = 20;
	Math::Rectangle srcRect = { 0, 0, (long)(texW * ratio), (long)texH };

	SHADER.m_spriteShader.DrawTex(&m_HpBarTex, (int)barPos.x, (int)barPos.y, drawWidth, BarHeight, &srcRect, &currentColor, pivotLeft);
}
