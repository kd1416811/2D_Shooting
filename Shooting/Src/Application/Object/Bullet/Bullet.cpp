#include "Bullet.h"
#include "MoveStrategy.h"
#include"../../Object/Bullet/BulletType/MoveUltimate.h"
#include"../../Concept/define.h"

Bullet::Bullet(){}

Bullet::~Bullet() { Release(); }

void Bullet::Update()
{
	if (!m_aliveFlg)return;

	m_animCnt += 0.5f;
	if (m_animCnt >= 30)
	{
		m_animCnt = 0;
	}

	//移動
	Move();

	if (m_strategy) 
	{
		auto* ult = static_cast<MoveUltimate*>(m_strategy.get());
		if (ult && ult->IsAnimationFinished()) {
			m_aliveFlg = false;
		}
	}

	//寿命、範囲チェック
	CheckLifeSpan();

	//行列更新
	if (!m_strategy)
	{
		UpdateMatrix();
	}
}

void Bullet::Draw()
{
	if (!m_aliveFlg) return;

	SHADER.m_spriteShader.SetMatrix(m_mat);

	if (m_strategy)
	{
		 
		if (m_strategy && m_pStragegyTex)
		{
			Math::Rectangle m_strategySrcRect = { 0, 0, 256,144 };
			Math::Vector2 pivot = { 0.5f, 0.5f }; // 画像の真ん中を軸にする
			Math::Color col = { 1.0f, 1.0f, 1.0f, 1.0f }; // 元の色のまま

			// 行列に座標が含まれているので、第2, 第3引数は 0 でOK
			SHADER.m_spriteShader.DrawTex(m_pStragegyTex, 0, 0, 256, 144, &m_strategySrcRect, &col, pivot);
		
			// --- 2. チャージエフェクトの描画 (重ね書き) ---
			auto* ult = static_cast<MoveUltimate*>(m_strategy.get());

			Math::Matrix offsetMat = Math::Matrix::CreateTranslation(170.0f * 2, 0.0f, 0.0f);
			Math::Matrix effectMat = offsetMat * m_mat; // 行列を合成

			SHADER.m_spriteShader.SetMatrix(effectMat); // 雷専用の行列をセット

			// 1枚のコマサイズが例えば 128x128 の場合
			int frameY = (int)ult->GetAnimeCnt() * 140;
			Math::Rectangle effectRect = { 0, frameY, 245, 140 };

			SHADER.m_spriteShader.DrawTex(ult->GetRailgunTex(), 0, 0, 245 *3, 140, &effectRect, nullptr, pivot);
		}
		else
		{
			// ホーミングやウェイブの時は waveTex を使う
			SHADER.m_spriteShader.DrawTex(&m_waveTex, { (int)m_animCnt * (int)Config::BulletMargin_y, 0, (int)Config::BulletMargin_y, (int)Config::BulletMargin_y }, 1.0f);
		}
	}
	else {
		// 通常弾
		SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,(int)Config::BulletMargin_x,(int)Config::BulletMargin_y }, 1.0f);
	}

	//int aMax = (m_config.animeMax > 0) ? m_config.animeMax : 1;
	//int chipW = m_config.texW / aMax;
	//int chipH = m_config.texH;
	//Math::Rectangle srcRect = { m_animeIdx * chipW, 0, chipW, chipH };

	//// --- 2. 描画設定（加算合成など） ---
	//if (m_config.isAddBlend) {
	//	D3D.SetBlendState(BlendMode::Add);
	//}

	//// --- 3. 描画実行（Strategyの有無に関わらずこれ1つにする） ---
	//// 前の if(m_strategy) のブロックは消してOKです
	//SHADER.m_spriteShader.SetMatrix(m_mat);
	//SHADER.m_spriteShader.DrawTex(&m_tex, 0, 0,
	//	(int)Config::BulletMargin_x, (int)Config::BulletMargin_y,
	//	&srcRect);

	//// --- 4. 後始末 ---
	//if (m_config.isAddBlend) {
	//	D3D.SetBlendState(BlendMode::Alpha);
	//}
	
}

void Bullet::Init()
{
	m_aliveFlg = true;
	m_speed = 10.0f;
	m_radius = 10.0f;
	m_deltaTime = 1.0f / 60.0f;
	m_pos = { 0,0,0 };
	m_dir = { 1.0f,0.0f,0.0f };//右に弾が出る
	m_animCnt = 0;

	m_objType = objectType::bullet;
	m_tex.Load("Textures/Bullet/FlameBullet.png");
	m_waveTex.Load("Textures/Player/HomingAnim.png");
	//m_config = BulletConfig();
}

void Bullet::OnHit(long long damage)
{
	m_aliveFlg = false;
}

void Bullet::SetStrategy(std::unique_ptr<MoveStrategy> strategy)
{
	m_strategy = std::move(strategy);

	if (m_strategy)
	{
		MoveParam param = { m_pos, m_dir, m_speed, m_deltaTime, m_pStragegyTex, m_mat, m_radius,m_owner };
		m_strategy->Init(param);

		// Strategy側で param.tex にテクスチャがセットされたら受け取る
		m_pStragegyTex = param.p_tex;
		m_radius = param.radius;
	}
}

//void Bullet::SetBulletConfig(const BulletConfig& config)
//{
	//m_config = config;
	//m_tex.Load(m_config.texPath); // 画像を読み込み直す
//}

void Bullet::Release()
{}

void Bullet::Move()
{

	if (m_strategy)
	{
		// パラメータをまとめて戦略オブジェクトに丸投げ！
		MoveParam param = { m_pos, m_dir, m_speed, m_deltaTime, m_pStragegyTex, m_mat, m_radius,m_owner };
		m_strategy->Update(param);

		m_mat = param.m_mat;
		m_pStragegyTex = param.p_tex;
		m_radius = param.radius;
		m_pos = param.pos;    
		m_dir = param.dir;
		m_speed = param.speed;
		
		return;
	}
	
	//弾の移動処理
	m_pos += m_dir * m_speed;

	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y + BulletAddStartPos.y, 0);
	
}

void Bullet::CheckLifeSpan()
{
	int screenHalf_W = One_Half(Config::SCREEN_WIDTH);
	int screenHlaf_H = One_Half(Config::SCREEN_HEIGHT);

	if (m_pos.x < -(screenHalf_W) - Config::BulletMargin_x ||
		m_pos.x >(screenHalf_W) + Config::BulletMargin_x ||
		m_pos.y < -(screenHlaf_H) - Config::BulletMargin_y ||
		m_pos.y >(screenHlaf_H) + Config::BulletMargin_y)
	{
		m_aliveFlg = false;
	}
}

void Bullet::UpdateMatrix()
{
	//m_scale = Math::Matrix::CreateScale(m_config.scale);

	//// 進行方向（m_angleRad）を考慮して行列を作成
	//m_rotation = Math::Matrix::CreateRotationZ(m_angleRad);
	//// スケールも反映
	//
	//m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y + BulletAddStartPos.y, 0);

	//m_mat = m_scale * m_rotation * m_trans;

	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y + BulletAddStartPos.y, 0);

	m_mat =  m_trans;
}
