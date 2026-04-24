#pragma once

class BaseObject
{
public:

	BaseObject(){}
	~BaseObject() { Release(); }

	void Update();
	void Draw();
	void Init();

protected:

	void Release();

	KdTexture		m_tex;
	Math::Matrix	m_rotation;
	Math::Matrix	m_trans;
	Math::Matrix	m_mat;
	Math::Vector3	m_pos = {};
	float shotTimer = 0.0f;      // タイマー
	float shotInterval = 0.5f;   // 発射間隔（0.5秒に1発）
	bool			m_aliveFlg = true;
};