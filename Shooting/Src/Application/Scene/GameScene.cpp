#include "gameScene.h"
#include"../Object/Player/Player.h"
#include"../Object/Enemy/Enemy.h"
#include"../Object/Bullet/Bullet.h"
#include"../Object/Skill/SkillManager.h"
#include"../Concept/DamageCalculation.h"
#include"../Object/Background/BackgroundBase.h"
#include"../Object/Background/Space/SpaceBackground.h"
#include"../Object/Background/Cloud/CloudBackground.h"
#include"../Object/Background/Abyss/AbyssBackground.h"
#include"SceneManager.h"
#include"../Object/Animation/explosion.h"
#include"../Concept/define.h"

GameScene::GameScene(){}

GameScene::~GameScene()
{
	Release();
}

void GameScene::Update()
{
	if (!m_background) return;

	m_background->Update();

	// 1. 生きている敵がいるかチェック
	bool isEnemyAlive = false;
	for (const auto& obj : m_objList) {
		if (obj->GetObjType() == BaseObject::objectType::enemy && obj->GetAliveFlg()) {
			isEnemyAlive = true;
			break;
		}
	}

	// 2. クリア判定：敵が全滅していて、まだクリアフラグが立っていないなら
	if (!isEnemyAlive && !m_isGameCleared) {
		m_isGameCleared = true;
		m_clearTimer = 0.0f; // タイマーリセット
	}

	// 3. クリア後の処理
	if (m_isGameCleared) {

		// --- ステージクリアロゴの演出 ---
		//フェードインするイメージ
		if (m_clearLogoAlpha < 1.0f)
		{
			m_clearLogoAlpha += 0.05f; 
		}

		if (m_clearLogoY > TargetLogoY)
		{
			// 現在地と目標地点の差を縮める(イージング)
			m_clearLogoY -= (m_clearLogoY - TargetLogoY) * 0.1f;
		}

		if (!m_hasGivenReward) {

			// 報酬を加算
			SceneManager::Instance().AddMaterials(
				SceneManager::Instance().GetRewardMaterials() * SceneManager::Instance().GetCurrentStage());

			//ここでステージをカウントアップ！
			SceneManager::Instance().NextStage();

			//m_UpGradeMaterials = 20;

			m_hasGivenReward = true; // 加算済みフラグを立てる
		}

		m_clearTimer += 1.0f / 60.0f; // 1/60.0f を加算

		// 指定した秒数が経過したら入力を受け付ける
		if (m_clearTimer >= CLEAR_LIMIT) {

			// ここでリザルトや次のステージへ
			SceneManager::Instance().SetNextScene(SceneManager::Instance().SceneType::Result);
		}
	}

	// スキルマネージャーの更新と入力を取得
	SkillType skill = m_skillManager->Update();

	// スキルが発動していたらプレイヤーに発射命令を出す
	if (skill != SkillType::None && m_playerPointer && isEnemyAlive)
	{
		m_playerPointer->ShootSkill(skill);
	}

	//全オブジェクトの更新関数を呼ぶ
	for (auto& obj : m_objList) {
		if (obj && obj->GetAliveFlg()) {
			obj->Update();
		}
	}

	//一括当たり判定
	CheckCollision();

	//不要になったオブジェクト削除
	RemoveDeadObjects();

	//新しいオブジェクトをメインリストに合流させる
	if (!m_addList.empty()) 
	{
		for (auto& newObj : m_addList)
		{
			// もし追加されるオブジェクトが「弾」だったら
			if (newObj->GetObjType() == BaseObject::objectType::bullet)
			{
				// Bullet型にキャストして弾速をセットする
				auto bullet = std::static_pointer_cast<Bullet>(newObj);
				bullet->SetBulletSpeed(SceneManager::Instance().GetCurrentBulletSpeed());
			}

			m_objList.push_back(newObj);
		}
		m_addList.clear(); // 予約リストを空にする
	}

	//m_skillManager->Update();
}

