#pragma once
#include "../BaseObject.h"

//弾の挙動種類
enum class BulletType
{
	Normal,
	homing,
	Wave,
	Laser
};

struct PlayerParameter
{
	int			id;
	Math::Vector3 startPos;		//初期座標
	long long	maxHp;			//最大のHP
	long long	nowHp;			//現在の残りHP
	long long   atk;
	int			def;
	int			Lv;			//プレイヤーレベル
};

class gameScene;

class Player : public BaseObject
{
public:

	Player() {}
	~Player() override { Release(); }


	void Update() override;
	void Draw()override;
	void Init()override;

	void OnHit(long long damage) override;

	void SetType(int id);

	// シーン情報をセット
	void SetOwner(gameScene* _owner)override { m_owner = _owner; }

	void AddCombo() { m_comboCount++; m_comboTimer = 2.0f; } // 2秒以内に次を当てれば継続
	void ResetCombo() { m_comboCount = 0; }
	int GetComboCount(){ return m_comboCount; }
	int GetPlayerLv() { return m_PlayerParam.Lv; }


	//発射間隔時間を受け取る
	

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

	//弾の挙動の初期種類
	BulletType m_currentShotType = BulletType::Normal;

	// IDをキーにしてデータを保存
	static std::map<int, PlayerParameter> s_playerMaster;
	PlayerParameter m_PlayerParam;

	static constexpr float NormalTimeRatio = 1.0f;		//1.0倍...通常倍率

	int m_comboCount;
	float m_comboTimer;
};