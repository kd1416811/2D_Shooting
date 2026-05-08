#include "gameScene.h"
#include"../Object/Player/Player.h"
#include"../Object/Enemy/Enemy.h"
#include"../Object/Bullet/Bullet.h"
#include"../Object/Skill/SkillManager.h"
#include"../Concept/DamageCalculation.h"

#include"../Concept/define.h"

gameScene::~gameScene()
{
	Release();
}

void gameScene::Update()
{
	//全オブジェクトの更新関数を呼ぶ
	for (int i = 0; i < (int)m_objList.size(); ++i)
	{
		if (m_objList[i] && m_objList[i]->GetAliveFlg())
		{
			m_objList[i]->Update();
		}
	}

	//一括当たり判定
	CheckCollision();

	//不要になったオブジェクト削除
	RemoveDeadObjects();

	m_skillManager->Update();
}

void gameScene::Draw()
{
	//全オブジェクトの描画関数を呼ぶ
	for (int i = 0; i < (int)m_objList.size(); ++i)
	{
		if (m_objList[i] && m_objList[i]->GetAliveFlg())
		{
			m_objList[i]->Draw();
		}
	}

	m_skillManager->Draw();
}

void gameScene::Init()
{
	m_damageCalc = std::make_unique<DamageCalculation>();

	// プレイヤーの生成
	m_playerPointer = CreateObject<Player>();
	m_playerPointer->SetType(0);

	// 敵の生成
	if (m_Stage % 3 == 1)	//1,4,7
	{
		auto enemy = CreateObject<Enemy>();
		enemy->SetType(Boss_1, m_Stage);
	}
	else if (m_Stage % 3 == 2)	//2,5,8
	{
		auto enemy = CreateObject<Enemy>();
		enemy->SetType(Boss_2, m_Stage);
	}
	else if (m_Stage % 3 == 0)	//3,6,9
	{
		auto enemy = CreateObject<Enemy>();
		enemy->SetType(Boss_3, m_Stage);
	}

	m_skillManager = std::make_shared<SkillManager>();
	m_skillManager->Init();

}

void gameScene::AddObject(std::shared_ptr<BaseObject> _obj)
{
	if (!_obj)return;

	m_objList.push_back(_obj);
}

void gameScene::Release()
{
	m_objList.clear();

	m_skillManager = nullptr;
}

void gameScene::RemoveDeadObjects()
{
	for (auto it = m_objList.begin(); it != m_objList.end();)
	{

		if (!(*it)->GetAliveFlg())
		{
			if (*it == m_playerPointer)m_playerPointer = nullptr;
			it = m_objList.erase(it);// リストから削除
		}
		else
		{
			++it;
		}

	}
}

void gameScene::CheckCollision()
{
	if (!m_playerPointer || !m_playerPointer->GetAliveFlg()) return;

	/////////////////////////////////
	// --- 弾と敵の当たり判定 ---
	/////////////////////////////////
	for (auto& bullet_obj : m_objList)
	{

		if (bullet_obj->GetObjType() != BaseObject::objectType::bullet ||
			!bullet_obj->GetAliveFlg())continue;

		auto bullet = std::static_pointer_cast<Bullet>(bullet_obj);


		for (auto& enemy_obj : m_objList)
		{
			if (enemy_obj->GetObjType() != BaseObject::objectType::enemy || !enemy_obj->GetAliveFlg()) continue;
			auto enemy = std::static_pointer_cast<Enemy>(enemy_obj);

			float dist = (bullet->GetPos() - enemy->GetPos()).Length();

			float enemyHalf = Config::EnemyMargin * 0.5f;
			float bulletHalf = Config::BulletMargin * 0.5f;

			if (dist < (enemyHalf + bulletHalf))
			{
				// 1. コンボ増加
				m_playerPointer->AddCombo();

				// 2. ダメージ計算
				int damage = m_damageCalc->Calculator(
					bullet->GetAtk(),
					enemy->GetDef(),
					enemy->GetResist(),
					m_playerPointer->GetComboCount(),
					m_playerPointer->GetPlayerLv()
				);
				bool criticalFlg = m_damageCalc->CriticalJudg();

				// 3. 適用
				enemy->OnHit(damage,criticalFlg);
				bullet->OnHit(0);
				break;
			}
		}
		break;
	}

	///////////////////////////////////////
	// --プレイヤーと敵の当たり判定--
	///////////////////////////////////////
	for (auto& enemy_obj : m_objList) {
		if (enemy_obj->GetObjType() != BaseObject::objectType::enemy ||
			!enemy_obj->GetAliveFlg()) continue;

		auto enemy = std::static_pointer_cast<Enemy>(enemy_obj);

		float dist = (m_playerPointer->GetPos() - enemy->GetPos()).Length();

		float playerHalf = Config::PlayerMargin * 0.5f;
		float enemyHalf = Config::EnemyMargin * 0.5f;

		if (dist < (playerHalf) +(enemyHalf)) {
			
			// 敵の攻撃力を引数に渡して OnHit を呼ぶ
			m_playerPointer->OnHit(enemy->GetAtk());
		}
	}
}
