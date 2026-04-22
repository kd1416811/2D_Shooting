#pragma once
#include <memory>

class Player;

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
	std::unique_ptr<Player> m_player;

private:

	GameScene();

public:
	static GameScene& GetInstance()
	{
		static GameScene instance;
		return instance;
	}
};

#define GAMESCENE GameScene::GetInstance()
