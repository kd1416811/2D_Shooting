#pragma once
// ゲージの挙動タイプを定義
enum class GaugeType {
	Charge,   // 0から溜まる（増加）【時計回り】true
	Cooldown  // Maxから減る（減少）【反時計回り】false
};

class SkillGauge
{
public:

	SkillGauge() = default;
	~SkillGauge() = default;

	void Init(int key, int max, Math::Vector2 p, Math::Vector2 radius,
		Math::Color col,GaugeType type, bool keyFlg);

	bool Update();
	void Draw(const Math::Vector2* vertices, float startDeg, bool showMemory) const;

	// 外部からコストを直接操作（ULT用など）
	void AddCost(float amount);
	bool IsFull() const { return m_nowCost >= m_maxCost; }


	//===============================
	//getter
	//===============================


	//===============================
	//setter
	//===============================
	void SetChargeSpeed(float speed) { m_chargeSpeed = speed; }

private:

	GaugeType m_type = GaugeType::Charge;

	Math::Vector2 m_pos;
	Math::Vector2 m_radius;
	Math::Color m_color;
	int m_keyCode;
	float m_nowCost;
	float m_maxCost;
	float m_chargeSpeed;
	bool m_keyFlg;
};