#pragma once
#include"BaseScene.h"

//前方宣言
class SkillManager;
class BaseObject;
class DamageCalculation;
class Player;
class BackgroundBase;

class GameScene:public BaseScene
{
public:

	GameScene();
	~GameScene();

	void Update()override;
	void Draw()override;
	void Init()override;

	void AddObject(std::shared_ptr<BaseObject> _obj);

	//全オブジェクトのリストを格納
	const std::vector<std::shared_ptr<BaseObject>>& GetObjList() { return m_objList; }

private:

	void Release();

	void RemoveDeadObjects();

	void CheckCollision(); // 当たり判定関数

	void Combo();// コンボ管理関数
	void ComboUpdate(); // コンボの更新関数

	//========================
	//	テンプレートInit関数
	//========================
	template <typename T>
	std::shared_ptr<T> CreateObject()
	{
		auto obj = std::make_shared<T>();	// インスタンス生成
		obj->Init();						//初期化
		obj->SetOwner(this);
		m_objList.push_back(obj);			// リストへ追加

		return obj;
	}

	//全オブジェクトを可変長配列で管理
	std::vector<std::shared_ptr<BaseObject>>m_objList;
	std::vector<std::shared_ptr<BaseObject>> m_addList; // 追加待ちリスト

	//背景
	std::unique_ptr<BackgroundBase> m_background = nullptr;


	std::shared_ptr<SkillManager>m_skillManager = nullptr;

	std::shared_ptr<Player> m_playerPointer = nullptr; // プレイヤーへの参照
	
	std::shared_ptr<DamageCalculation> m_damageCalc = nullptr; // 計算機を1つ持つ

	KdTexture m_AtkBuffTex;
	KdTexture m_CritBuffTex;
	KdTexture m_StageClearTex;
	KdTexture m_NumberTex; // 数字テクスチャ

	float     m_clearLogoY = 500.0f;     // 画像の現在のY座標（画面外の上からスタート）
	float     m_clearLogoAlpha = 0.0f;   // 画像の透明度（最初は見えない）
	const float TargetLogoY = 100.0f; // 最終的に止まる位置

	bool  m_isGameCleared = false; // クリアしたかどうかのフラグ
	bool  m_hasGivenReward = false;    // 追加：報酬（素材）を既に加算したか
	float m_clearTimer = 0.0f;		// クリア後の経過時間
	const float CLEAR_LIMIT = 4.0f; // 何秒待つか

	float m_comboScale = 3.0f;      // 現在の拡大率
	float m_comboShake = 0.0f;      // 現在の揺れオフセット
	int   m_lastComboCount = 0;     // 前フレームのコンボ数（変化検知用）

	bool m_debugMode;//デバックモードフラグ

	// 現在のステージ,SceneManagerから受け取る
	int m_Stage;

};