#include "gameScene.h"
#include"../Object/Player/Player.h"
#include"../Object/Enemy/Enemy.h"
#include"../Object/Skill/SkillManager.h"

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
	// プレイヤーの生成
	CreateObject<Player>();

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
			it = m_objList.erase(it);// リストから削除
		}
		else
		{
			++it;
		}

	}
}
