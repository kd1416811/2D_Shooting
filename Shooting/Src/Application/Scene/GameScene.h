#pragma once

//前方宣言
class SkillManager;
class BaseObject;
class DamageCalculation;
class Player;

class gameScene
{
public:

	~gameScene();

	void Update();
	void Draw();
	void Init();

	void AddObject(std::shared_ptr<BaseObject> _obj);

	//全オブジェクトのリストを格納
	const std::vector<std::shared_ptr<BaseObject>>& GetObjList() { return m_objList; }

private:

	void Release();

	void RemoveDeadObjects();

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

	std::shared_ptr<SkillManager>m_skillManager = nullptr;


	std::shared_ptr<Player> m_playerPointer = nullptr; // プレイヤーへの参照
	
	std::shared_ptr<DamageCalculation> m_damageCalc; // 計算機を1つ持つ
	
	void CheckCollision(); // 当たり判定関数

	// 例：現在のステージ。本来はセーブデータや前のシーンから受け取る
	int m_Stage = 1;

private:

	gameScene(){}

public:
	static gameScene& Instance()
	{
		static gameScene instance;
		return instance;
	}

};