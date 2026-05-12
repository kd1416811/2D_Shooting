#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include"ResultScene.h"

void SceneManager::PreUpdate()
{
	//シーン切替
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}
}

void SceneManager::Update()
{
	//ポリモーフィズム
	//同じ関数名であっても、呼び出すオブジェクトによって処理内容が変わること
	m_pCurrentScene->Update();
}

void SceneManager::Draw()
{
	m_pCurrentScene->Draw();
}

void SceneManager::Init()
{
	ChangeScene(m_currentSceneType);
}

void SceneManager::Release()
{}

void SceneManager::ChangeScene(SceneType _sceneType)
{
	//①次のシーンを作成、②フラグを更新する
	switch (_sceneType)
	{
	case SceneType::Title:

		m_pCurrentScene = std::make_shared<TitleScene>();

		break;

	case SceneType::Game:

		m_pCurrentScene = std::make_shared<GameScene>();

		break;

	case SceneType::Result:

		m_pCurrentScene = std::make_shared<ResultScene>();

		break;
	}

	if (m_pCurrentScene)
	{
		m_pCurrentScene->Init();
	}

	m_currentSceneType = _sceneType;
}