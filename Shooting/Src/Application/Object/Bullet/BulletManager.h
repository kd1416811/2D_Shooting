#pragma once

class Bullet;

class BulletManager
{
public:

	BulletManager() { Init(); }
	~BulletManager() { Release(); }

	void Draw();
	void Update();
	void AddBullet(std::shared_ptr<Bullet> bullet);

private:

	void Init();
	void Release();

	std::list<std::shared_ptr<Bullet>> m_bulletList; // 弾のリスト
};