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

class gameScene;

class Player : public BaseObject
{
public:

	Player() {}
	~Player() override { Release(); }


	void Update() override;
	void Draw()override;
	void Init()override;

	void OnHit() override;

	// シーン情報をセット
	void SetOwner(gameScene* _owner)override { m_owner = _owner; }

private:

	void Release()override;

	void Shoot();	//発射させる関数

	//自機の基本の動き
	void UpdateMove();

	//弾を毎フレーム打つ処理＆弾の制御
	void UpdateShot();

	//自機と敵との当たり判定
	void UpdateCheakCollision();

	//画面端制御
	void UpdateMaxScreenEdge();

	//行列更新
	void UpdateMatrix();

	//弾の挙動の初期種類
	BulletType m_currentShotType = BulletType::Normal;

	static constexpr float NormalTimeRatio = 1.0f;		//1.0倍...通常倍率
};