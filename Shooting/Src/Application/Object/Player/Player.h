#pragma once
#include "../BaseObject.h"
#include <Application/Object/Skill/SkillManager.h>

struct PlayerParameter
{
	int			id;
	Math::Vector3 startPos;		//初期座標
	long long	maxHp;			//最大のHP
	long long	nowHp;			//現在の残りHP
	long long	hpBar;			//ゲージ1本あたりのHP量
	long long   atk;
	int			def;
	int			Lv;			//プレイヤーレベル
};

class GameScene;

class Player : public BaseObject
{
public:

	Player() {}
	~Player() override { Release(); }


	void Update() override;
	void Draw()override;
	void Init()override;

	void OnHit(long long damage) override;

	void SetType(int id, int stageLevel);

	void ShootSkill(SkillType type);

	// シーン情報をセット
	void SetOwner(GameScene* _owner)override { m_owner = _owner; }

	// 発射間隔をセットする関数
	void SetShotInterval(float interval) { m_shotInterval = interval; }

	void AddCombo() { m_comboCount++; m_comboTimer = 2.0f; } // 2秒以内に次を当てれば継続
	void ResetCombo() { m_comboCount = 0; }
	int GetComboCount(){ return m_comboCount; }
	int GetPlayerLv()const  { return m_PlayerParam.Lv; }
	int GetDef() const { return m_PlayerParam.def; }
	long long GetHp() const { return m_PlayerParam.nowHp; }
	long long GetAtk() const { return m_PlayerParam.atk; }

	// 発射間隔を取得する関数
	float GetShotInterval() const { return m_shotInterval; }

	bool IsAtkBuffActive() const { return m_isBuffActive; }
	bool IsCritBuffActive() const { return m_isCritBuffActive; }

private:

	void Release()override;

	void Shoot();	//発射させる関数

	//自機の基本の動き
	void UpdateMove();

	//弾を毎フレーム打つ処理＆弾の制御
	void UpdateShot();

	//画面端制御
	void UpdateMaxScreenEdge();

	//行列更新
	void UpdateMatrix();

	void LoadParameter();

	void DrawHpBar();

	
	// IDをキーにしてデータを保存
	static std::map<int, PlayerParameter> s_playerMaster;
	PlayerParameter m_PlayerParam;

	static constexpr float NormalTimeRatio = 1.0f;				//1.0倍...通常倍率
	const Math::Vector3 HpBarAddPos = { 30.0f,40.0f,0.0f };		//プレイヤーの位置からどれだけずらすか

	int m_comboCount;
	float m_comboTimer;

	float m_buffTimer = 0.0f;       // 強化残り時間
	bool  m_isBuffActive = false;   // 強化中フラグ

	float m_critBuffTimer = 0.0f;    // クリティカル強化残り時間
	bool  m_isCritBuffActive = false; // クリティカル強化中フラグ

	KdTexture m_HpBarTex;
};