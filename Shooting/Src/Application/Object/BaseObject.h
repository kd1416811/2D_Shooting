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
		bullet,
		damageEffect
	};

	BaseObject() {}
	virtual ~BaseObject() { Release(); }

	virtual void Update();
	virtual void Draw();
	virtual void Init();
	
	virtual void OnHit(long long damage) { (void)damage; }

	virtual void SetOwner(gameScene* _owner) {}

	float One_Half(float v) { return v * 0.5f; }

	const	objectType&		GetObjType()	{ return m_objType; }
	const	Math::Vector3&	GetPos()		{ return m_pos; }
	const	bool&			GetAliveFlg()	{ return m_aliveFlg; }

protected:

	virtual void Release();

	KdTexture		m_tex;
	Math::Matrix	m_scale;
	Math::Matrix	m_rotation;
	Math::Matrix	m_trans;
	Math::Matrix	m_mat;
	Math::Vector3	m_pos = {};
	Math::Vector3   m_dir = {}; //移動方向
	int				m_deg;		//角度
	int				m_lifeTime;
	float			m_shotInterval;		
	float			m_ObjScale;
	float			m_speed;
	float			m_shotTimer;
	float			m_deltaTime;
	float			m_alpha;
	bool			m_aliveFlg = true;

	objectType		m_objType = objectType::None;

	gameScene*		m_owner = nullptr;
};