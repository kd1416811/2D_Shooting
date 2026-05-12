#pragma once

enum class SkillType
{
	Q,
	W,
	E,
	R,
	ULT,
	None = 999
};

class SkillGauge;

class SkillManager
{
public:

	SkillManager();
	~SkillManager();

	void Init();
	SkillType Update();
	void Draw();
	
	float GetUltMaxCost()const { return ULT_MAX_COST; }

private:

	void InitCircleVertices();

	// --- 定数・構造体定義 ---
	static constexpr int	SKILL_COUNT		= 4;
	static constexpr float	BASE_SKILL_COST = 200.0f;
	static constexpr float	ULT_MAX_COST	= 8.0f;

	// スキルごとの固有設定をまとめる構造体
	struct SkillParam {
		int				key;		// キー
		float			speed;		// たまりやすさ (chargeSpeed)
		Math::Vector2	pos;		// 表示位置
		Math::Vector2	radius;		//半径
	};

	//スキルの数だけ設定を用意する
	//ULT用の隙間をあけて描画
	const SkillParam G_SkillParam[SKILL_COUNT] = {
		{ 'Q', 2.0f,  { -250.0f , -250.0f},{30.0f,30.0f}}, // たまりやすい
		{ 'W', 1.5f,  { -150.0f , -250.0f},{30.0f,30.0f}}, // ちょいたまる
		{ 'E', 1.0f,  {	 150.0f , -250.0f},{30.0f,30.0f}}, // 普通
		{ 'R', 0.8f,  {  250.0f , -250.0f},{30.0f,30.0f}}  // たまりづらい
	};

	const SkillParam G_ULTParam = { VK_SPACE, 0 ,{ 0.0f , -250.0f } ,{ 50.0f , 50.0f } };

	//===============
	//スキル系の変数
	//===============
	float	vertexNum;
	float	startDeg;		//角度
	float	alpha;
	int		alphaDirection;
	int		Frame;

	Math::Color		color;
	Math::Matrix	m_mat;
	Math::Vector2	m_pos;
	Math::Matrix	m_homingMat;
	Math::Matrix	m_WaveMat;
	Math::Matrix	m_AtkMat;
	Math::Matrix	m_CritMat;
	KdTexture		m_costTex;
	KdTexture		m_homingTex;
	KdTexture		m_WaveTex;
	KdTexture		m_AtkTex;
	KdTexture		m_CritTex;

	//スキル実体
	std::array<std::unique_ptr<SkillGauge>, SKILL_COUNT>	m_skillGauge;
	std::unique_ptr<SkillGauge>								m_ultGauge = nullptr;
	
	Math::Vector2 m_circleVertices[361];
};