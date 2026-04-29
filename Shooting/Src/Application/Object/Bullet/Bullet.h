#pragma once
#include"../BaseObject.h"

enum class BulletType
{
	Normal,
	homing,
	Wave,
	Laser
};

class Bullet: public BaseObject
{
public:

	Bullet();
	~Bullet() override { Release(); }

	// 初期化用：発射位置と方向を受け取る
	void Init(const Math::Vector2& startPos, const Math::Vector2& dir, float speed);

	void Update() override;
	void Draw() override;

private:

	static constexpr float BulletMargin = 16.0f;	//弾の大きさ

	void Release() override;

	// 弾固有の移動ロジック
	void Move();

	// 画面外判定などの寿命管理
	void CheckLifeSpan();
};