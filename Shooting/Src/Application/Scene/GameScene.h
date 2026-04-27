#pragma once

class Player;
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

	//Player* m_player = nullptr;
	//スマートポインタ
	std::unique_ptr<Player> m_player = nullptr;
	std::unique_ptr<SkillManager> m_skillManager = nullptr;

private:

	GameScene();

public:
	static GameScene& Instance()
	{
		static GameScene instance;
		return instance;
	}
};
