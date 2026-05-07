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

	void SetDamage(long long damage, const Math::Vector3& pos);

private:

	//ダメージ数値にコンマを付ける関数(1000000→1,000,000)
	std::string FormatComma(long long value);

	long long m_damage;

	static KdTexture s_numberTex;
};