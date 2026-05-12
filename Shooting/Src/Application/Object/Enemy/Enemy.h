#pragma once 
#include"../BaseObject.h"

class GameScene;

enum enemyType
{
	Boss_1,
	Boss_2,
	Boss_3
};

struct EnemyParameter 
{
	int			id;				//敵のナンバー
	Math::Vector3 startPos;		//初期座標
	long long	maxHp;			//最大のHP
	long long	nowHp;			//現在の残りHP
	long long	hpBar;			//ゲージ1本あたりのHP量
	int			attack;			//攻撃力
	int			def;			//守備力
	int			Resistance;		//攻撃耐性
	char		texName[64];	//敵の画像フォルダ名格納
};

class Enemy :public BaseObject
{
public:

	Enemy();
	~Enemy()override { Release(); }

	void Update() override;
	void Draw()override;
	void Init()override;

	//void OnHit(long long damage) override;
	void OnHit(long long damage, bool b_critical);

	// ★ IDを指定してステータスを自分にコピーする関数
	void SetType(int id,int stageLevel);

	// シーン情報をセット
	void SetOwner(GameScene* _owner)override { m_owner = _owner; }

	//攻撃力を取得
	int GetAtk() const { return m_EnemyParam.attack; }

	// 防御力を取得
	int GetDef() const { return m_EnemyParam.def; }

	//体力取得
	long long GetHp() const { return m_EnemyParam.nowHp; }

	// 耐性を取得
	int GetResist() const { return m_EnemyParam.Resistance; }

	bool IsInvincible() const { return m_invincibleTimer > 0; }

private:

	void Release()override;

	//csvからid読み込み関数
	void LoadParameter();

	//HPBarの表示
	void DrawHpBar();

	void UpdateShot(); // 弾の発射管理

	void Shoot();      // 弾を生成する

	float m_invincibleTimer = 0.0f; // 無敵残り時間

	// IDをキーにしてデータを保存
	static std::map<int, EnemyParameter> s_enemyMaster;

	// この個体のステータス
	EnemyParameter m_EnemyParam;

	KdTexture m_HpBarTex;
	KdTexture m_NumberTex;

	Math::Vector3 m_moveDir;      // 現在の移動方向
	float         m_moveTimer = 0.0f; // 次に方向を変えるまでの時間
	float         m_moveSpeed = 2.0f; // 移動速度（お好みで調整）

	bool  m_isAttacking = false; // 攻撃アニメーション中か
	float m_attackAnimCnt = 0.0f;
	KdTexture m_attackTex;
};