#pragma once
//前方宣言
class gameScene;

class BaseObject
{
public:
	//オブジェクトの種類
	enum class objectType {
		None,
		player,
		enemy,
		bullet
	};

	BaseObject() {}
	virtual ~BaseObject() { Release(); }

	virtual void Update();
	virtual void Draw();
	virtual void Init();

	virtual void OnHit() {}

	virtual void SetOwner(gameScene* _owner) {}


	//==========================
	//			関数
	// =========================

	//数字を0.5倍にする
	float			Half(float value) { return value * 0.5f; }


	//===========================
	//			getter
	// ==========================

	//忘れないように
	//const ～& ...中身は書き換えないけど、コピーのコストを避けたいとき
	const objectType& GetObjType() { return m_objType; }
	const Math::Vector3& GetPos() { return m_pos; }
	const bool& GetAliveFlg() { return m_aliveFlg; }

protected:

	virtual void Release();

	static constexpr int	SCREEN_WIDTH = 1280;		//画面の端から端までの長さ
	static constexpr int	SCREEN_HEIGHT = 720;		//画面の上から下までの長さ

	static constexpr float	PlayerMargin = 64.0f;	//自機の大きさ
	static constexpr float	EnemyMargin = 64.0f;	//自機の大きさ
	static constexpr float	BulletMargin = 16.0f;	//自機の大きさ
	static constexpr float	ShotInterval = 0.2f;		//発射間隔(0.2秒)

	KdTexture		m_tex;
	Math::Matrix	m_rotation;
	Math::Matrix	m_trans;
	Math::Matrix	m_mat;
	Math::Vector3	m_pos = {};
	Math::Vector3   m_dir = {}; //移動方向
	int				m_deg;		//角度
	float			m_speed;
	float			m_shotTimer;
	float			m_deltaTime;
	bool			m_aliveFlg = true;

	objectType m_objType = objectType::None;

	gameScene* m_owner = nullptr;
};