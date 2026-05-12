#pragma once

//前方宣言
class BaseScene;

//全てのシーンを管理するクラス
class SceneManager
{
public:

	enum SceneType
	{
		Title,
		Game,
		Result
	};

	void PreUpdate();
	void Update();
	void Draw();


	void SetNextScene(SceneType _nextScene)
	{
		m_nextSceneType = _nextScene;
	}

	//現在のシーンを取得する関数
	SceneType GetCurrentScene() const
	{
		return m_currentSceneType;
	}

	// 素材のゲッターとセッター
	int GetTotalMaterials() const { return m_totalMaterials; }
	int  GetCurrentStage() const { return m_currentStage; }
	int GetAtkLevel()       const { return m_atkLevel; }
	int GetCritRateLevel()  const { return m_critRateLevel; }
	//int GetMustCost()       const { return m_MustCost; }
	long long GetCurrentAtk() const { return m_currentAtk; }
	long long GetCurrentHp() const { return m_crrentHp; }
	int GetCurrentDef() const { return m_currentDef; }
	float GetAddedAtkPower() const { return m_addedAtkPower; }
	float GetAddedCritRate() const { return m_addedCritRate; }
	void AddMaterials(int amount) { m_totalMaterials += amount; }
	int GetBulletSpeedLevel() const { return m_bulletSpeedLevel; }
	float GetCurrentBulletSpeed() const {
		return m_baseBulletSpeed + (m_bulletSpeedLevel * 1.5f); // 1レべごとに1.5アップ
	}
	int GetShotIntervalLevel() const { return m_shotIntervalLevel; }
	float GetBaseShotInterval() const { return m_baseShotInterval; }// 発射間隔の初期値を受け取る
	float GetPlayerShotInterval() const {
		// 初期値 0.3秒 から 1レベルごとに 0.02秒 ずつ短くする（例）
		float interval = 0.3f - (m_shotIntervalLevel * 0.02f);

		// 限界値（これ以上速くなると弾が出すぎる設定）
		if (interval < 0.04f) interval = 0.04f;

		return interval;
	}

	// 幸運クリティカル発生率 (1レベルごとに 5% アップ)
	float GetLuckyCritRate() const {
		float rate = m_luckyCritLevel * 5.0f;
		return rate;
	}

	int GetCritIncreLevel() const { return m_critIncreLevel; }
	float GetAddedCritIncreRate() const { return m_addedCritIncreRate; }//くてぃかるダメージ増加率の追加分を受け取る


	void SetCurrentAtk(long long atk) { m_currentAtk = atk; }
	void SetCurrentHp(long long hp) { m_crrentHp = hp; }
	void SetCurrentDef(int def) { m_currentDef = def; }

	// 項目ごとに基本価格や上昇幅を変えるとゲームバランスが良くなります
	int GetAtkPowerCost()     const { return 5 + (m_atkLevel * 20); }
	int GetCritRateCost()     const { return 5 + (m_critRateLevel * 20); }
	int GetBulletSpeedCost()  const { return 10 + (m_bulletSpeedLevel * 10); }
	int GetShotIntervalCost() const { return 20 + (m_shotIntervalLevel * 30); }
	int GetCritIncreCost()    const { return 5 + (m_critIncreLevel * 10); }
	int GetLuckyCritCost() const { return 20 + (m_luckyCritLevel * 20); }
	// コスト関数（数値はバランスを見て調整してください）
	int GetHpCost()  const { return 5 + (m_hpLevel * 10); }
	int GetDefCost() const { return 5 + (m_defLevel * 10); }
	int GetAtkCost() const { return 5 + (m_atkBaseLevel * 10); }

	// 倍率取得 (例: 1レベルごとに 20% アップ)
	float GetHpMultiplier()  const { return 1.0f + (m_hpLevel * 0.3f); }
	float GetDefMultiplier() const { return 1.0f + (m_defLevel * 0.3f); }
	float GetAtkMultiplier() const { return 1.0f + (m_atkBaseLevel * 0.3f); }

	int GetAtkBaseLevel() const { return m_atkBaseLevel; }
	int GetHpLevel() const { return m_hpLevel; }
	int GetDefLevel() const { return m_defLevel; }
	int GetLuckyCritLevel() const { return m_luckyCritLevel; }
	int GetRewardMaterials() const { return m_RewardMaterials; }

