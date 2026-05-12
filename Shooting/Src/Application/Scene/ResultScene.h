#pragma once
#include"BaseScene.h"

class SkillManager;
class DamageCalculation;
class Bullet;

class ResultScene :public BaseScene
{
public:

	ResultScene() ;
	~ResultScene();

	void Init()override;
	void Update()override;
	void Draw()override;//2D描画
	void Release()override;

	void DrawNumber(const KdTexture& tex, const KdTexture& tex2, int value,int add, Math::Vector2 startPos,float sabWidth,float sabHeight, float scale,float mojiScale,bool b_Multi,bool b_SecTex ,bool b_persent);

private:
	std::shared_ptr<DamageCalculation> m_tempCalc = nullptr; // 計算機を1つ持つ
	std::shared_ptr<SkillManager> m_skillManager = nullptr; // スキルマネージャーを1つ持つ

	KdTexture m_tex;
	KdTexture m_AudioTex;
	KdTexture m_numberTex;
	KdTexture m_materialTex;
	KdTexture m_costTex;
	KdTexture m_bookTex;
	KdTexture m_statusTex;
	KdTexture m_statusListTex;
	KdTexture m_BaseStatusTex;
	KdTexture m_subStatusTex;
	KdTexture m_atkTex;
	KdTexture m_hpTex;
	KdTexture m_defTex;
	KdTexture m_criticalTex;
	KdTexture m_cloverTex;
	KdTexture m_atkPowertex;
	KdTexture m_bulletSpeedtex;
	KdTexture m_burstRateTex;
	KdTexture m_criticalDmgTex;
	KdTexture m_lvTex;
	KdTexture m_stageTex;
	
	bool AkeyFlg = false;
};