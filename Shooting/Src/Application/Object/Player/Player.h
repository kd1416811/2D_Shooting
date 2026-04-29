#pragma once
#include "../BaseObject.h"
#include"../Bullet/Bullet.h"

class BulletManager;

class Player : public BaseObject
{
public:

	Player() { Init(); }
	~Player()override { Release(); }

	void Update()override;
	void Draw()	 override;
	
	void SetBulletManager(std::shared_ptr<BulletManager> bulletMgr)
											{ m_bulletManager = bulletMgr; }
private:

	static constexpr float NormalTimeRatio = 1.0f;		//1.0倍...通常倍率
	static constexpr float PlayerMargin = 64.0f;		//自機の大きさ

	//弾の挙動の初期種類
	BulletType m_currentShotType = BulletType::Normal;
	
	std::shared_ptr<BulletManager> m_bulletManager = nullptr;

	void Shoot();	//発射させる関数

	void Init() override;
	void Release()override;
};