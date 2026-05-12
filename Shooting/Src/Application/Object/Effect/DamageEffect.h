#pragma once
#include "../BaseObject.h"

class DamageEffect:public BaseObject
{
public:

	DamageEffect(){}
	~DamageEffect(){}

	void Init() override;
	void Draw()override;
	void Update()override;

	void SetDamage(long long damage, const Math::Vector3& pos, bool b_critical,float speedRate = 1.0f);

	void SetColor(const Math::Color& col) { m_color = col; }	

private:

	//ダメージ数値にコンマを付ける関数(1000000→1,000,000)
	std::string FormatComma(long long value);

	long long m_damage;
	bool m_criticalFlg;

	Math::Color m_color;

	static KdTexture s_numberTex;
};