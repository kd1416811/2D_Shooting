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

	void OnHit() override;

	// シーン情報をセット
	void SetOwner(gameScene* _owner) override { m_owner = _owner; }


	void SetPos(Math::Vector3& pos) { m_pos = pos; }//playerの座標を受け取る


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
};