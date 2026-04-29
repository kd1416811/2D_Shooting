#include "GameScene.h"
#include "../Object/Player/Player.h"
#include"../Object/Bullet/BulletManager.h"
#include"../Object/Enemy/Enemy.h"
#include"../Object/Skill/SkillManager.h"

GameScene::GameScene() = default;
GameScene::~GameScene() = default;

void GameScene::Init()
{
	//================================
	//実体を生成(new)
	// ================================
	if (!m_bulletManager)	m_bulletManager = std::make_shared<BulletManager>();
	if (!m_skillManager)	m_skillManager	= std::make_unique<SkillManager>();

	if (!m_player)
	{
		auto player = std::make_shared<Player>();
		player->SetBulletManager(m_bulletManager);
		m_player = player;
	}

	if (!m_enemy) m_enemy	= std::make_shared<Enemy>();
	
	 

	//==========================
	//初期化
	//==========================
	if (m_player)	m_player->Init();
	if (m_enemy)	m_enemy	->Init();

	if (m_skillManager)m_skillManager->Init();
}

void GameScene::Draw()
{
	if (m_player) m_player->Draw();
	if (m_enemy)  m_enemy ->Draw();

	if (m_bulletManager)m_bulletManager->Draw();
	if (m_skillManager)m_skillManager->Draw();
}

void GameScene::Update()
{
	if (m_player) m_player->Update();
	if (m_enemy)  m_enemy ->Update();

	if (m_bulletManager)m_bulletManager->Update();
	if (m_skillManager)m_skillManager->Update();
}

void GameScene::Release()
{
	//===========================
	//実体を削除(Release)
	//===========================
	m_player = nullptr;
	m_enemy = nullptr;
	m_bulletManager = nullptr;
	m_skillManager = nullptr;
}