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
	bool			m_aliveFlg = true;
};