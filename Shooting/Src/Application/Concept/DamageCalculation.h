#pragma once

class DamageCalculation
{
public:

	DamageCalculation() { m_criticalFlg = false; }
	~DamageCalculation(){}

	//最終ダメージ係数を返す関数
	int Calculator(long long damage, int enemyDef, int enemyResist, int combo, int lv, long long playerHp, long long enemyHp,bool isCritical);

	//クリティカルダメージを返す
	float Critical();

	//クリティカル判定
	bool CriticalJudg();

	// --- ゲッター関数（値を見るだけ） ---
	float GetCriticalRate() const { return m_criticalRate; }
	float GetCriticalIncreRate() const { return m_criticalIncreRate; }
	float GetAtkPower() const { return m_AtkPower; }
	float GetMaxRndRation() const { return m_maxRndRation; }
	int GetMaxRndInt() const { return m_maxRndInt; }

	// --- セッター関数（値を更新する） ---
	void SetCriticalRate(float rate) { m_criticalRate = rate; }
	void SetCriticalIncreRate(float rate) { m_criticalIncreRate = rate; }
	void SetAtkPower(float power) { m_AtkPower = power; }
	void SetMaxRndRation(float ratio) { m_maxRndRation = ratio; }

private:

	static constexpr int damageStatic = 1000;//定数

	//初期クリティカル発生確率(30%)
	float m_criticalRate = 30.0f;

	//初期クリティカル増加率
	float m_criticalIncreRate = 0.0f;

	//初期攻撃強度
	float m_AtkPower = 100.0f;

	//初期ランダム倍率..幸運クリティカル
	float m_maxRndRation = 1.10f;
	int	  m_maxRndInt = 110;//初期値..110％

	long long m_AtkDamage = 0;	//与ダメージ格納変数
	long long m_finalDamage = 0;//最終ダメージ格納変数

	//クリティカルしているかどうか
	bool	m_criticalFlg;
};