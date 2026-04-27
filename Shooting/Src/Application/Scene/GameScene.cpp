#include "GameScene.h"
#include "../Object/Player/Player.h"
#include"../Object/Skill/SkillManager.h"

GameScene::GameScene() = default;
GameScene::~GameScene() = default;

void GameScene::Init()
{
	if (!m_player)
	{
		//newの変わり(スマートポインタ)
		m_player = std::make_unique<Player>();
	}
	if(!m_skillManager)m_skillManager = std::make_unique<SkillManager>();

	if (m_player) m_player->Init();
	if (m_skillManager)m_skillManager->Init();
}

void GameScene::Draw()
{
	if (m_player) m_player->Draw();
	if (m_skillManager)m_skillManager->Draw();
}

void GameScene::Update()
{
	if (m_player) m_player->Update();
	if (m_skillManager)m_skillManager->Update();
}

void GameScene::Release()
{
	m_player = nullptr;
	m_skillManager = nullptr;
}