void GameScene::Draw()
{
	if (!m_background) return;

	m_background->Draw();

	//全オブジェクトの描画関数を呼ぶ
	for (int i = 0; i < (int)m_objList.size(); ++i)
	{
		if (m_objList[i] && m_objList[i]->GetAliveFlg())
		{
			m_objList[i]->Draw();
		}
	}

	m_skillManager->Draw();

	// --- バフアイコンの描画 (左上) ---
	if (m_playerPointer) 
	{
		// 左上の基準点 
		float startX = -600.0f;
		float startY = 320.0f;
		float offset = 80.0f; // アイコンの間隔
		int iconSize = 60;

		// 行列をリセット（UI用の位置に設定）
		Math::Matrix uiMat = Math::Matrix::Identity;

		// 1. 攻撃力アップバフ (Eキー)
		if (m_playerPointer->IsAtkBuffActive()) 
		{
			uiMat = Math::Matrix::CreateTranslation(startX, startY, 0);
			SHADER.m_spriteShader.SetMatrix(uiMat);
			SHADER.m_spriteShader.DrawTex(&m_AtkBuffTex, 0, 0, iconSize, iconSize, nullptr);

			startY -= offset; // 次のバフは下にずらす
		}

		// 2. クリティカルアップバフ (Rキー)
		if (m_playerPointer->IsCritBuffActive()) {
			uiMat = Math::Matrix::CreateTranslation(startX, startY, 0);
			SHADER.m_spriteShader.SetMatrix(uiMat);
			SHADER.m_spriteShader.DrawTex(&m_CritBuffTex, 0, 0, iconSize, iconSize, nullptr);
		}
	}

	// --- ステージクリアロゴの描画 ---
	if (m_isGameCleared)
	{
		// ロゴ用の行列作成（スライドするY座標を反映）
		Math::Matrix logoMat = Math::Matrix::CreateTranslation(0, m_clearLogoY, 0);
		SHADER.m_spriteShader.SetMatrix(logoMat);

		// アルファ値を反映させた色を作成
		Math::Color col = { 1.0f, 1.0f, 1.0f, m_clearLogoAlpha };

		SHADER.m_spriteShader.DrawTex(&m_StageClearTex, 0, 0, 416 * 2, 70* 2, nullptr, &col);
	}
}

void GameScene::Init()
{
	// SceneManagerから現在のステージ番号をもらってくる
	m_Stage = SceneManager::Instance().GetCurrentStage();

	// 敵の生成　＆　背景
	if (m_Stage % 3 == 1)	//1,4,7
	{
		m_background = std::make_unique<SpaceBackground>();

		auto enemy = CreateObject<Enemy>();
		enemy->SetOwner(this);
		enemy->SetType(Boss_1, m_Stage);
	}
	else if (m_Stage % 3 == 2)	//2,5,8
	{
		m_background = std::make_unique<SpaceBackground>();

		auto enemy = CreateObject<Enemy>();
		enemy->SetOwner(this);
		enemy->SetType(Boss_2, m_Stage);
	}
	else if (m_Stage % 3 == 0)	//3,6,9
	{
		m_background = std::make_unique<SpaceBackground>();

		auto enemy = CreateObject<Enemy>();
		enemy->SetOwner(this);
		enemy->SetType(Boss_3, m_Stage);
	}

	//背景
	m_background->Init();

	// プレイヤーの生成
	m_playerPointer = CreateObject<Player>();
	m_playerPointer->SetType(0, m_Stage);

	SceneManager::Instance().SetCurrentAtk(m_playerPointer->GetAtk());	
	SceneManager::Instance().SetCurrentHp(m_playerPointer->GetHp());
	SceneManager::Instance().SetCurrentDef(m_playerPointer->GetDef());


	m_damageCalc = std::make_shared<DamageCalculation>();

	// ここで一回だけ、強化分を計算機に叩き込む！
	float finalAtk = m_damageCalc->GetAtkPower() + SceneManager::Instance().GetAddedAtkPower();
	float finalCrit = m_damageCalc->GetCriticalRate() + SceneManager::Instance().GetAddedCritRate();
	
	m_damageCalc->SetAtkPower(finalAtk);
	m_damageCalc->SetCriticalRate(finalCrit);

	// クリティカル増加率などもあればここでSet
	// 基本値 + 強化分
	float finalCritDmg = m_damageCalc->GetCriticalIncreRate() + SceneManager::Instance().GetAddedCritIncreRate();

	// セットする
	m_damageCalc->SetCriticalIncreRate(finalCritDmg);


	m_skillManager = std::make_shared<SkillManager>();
	m_skillManager->Init();

	m_AtkBuffTex.Load("Textures/atkBuff.png");
	m_CritBuffTex.Load("Textures/criticalBuff.png");
	m_StageClearTex.Load("Textures/StageClear.png");

}

void GameScene::AddObject(std::shared_ptr<BaseObject> _obj)
{
	if (!_obj)return;

	m_addList.push_back(_obj);
}

void GameScene::Release()
{
	m_objList.clear();

	m_skillManager = nullptr;
}

void GameScene::RemoveDeadObjects()
{
	for (auto it = m_objList.begin(); it != m_objList.end();)
	{

		if (!(*it)->GetAliveFlg())
		{
			if (*it == m_playerPointer)m_playerPointer = nullptr;
			it = m_objList.erase(it);// リストから削除
		}
		else
		{
			++it;
		}

	}
}

