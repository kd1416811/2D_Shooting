#pragma once 
#include"../BaseObject.h"

class DamageEffect;

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

	void OnHit(long long damage) override;

	// ★ IDを指定してステータスを自分にコピーする関数
	void SetType(int id,int stageLevel);

	// シーン情報をセット
	void SetOwner(gameScene* _owner)override { m_owner = _owner; }

private:

	void Release()override;

	//csvからid読み込み関数
	void LoadParameter();

	//HPBarの表示
	void DrawHpBar();

	// IDをキーにしてデータを保存
	static std::map<int, EnemyParameter> s_enemyMaster; 

	// この個体のステータス
	EnemyParameter m_EnemyParam;

	KdTexture m_HpBarTex;
	KdTexture m_NumberTex;

	long long m_AtkDamage;//与ダメージ
};