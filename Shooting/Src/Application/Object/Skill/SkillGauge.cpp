#include "SkillGauge.h"

void SkillGauge::Init(int key, int max, Math::Vector2 p, Math::Vector2 radius, Math::Color col, GaugeType type, bool keyFlg)
{
	m_keyCode = key;      // キー設定
	m_maxCost = max;      // 最大値設定
	m_pos = p;        // 座標設定
	m_radius = radius;	//半径
	m_color = col;      // 色設定
	m_keyFlg = keyFlg;     // 最初は入力可能な状態

	m_type = type;

	// 初期状態のリセット
	m_nowCost = (m_type == GaugeType::Charge) ? 0.0f : m_maxCost;
}

bool SkillGauge::Update()
{
	// コスト回復
	if (m_type == GaugeType::Charge)
	{
		// 【増加タイプ】0からMaxへ
		if (m_nowCost < m_maxCost)
		{
			m_nowCost += m_chargeSpeed;
			if (m_nowCost > m_maxCost) m_nowCost = m_maxCost;
		}
	}
	else {
		// 【減少タイプ】Maxから0へ
		if (m_nowCost > 0)
		{
			m_nowCost -= m_chargeSpeed;
			if (m_nowCost < 0) m_nowCost = 0;
		}
	}

	// 入力判定
	bool isReady = (m_type == GaugeType::Charge) ? (m_nowCost >= m_maxCost) : (m_nowCost <= 0);

	if (isReady) {
		if ((GetAsyncKeyState(m_keyCode) & 0x8000))
		{
			if (m_keyFlg)
			{
				// 発動後のリセット処理も分岐
				m_nowCost = (m_type == GaugeType::Charge) ? 0.0f : m_maxCost;
				m_keyFlg = false;
				return true;	// 「スキル発動」を通知
			}
		}
		else {
			m_keyFlg = true;
		}
	}
	return false;
}

void SkillGauge::Draw(const Math::Vector2* vertices, float startDeg, bool showMemory) const
{
	// 1. ゲージの「塗り」部分

	float ratio = (m_nowCost / m_maxCost);
	int targetDeg = (int)(360.0f * ratio);

	for (int i = 0; i < targetDeg; ++i)
	{
		int degCurrent, degNext;

		if (m_type == GaugeType::Charge) {
			// 【時計回り】足していく
			degCurrent = ((int)startDeg + i) % 360;
			degNext = ((int)startDeg + (i + 1)) % 360;
		}
		else if(m_type == GaugeType::Cooldown){
			// % 360 で負の数にならないよう +360 してから余りを出す
			// 【反時計回り】引いていく
			degCurrent = ((int)startDeg - i + 360) % 360;
			degNext = ((int)startDeg - (i + 1) + 360) % 360;
		}

		SHADER.m_spriteShader.DrawTriangle(
			m_pos.x + vertices[degCurrent].x * m_radius.x,
			m_pos.y + vertices[degCurrent].y * m_radius.y,
			m_pos.x + vertices[degNext].x * m_radius.x,
			m_pos.y + vertices[degNext].y * m_radius.y,
			m_pos.x, m_pos.y,
			&m_color, true
		);
	}

	// 2. 「目盛り」部分 (先ほどのCostMemoryのロジックを統合)
	if (showMemory)
	{
		// 半径の 20% を目盛りの長さにする
		float tickLength = m_radius.x * 0.2f;
		Math::Color ULTLineCol{ 0, 0, 0, 0.7f };

		for (int i = 0; i < (int)m_maxCost; i++) {
			float currentDeg = (360.0f / m_maxCost * i) + startDeg;
			float rad = DirectX::XMConvertToRadians(currentDeg);

			float xOuter = m_pos.x + sin(rad) * m_radius.x;
			float yOuter = m_pos.y + cos(rad) * m_radius.y;
			float xInner = m_pos.x + sin(rad) * (m_radius.x - tickLength);
			float yInner = m_pos.y + cos(rad) * (m_radius.y - tickLength);

			SHADER.m_spriteShader.DrawLine(xOuter, yOuter, xInner, yInner, &ULTLineCol);
		}
	}
}

void SkillGauge::AddCost(float amount)
{
	// スキルが使われたらULTをチャージする
	m_nowCost += amount;
	if (m_nowCost > m_maxCost)
	{
		m_nowCost = m_maxCost;
	}


	if (m_nowCost < 0.0f) {
		m_nowCost = 0.0f;
	}
}
