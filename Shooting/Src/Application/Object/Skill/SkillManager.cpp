#include "SkillManager.h"
#include"SkillGauge.h"

SkillManager::SkillManager() = default;
SkillManager::~SkillManager() = default;

void SkillManager::Init()
{
	startDeg = 0;//真上からスタート
	alpha = 0.2f;
	alphaDirection = 1;

	//=========================--
	//円の頂点データの作成
	//===========================
	InitCircleVertices();

	//============================
	//通常スキルの初期化
	//============================

	for (int i = 0; i < SKILL_COUNT; i++) {

		m_skillGauge[i] = std::make_unique<SkillGauge>();

		// ここで SkillGauge のメンバに値を代入
		const auto& param = G_SkillParam[i];

		m_skillGauge[i]->Init(
			param.key, 
			BASE_SKILL_COST,
			param.pos,
			param.radius,
			{ 0,0,0,0.2f },
			GaugeType::Cooldown,
			false);

		// 個別のチャージ速度を適応
		m_skillGauge[i]->SetChargeSpeed(param.speed);
	}

	//========================
	// ULTの初期化
	//========================
	m_ultGauge = std::make_unique<SkillGauge>();

	const auto& ULTparam = G_ULTParam;

	m_ultGauge->Init(
		ULTparam.key,
		ULT_MAX_COST, 
		ULTparam.pos,
		ULTparam.radius, 
		{ 0,1,0,0.2f },
		GaugeType::Charge, 
		true);

	// ULTは自動回復しない（通常スキル使用で増える）ため、速度は0
	m_ultGauge->SetChargeSpeed(0.0f);

	Frame = 0;

	//m_costTex.Load("texture/costUI2.png");
}

void SkillManager::Update()
{
	//===========================================
	// 通常スキル用
	// ==========================================

	// 全スキルをループで回す
	for (auto& gauge : m_skillGauge)
	{
		if (gauge->Update()) 
		{
			m_ultGauge->AddCost(1.0f);
		}
	}

	//===============================================
	// ウルト用
	//=============================================== 

	//ウルトゲージを使う
	if (m_ultGauge->IsFull())
	{
		m_ultGauge->Update();
	}


	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	Frame++;
}


void SkillManager::Draw()
{
	// --- 1. 背景テクスチャの描画 ---
	//土台となる枠などを描画
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_costTex, Math::Rectangle{ 300,0,300,302 }, 1.0f);

	color = { 0,0,0,0.2f };

	// --- 2. ゲージ（中身）の描画 ---
	// ゲージはスクリーン座標で描くため、一旦行列をリセット
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);

	//===========================================
	// 通常スキル用
	// ==========================================
	for (const auto& gauge : m_skillGauge)
	{
		gauge->Draw(m_circleVertices, startDeg, false);
	}

	//==========================================
	//ウルト用
	//==========================================
	m_ultGauge->Draw(m_circleVertices, startDeg, true);


	// --- 3. 前面（飾り）の描画 ---
	// ゲージの上に重ねたい装飾やアイコンがあればここで描画
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_costTex, Math::Rectangle{ 0,0,300,302 }, 1.0f);
}

//=========================--
//円の頂点データの作成
//===========================
void SkillManager::InitCircleVertices()
{
	for (int i = 0; i <= 360; ++i) {
		float rad = DirectX::XMConvertToRadians((float)i);
		// 半径1の基本の円を作る
		m_circleVertices[i].x = sin(rad);
		m_circleVertices[i].y = cos(rad);
	}
}
