#pragma once

class DamageCalculation
{
public:

	DamageCalculation() { m_criticalFlg = false; }
	~DamageCalculation(){}

	//最終ダメージ係数を返す関数
	int Calculator(long long damage,int enemyDef,int enemyResist,int combo,int lv);

	//クリティカルダメージを返す
	float Critical();

	//クリティカル判定
	bool CriticalJudg();

private:

	static constexpr int damageStatic = 1000;//定数

	//初期クリティカル発生確率(30%)
	float m_criticalRate = 30.0f;

	//初期クリティカル増加率
	float m_criticalIncreRate = 0.0f;

	//初期攻撃強度
	float m_AtkPower = 100.0f;

	//初期ランダム倍率
	float m_maxRndRation = 1.10f;

	long long m_AtkDamage = 0;	//与ダメージ格納変数
	long long m_finalDamage = 0;//最終ダメージ格納変数

	bool	m_criticalFlg;
};