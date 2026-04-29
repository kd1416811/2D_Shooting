#pragma once

//class Player;
class BaseObject;
class BulletManager;
class SkillManager;

class GameScene
{
public:

	
	~GameScene();

	// コピーと代入を禁止（シングルトンの保護）
	GameScene(const GameScene&) = delete;
	GameScene& operator=(const GameScene&) = delete;

	void Init();
	void Draw();
	void Update();

private:

	void Release();

	//スマートポインタ
	std::shared_ptr<BaseObject>		m_player		= nullptr;
	std::shared_ptr<BaseObject>		m_enemy			= nullptr;


	std::shared_ptr<BulletManager>	m_bulletManager	= nullptr;
	std::unique_ptr<SkillManager>	m_skillManager	= nullptr;

private:

	GameScene();

public:
	static GameScene& Instance()
	{
		static GameScene instance;
		return instance;
	}
};
