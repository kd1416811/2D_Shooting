#pragma once
#include"BaseScene.h"

// 強化項目の情報をまとめる構造体
struct UpgradeItem 
{
	Math::Vector2 pos;      // 黒枠を表示する座標
	int level;              // 現在のレベル
	int baseCost;           // 初期コスト
	std::string name;       // 項目名（デバッグ・管理用）
};

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

	void DrawNumber(const KdTexture& tex, const KdTexture& tex2, int value,int add, Math::Vector2 startPos,Math::Color col,float sabWidth,float sabHeight, float scale,float mojiScale,bool b_Multi,bool b_SecTex ,bool b_persent);

private:

	void HandleInput();                // 入力処理（Updateから呼ぶ）
	void LevelUp(int idx);             // レベルアップ実行関数
	int  GetNeedCost(int idx);         // 必要コスト計算関数

	std::shared_ptr<DamageCalculation> m_tempCalc = nullptr; // 計算機を1つ持つ
	std::shared_ptr<SkillManager> m_skillManager = nullptr; // スキルマネージャーを1つ持つ

	// --- 強化システム用の追加変数 ---
	int m_selectIdx = 0;               // 現在選択中の番号 (0～9)
	const int MAX_UPGRADE = 10;        // 強化項目の総数
	std::vector<UpgradeItem> m_upgrades; // 強化項目のリスト
	KdTexture m_cursorTex;//黒枠用

	float m_statScales[9] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	float m_statShake = 0.0f;

	KdTexture m_tex;
	KdTexture m_AudioTex;
	KdTexture m_numberTex;
	KdTexture m_materialTex;
	KdTexture m_costTex;
	KdTexture m_bookTex;
	KdTexture m_statusTex;
	KdTexture m_statusListTex;

	KdTexture m_BaseStatusTex;
	KdTexture m_BaseStatusSecTex;
	KdTexture m_subStatusTex;
	KdTexture m_subStatusSecTex;

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
	KdTexture m_statusPowerTex;
	KdTexture m_enterTex;
	KdTexture m_playGameTex;
	KdTexture m_BackSpaceTex;
	KdTexture m_backTitleTex;
	
	bool AkeyFlg = false;
};