	// 強化関数
	void UpgradePlayerHp() 
	{
		int cost = GetHpCost();
		if (m_totalMaterials >= cost) 
		{
			m_totalMaterials -= cost;
			m_hpLevel++;
		}
	}
	
	
	void UpgradePlayerDef()
	{
		int cost = GetDefCost();
		if (m_totalMaterials >= cost) 
		{
			m_totalMaterials -= cost;
			m_defLevel++;
		}
	}
	
	
	void UpgradePlayerAtk() 
	{
		int cost = GetAtkCost();
		if (m_totalMaterials >= cost) 
		{
			m_totalMaterials -= cost;
			m_atkBaseLevel++;
		}
	}
	
	void UpgradeLuckyCrit()
	{
		int cost = GetLuckyCritCost();
		if (m_totalMaterials >= cost) 
		{
			m_totalMaterials -= cost;
			m_luckyCritLevel++;
		}
	}

	// 強化を実行する
	void UpgradeAtk()
	{
		int cost = GetAtkPowerCost();
		if (m_totalMaterials >= cost)
		{
			m_totalMaterials -= cost;
			m_atkLevel++;
			m_addedAtkPower += 50.0f; // 強化で増えた攻撃強度を更新
		}
	}

	void UpgradeCritRate()
	{
		int cost = GetCritRateCost();
		if (m_totalMaterials >= cost)
		{
			m_totalMaterials -= cost;
			m_critRateLevel++;
			m_addedCritRate += 2.0f; // 強化で増えたクリティカル率を更新
		}
	}

	// 強化関数（ResultSceneのUpdateで呼ぶ用）
	void UpgradeBulletSpeed()
	{
		int cost = GetBulletSpeedCost();
		if (m_totalMaterials >= cost)
		{
			m_totalMaterials -= cost;
			m_bulletSpeedLevel++;
		}
	}

	// 強化関数
	void UpgradeShotInterval() {
		// 必要素材のチェックなどは攻撃力強化と同様に

		int cost = GetShotIntervalCost();
		if (m_totalMaterials >= cost)
		{
			m_totalMaterials -= cost;
			m_shotIntervalLevel++;
		}
	}

	void UpgradeCritIncreRate() 
	{
		int cost = GetCritIncreCost();

		if (m_totalMaterials >= cost)
		{
			m_totalMaterials -= cost;
			// 1回の強化で 0.25 (25%) ずつアップさせる
			m_addedCritIncreRate += 0.25f;
			m_critIncreLevel++;
		}
	}

	void ResetStage() { m_currentStage = 1; } // ゲームオーバー時など用
	void NextStage() { m_currentStage++; }//次のステージへ
	

private:

	//初期化や解放は自分でする
	void Init();
	void Release();

	//シーン切替関数
	void ChangeScene(SceneType _sceneType);

	//現在のシーンを管理するポインタ変数
	std::shared_ptr<BaseScene>m_pCurrentScene;

	//現在のシーンを管理するフラグ変数
	SceneType m_currentSceneType = SceneType::Result;

	//次のシーンを管理するフラグ変数
	SceneType m_nextSceneType = m_currentSceneType;

	
	//レベル系
	int m_atkLevel = 0; // 攻撃力の強化段階
	int m_critIncreLevel = 0; // クリティカルダメージ増加の強化段階
	int m_critRateLevel = 0; // クリティカル率の強化段階
	int m_shotIntervalLevel = 0; // 発射間隔の強化段階
	int  m_bulletSpeedLevel = 0;   // 弾速の強化段階
	int m_hpLevel = 0;
	int m_defLevel = 0;
	int m_atkBaseLevel = 0; // CSVのatkを底上げする用
	int m_luckyCritLevel = 0; // 幸運クリティカルのレベル


	int m_RewardMaterials = 100; // クリア報酬でもらえる素材数。
	int m_totalMaterials = 0; // ここに累計の素材数が貯まる
	int m_currentStage = 1; // 1からスタート

	long long m_currentAtk = 0; // ここに保存される
	long long m_crrentHp = 0;
	int m_currentDef = 0;

	float m_addedAtkPower = 0.0f;    // 強化で増えた攻撃強度
	float m_addedCritRate = 0.0f;    // 強化で増えたクリティカル率

	
	float m_addedCritIncreRate = 0.0f; // 追加分のクリティカルダメージ（0.1 = +10%）

	
	//int m_MustCost = 0; // 強化に必要な素材数を計算して入れる変数（UI表示用）

	
	float m_baseShotInterval = 0.3f; // 発射間隔の初期値

	
	float m_baseBulletSpeed = 10.0f; // 弾速の初期値

private:

	//外で変数宣言できない
	SceneManager() { Init(); }
	~SceneManager() { Release(); }

public:

	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

};