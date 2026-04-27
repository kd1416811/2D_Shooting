#pragma once

class SkillGauge;

class SkillManager
{
public:

	SkillManager();
	~SkillManager();

	void Init();
	void Update();
	void Draw();
	

private:

	void InitCircleVertices();

	float deg;		//角度

	Math::Color color;
	float alpha;
	int alphaDirection;

	KdTexture m_costTex;
	Math::Matrix m_mat;
	Math::Vector2 m_pos;

	//===============
	//スキル系の変数
	//===============
	float vertexNum;

	static const int SKILL_COUNT = 4;
	static constexpr float BASE_SKILL_COST = 200.0f;
	static constexpr float ULT_MAX_COST = 12.0f;

	std::array<std::unique_ptr<SkillGauge>, SKILL_COUNT> m_skillGauge;
	std::unique_ptr<SkillGauge>m_ultGauge;
	
	Math::Vector2 m_circleVertices[361];


	// スキルごとの固有設定をまとめる構造体
	struct SkillParam {
		int   key;      // キー
		float speed;    // たまりやすさ (chargeSpeed)
		Math::Vector2 pos;// 表示位置
		Math::Vector2 radius; //半径
	};

	// スキルの数だけ設定を用意する
	//ULT用の隙間をあけて描画
	const SkillParam G_SkillParam[SKILL_COUNT] = {
		{ 'Y', 2.0f, { -250.0f , -250.0f},{50.0f,50.0f}}, // たまりやすい
		{ 'U', 1.5f, { -150.0f , -250.0f},{50.0f,50.0f}}, // ちょいたまる
		{ 'O', 1.0f, { 150.0f , -250.0f},{50.0f,50.0f}}, // 普通
		{ 'P', 0.8f,  { 250.0f , -250.0f},{50.0f,50.0f}}  // たまりづらい
	};

	const SkillParam G_ULTParam = { 'I',0,{0.0f,-250.0f } ,{70.0f,70.0f} };

	int Frame;
};