void GameScene::CheckCollision()
{
	if (!m_playerPointer || !m_playerPointer->GetAliveFlg()) return;

	// 生きているオブジェクトだけのコピーリストを作る（ループ中のメモリ破壊を防ぐ）
	std::vector<std::shared_ptr<BaseObject>> currentObjects = m_objList;

	for (auto& bullet_obj : currentObjects) {
		// nullチェック、生存チェック、型チェック
		if (!bullet_obj || !bullet_obj->GetAliveFlg()) continue;
		if (bullet_obj->GetObjType() != BaseObject::objectType::bullet) continue;

		auto bullet = std::static_pointer_cast<Bullet>(bullet_obj);

		for (auto& enemy_obj : currentObjects) {
			if (!bullet->GetAliveFlg()) break; // 自分が死んだら即終了

			if (!enemy_obj || !enemy_obj->GetAliveFlg()) continue;
			if (enemy_obj->GetObjType() != BaseObject::objectType::enemy) continue;

			auto enemy = std::static_pointer_cast<Enemy>(enemy_obj);

			// --- ここに距離計算と当たり判定のロジックを書く ---
			// ※ bullet->OnHit(0) を呼んだら、必ず break すること

			// --- 判定用座標を「見た目」と完全に一致させる ---
			Math::Vector3 bPos = bullet->GetPos();
			if (bullet->HasStrategy()) {
				//bPos.y += 100.0f; bPos.x += 340.0f;
			}
			else {
				// 通常弾も描画位置（銃口の高さ）に合わせる
				bPos.y += bullet->GetAddPos().y;
			}

			float dist = (bPos - enemy->GetPos()).Length();


			float bulletHalf = (Config::BulletMargin_y * 0.5f);
			float enemyHalf = (Config::EnemyMargin_x * 0.5f);

			if (dist < (enemyHalf + bulletHalf))
			{
				if (enemy->IsInvincible()) continue;

				m_playerPointer->AddCombo();

				auto exp = CreateObject<Explosion>();
				if (exp) { exp->SetPos(bPos); exp->Update(); }

				// ダメージ計算
				int damage = m_damageCalc->Calculator(
					bullet->GetAtk(), enemy->GetDef(), enemy->GetResist(),
					m_playerPointer->GetComboCount(), m_playerPointer->GetPlayerLv(),
					m_playerPointer->GetHp(), enemy->GetHp(),
					m_playerPointer->IsCritBuffActive()
				);

				// 敵にダメージを与える
				enemy->OnHit(damage, m_damageCalc->CriticalJudg());

				bullet->OnHit(0);
				break;
			}
		}
	}

	///////////////////////////////////////
	// --プレイヤーと敵の当たり判定--
	///////////////////////////////////////
	for (auto& enemy_obj : currentObjects) {
		if (enemy_obj->GetObjType() != BaseObject::objectType::enemy ||
			!enemy_obj->GetAliveFlg()) continue;

		auto enemy = std::static_pointer_cast<Enemy>(enemy_obj);

		float dist = (m_playerPointer->GetPos() - enemy->GetPos()).Length();

		float playerHalf = Config::PlayerMargin * 0.5f;
		float enemyHalf = Config::EnemyMargin_y;

		if (dist < (playerHalf)+(enemyHalf)) {

			// 敵の攻撃力を引数に渡して OnHit を呼ぶ
			m_playerPointer->OnHit(enemy->GetAtk());
		}
	}

	///////////////////////////////////////
	// --プレイヤーと敵の弾当たり判定--
	///////////////////////////////////////
	for (auto& obj : currentObjects) {
		if (!obj || !obj->GetAliveFlg()) continue;

		// オブジェクトが「敵の弾」だった場合
		if (obj->GetObjType() == BaseObject::objectType::enemyBullet) {
			auto eBullet = std::static_pointer_cast<Bullet>(obj);

			// プレイヤーとの距離を測る
			float dist = (eBullet->GetPos() - m_playerPointer->GetPos()).Length();
			float playerHalf = Config::PlayerMargin * 0.5f;
			float enemyBulletHalf = Config::BulletMargin_y * 0.5f;

			if (dist < (playerHalf)+(enemyBulletHalf))
			{

				int damage = m_damageCalc->Calculator(
					eBullet->GetAtk(), m_playerPointer->GetDef(), 0,
					m_playerPointer->GetComboCount(), m_playerPointer->GetPlayerLv(),
					m_playerPointer->GetHp(), m_playerPointer->GetHp(),
					m_playerPointer->IsCritBuffActive()
				);

				// プレイヤーにダメージ
				m_playerPointer->OnHit(damage);

				// 弾は消える
				eBullet->OnHit(0);

				// 爆発エフェクトなど
				auto exp = CreateObject<Explosion>();
				if (exp) { exp->SetPos(eBullet->GetPos()); exp->Update(); }
			}
		}

	}
}