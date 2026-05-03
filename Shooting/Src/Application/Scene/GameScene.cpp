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

	//m_skillManager->Update();
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
	CreateObject<Enemy>();

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
