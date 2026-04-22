#include "GameScene.h"
#include "../Object/Player/Player.h"

GameScene::GameScene() = default;
GameScene::~GameScene() = default;

void GameScene::Init()
{
	if (!m_player)
	{
		//newの変わり(スマートポインタ)
		m_player = std::make_unique<Player>();
	}

	if (m_player) m_player->Init();
}

void GameScene::Draw()
{
	if (m_player) m_player->Draw();
}

void GameScene::Update()
{
	if (m_player) m_player->Update();
}

void GameScene::Release()
{
	m_player = nullptr;
}