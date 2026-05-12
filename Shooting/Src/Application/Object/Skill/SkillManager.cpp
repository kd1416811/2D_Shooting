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
			{ 0.0f,0.0f,0.0f,0.5f },
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

	// ULTは自動回復しない（通常スキル使用で増える）ため、速度は0に固定する
	m_ultGauge->SetChargeSpeed(0.0f);

	Frame = 0;

	m_costTex.Load("Textures/costUI2.png");
	m_homingTex.Load("Textures/Homing2.png");
	m_WaveTex.Load("Textures/Wave2.png");
	m_AtkTex.Load("Textures/Atk2.png");
	m_CritTex.Load("Textures/Critical2.png");
}

SkillType SkillManager::Update()
{


	SkillType ActiveType = SkillType::None;

	//===========================================
	// 通常スキル用
	// ==========================================

	// 全スキルをループで回す
	for (int i = 0; i < SKILL_COUNT; i++)
	{
		if (m_skillGauge[i]->Update())
		{
			m_ultGauge->AddCost(1.0f);

			// インデックスに応じてスキルタイプを設定
			if (i == 0) ActiveType = SkillType::Q;
			if (i == 1) ActiveType = SkillType::W;
			if (i == 2) ActiveType = SkillType::E;
			if (i == 3) ActiveType = SkillType::R;
		}
	}

	//===============================================
	// ウルト用
	//=============================================== 
	const auto& ULTparam = G_ULTParam;

	//ウルトゲージを使う
	if (m_ultGauge->IsFull())
	{
		if (m_ultGauge->Update())
		{
			ActiveType = SkillType::ULT;
		}
	}


	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y + ULTparam.pos.y, 0);
	m_homingMat = Math::Matrix::CreateTranslation(m_pos.x - 250, m_pos.y + ULTparam.pos.y, 0);
	m_WaveMat = Math::Matrix::CreateTranslation(m_pos.x -150, m_pos.y + ULTparam.pos.y, 0);
	m_AtkMat = Math::Matrix::CreateTranslation(m_pos.x + 150, m_pos.y + ULTparam.pos.y, 0);
	m_CritMat = Math::Matrix::CreateTranslation(m_pos.x + 250, m_pos.y + ULTparam.pos.y, 0);

	Frame++;

	return ActiveType;
}


void SkillManager::Draw()
{
	// --- 1. 背景テクスチャの描画 ---
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_costTex, Math::Rectangle{ 100,0,100,100 }, 1.0f);
	SHADER.m_spriteShader.SetMatrix(m_homingMat);
	SHADER.m_spriteShader.DrawTex(&m_homingTex, Math::Rectangle{ 0,0,80,80 }, 1.0f);
	SHADER.m_spriteShader.SetMatrix(m_WaveMat);
	SHADER.m_spriteShader.DrawTex(&m_WaveTex, Math::Rectangle{ 0,0,80,80 }, 1.0f);
	SHADER.m_spriteShader.SetMatrix(m_AtkMat);
	SHADER.m_spriteShader.DrawTex(&m_AtkTex, Math::Rectangle{ 0,0,80,80 }, 1.0f);
	SHADER.m_spriteShader.SetMatrix(m_CritMat);
	SHADER.m_spriteShader.DrawTex(&m_CritTex, Math::Rectangle{ 0,0,80,80 }, 1.0f);

	//color = { 0,0,0,1.0f };

	// --- 2. ゲージ（中身）の描画 ---
	// 行列をリセット
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
	SHADER.m_spriteShader.DrawTex(&m_costTex, Math::Rectangle{ 0,0,100,100 }, 1.0f);
	
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
