#pragma once

class BaseObject
{
public:

	BaseObject();
	virtual ~BaseObject() { Release(); }

	virtual void Update();
	virtual void Draw();
	virtual void Init();

	//getter
	bool GetAliveFlg() const { return m_aliveFlg; }

	//setter
	//数字を0.5倍にする関数
	float Half(float value) { return value * 0.5f; }

protected:

	virtual void Release();

	static constexpr int SCREEN_WIDTH = 1280;						//画面の端から端までの長さ
	static constexpr int SCREEN_HEIGHT = 720;						//画面の上から下までの長さ
	
	static constexpr float SHOT_INTERVAL = 0.2f;	//発射間隔(0.2秒)

	KdTexture		m_tex;
	Math::Matrix	m_rotation;
	Math::Matrix	m_trans;
	Math::Matrix	m_mat;
	Math::Vector2	m_pos = {};
	Math::Vector2   m_dir = {}; //移動方向
	float			m_shotTimer;
	float			m_speed;
	float			m_deltaTime;
	bool			m_aliveFlg = true;
};