#pragma once
#include"../BaseObject.h"

//前方宣言
class gameScene;

class Bullet :public BaseObject
{
public:

	Bullet() {}
	~Bullet() { Release(); }

	void Update() override;
	void Draw()override;
	void Init()override;

	void OnHit(long long damage) override;

	// シーン情報をセット
	void SetOwner(gameScene* _owner) override { m_owner = _owner; }


	void SetPos(Math::Vector3& pos) { m_pos = pos; }//playerの座標を受け取る
	void SetAtk(long long atk) { m_atk = atk; } // ★プレイヤーの攻撃力をもらうための関数

private:

	void Release()override;

	// 弾固有の移動ロジック
	void Move();

	// 画面外判定などの寿命管理
	void CheckLifeSpan();

	//弾と敵との当たり判定
	void CheakCollision();

	//行列更新
	void UpdateMatrix();

	long long m_atk = 0; // ★弾が持つ攻撃力
};