#include "DamageCalculation.h"
#include <random>

int DamageCalculation::Calculator(long long damage, int enemyDef, int enemyResist,int combo,int lv)
{
	//基礎ダメージ計算
	m_AtkDamage = (long long)((damage * Critical()) - enemyDef) * lv;

	if (m_AtkDamage < 1)m_AtkDamage = 1;

	//コンボ補正の計算
	float comboBonus = 1.0f + (combo * 0.01f);
	if (comboBonus > 2.0f) comboBonus = 2.0f; // カンスト設定


	// 攻撃強度と耐性による補正
	//最終ダメージ = 与ダメージ＊(1+(自分の攻撃強度 - 敵の耐性)/定数)
	float strengthBonus = 1.0f + ((m_AtkPower * (lv * 1.5f)) - (float)(enemyResist * lv)) / damageStatic;
	
	m_finalDamage = (long long)(m_AtkDamage * strengthBonus * comboBonus);

	// ダメージの「揺らぎ」(95% 〜 110% の間でランダムな倍率を生成)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.95f, m_maxRndRation);
	float variance = dist(gen);

	//最終ダメージ計算
	m_finalDamage = (long long)(m_finalDamage * variance);

	return (m_finalDamage < 1) ? 1 : (int)m_finalDamage;

}

float DamageCalculation::Critical()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 99);
	
	double criticalHit = 1.0f;

	if (dis(gen) < m_criticalRate)
	{
		//クリティカル発生
		//1.5(150%) + クリティカルダメージ増加%
		m_criticalFlg = true;
		CriticalJudg();

		return(1.5f + m_criticalIncreRate);
	}

	m_criticalFlg = false;
	CriticalJudg();

	return criticalHit;
}

bool DamageCalculation::CriticalJudg()
{
	if (!m_criticalFlg)
	{
		return false;
	}
	else
	{
		return true;
	}
}
