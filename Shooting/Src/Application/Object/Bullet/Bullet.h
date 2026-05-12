#pragma once
#include"../BaseObject.h"


//struct BulletConfig
//{
//	std::string texPath = ""; // 画像パス
//	int   animeMax = 1;      // アニメーション最大コマ数
//	float animeInterval = 0.1f; // アニメーション速度
//	float scale = 1.0f;      // 表示サイズ倍率
//	bool  isAddBlend = false; // 加算合成（光るエフェクト）にするか
//	int texW = 48;
//	int texH = 32;
//};

//前方宣言
class GameScene;
class MoveStrategy;

class Bullet :public BaseObject
{
public:

	Bullet();
	~Bullet();

	void Update() override;
	void Draw()override;
	void Init()override;

	void OnHit(long long damage) override;

	//行列更新
	void UpdateMatrix();

	// シーン情報をセット
	void SetOwner(GameScene* _owner) override { m_owner = _owner; }

	void SetStrategy(std::unique_ptr<MoveStrategy> strategy);

	//void SetBulletConfig(const BulletConfig& config);

	void SetPos(const Math::Vector3& pos) { m_pos = pos; }//playerの座標を受け取る
	void SetDir(const Math::Vector3& dir) { m_dir = dir; } // playerの向いている方向を受け取る
	void SetAtk(long long atk) { m_atk = atk; } // ★プレイヤーの攻撃力をもらうための関数

	//弾の速さをセットする
	void SetBulletSpeed(float speed) { m_speed = speed; }


	//弾の攻撃力を取得する
	long long GetAtk() const { return m_atk; }

	float GetRadius() const { return m_radius; }

	bool HasStrategy() const { return m_strategy != nullptr; }

	Math::Vector2 GetAddPos() const { return BulletAddStartPos; } // ★弾の初期位置を少し前に出すための関数}

private:

	void Release()override;

	// 弾固有の移動ロジック
	void Move();

	// 画面外判定などの寿命管理
	void CheckLifeSpan();


	std::unique_ptr<MoveStrategy> m_strategy = nullptr;

	const Math::Vector2 BulletAddStartPos = { 0.0f,10.0f };

	long long m_atk = 0; // ★弾が持つ攻撃力

	float m_radius;

	KdTexture* m_pStragegyTex = nullptr; // Strategyから受け取ったテクスチャ用ポインタ
	KdTexture m_waveTex; // ウェイブ弾のテクスチャ

	//BulletConfig m_config;
	//int   m_animeIdx = 0;      // 現在のコマ
	//float m_animeTimer = 0.0f; // タイマー
	//float m_angleRad = 0.0f;   // 進行方向の角度
};