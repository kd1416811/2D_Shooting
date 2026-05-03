#pragma once

//前方宣言
class SkillManager;
class BaseObject;

class gameScene
{
public:

	~gameScene() { Release(); }

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
	void CreateObject()
	{
		auto obj = std::make_shared<T>();	// インスタンス生成
		obj->Init();						//初期化
		obj->SetOwner(this);
		m_objList.push_back(obj);			// リストへ追加

		return;
	}

	//全オブジェクトを可変長配列で管理
	std::vector<std::shared_ptr<BaseObject>>m_objList;

	std::shared_ptr<SkillManager>m_skillManager = nullptr;

private:

	gameScene() {}

public:
	static gameScene& Instance()
	{
		static gameScene instance;
		return instance;
	}

};