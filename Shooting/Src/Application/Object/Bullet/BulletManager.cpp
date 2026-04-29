#include "BulletManager.h"
#include"Bullet.h"

void BulletManager::Draw()
{
	for (auto& bullet : m_bulletList)
	{
		bullet->Draw();
	}
}

void BulletManager::Update()
{
	//全ての弾を更新し、死んだ弾（画面外など）を削除
	for (auto it = m_bulletList.begin(); it != m_bulletList.end();)
	{
		(*it)->Update();

		if (!(*it)->GetAliveFlg())
		{
			it = m_bulletList.erase(it);// リストから削除
		}
		else
		{
			++it;
		}
	}
}

void BulletManager::AddBullet(std::shared_ptr<Bullet> bullet)
{
	m_bulletList.push_back(bullet);
}

void BulletManager::Init()
{
}

void BulletManager::Release()
{}
