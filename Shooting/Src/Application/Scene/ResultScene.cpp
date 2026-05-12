#include "ResultScene.h"
#include"SceneManager.h"
#include"../Concept/DamageCalculation.h"
#include"../Object/Skill/SkillManager.h"
#include"../Object/Player/Player.h"
#include"../Concept/define.h"

ResultScene::ResultScene()
{
    
}

ResultScene::~ResultScene()
{
    Release();
}

void ResultScene::Init()
{
    m_tempCalc = std::make_shared<DamageCalculation>();
	m_skillManager = std::make_shared<SkillManager>();

	m_tex.Load("Textures/Haikei.png");
    m_numberTex.Load("Textures/number4.png");
    m_materialTex.Load("Textures/Item.png");
	m_costTex.Load("Textures/Cost.png"); 
	m_bookTex.Load("Textures/book.png");
    m_statusTex.Load("Textures/status.png");
    m_statusListTex.Load("Textures/moji.png");
	m_BaseStatusTex.Load("Textures/BaseStatus.png");
	m_subStatusTex.Load("Textures/SubStatus.png"); 
    m_atkTex.Load("Textures/atkBuff.png");
	m_hpTex.Load("Textures/HP.png");
	m_defTex.Load("Textures/DEF.png");
    m_criticalTex.Load("Textures/criticalBuff.png");
	m_cloverTex.Load("Textures/clover.png");
	m_atkPowertex.Load("Textures/atkPower.png");
	m_bulletSpeedtex.Load("Textures/bulletSpeed.png");
	m_burstRateTex.Load("Textures/burstRate.png");
	m_criticalDmgTex.Load("Textures/criticalDMG.png");
	m_lvTex.Load("Textures/lv.png");
    m_stageTex.Load("Textures/Stage.png");
}

void ResultScene::Update()
{
    // Aキーで攻撃強度アップ
    if (GetAsyncKeyState('H') & 0x0001) 
    {
		SceneManager::Instance().UpgradeAtk();// 攻撃強度の強化
    }

    // Sキーでクリティカル率アップ
    if (GetAsyncKeyState('F') & 0x0001)
    {
		SceneManager::Instance().UpgradeCritRate();// クリティカル率の強化
    }

    if (GetAsyncKeyState('J') & 0x0001) 
    {
		SceneManager::Instance().UpgradeBulletSpeed();// 弾速の強化
    }
    if (GetAsyncKeyState('K') & 0x0001)
    {
		SceneManager::Instance().UpgradeShotInterval();// 発射間隔の強化
    }
    if (GetAsyncKeyState('L') & 0x0001)
    {
        SceneManager::Instance().UpgradeCritIncreRate();// クリティカルダメージ増加の強化
    }
    if (GetAsyncKeyState('A') & 0x0001)
    {
		SceneManager::Instance().UpgradePlayerAtk();// 攻撃力の強化
    }
    if (GetAsyncKeyState('S') & 0x0001)
    {
		SceneManager::Instance().UpgradePlayerHp();// HPの強化
    }
    if (GetAsyncKeyState('D') & 0x0001)
    {
		SceneManager::Instance().UpgradePlayerDef();// 防御力の強化
    }
    if (GetAsyncKeyState('G') & 0x0001)
    {
		SceneManager::Instance().UpgradeLuckyCrit();//幸運クリティカルの強化
    }

    // エンターキーで次のステージへ
    if (GetAsyncKeyState(VK_RETURN) & 0x0001) {
        SceneManager::Instance().SetNextScene(SceneManager::Instance().SceneType::Game);
    }

    if (GetAsyncKeyState(VK_BACK) & 0x0001)
    {
        SceneManager::Instance().SetNextScene(SceneManager::Instance().SceneType::Title);
    }
}

void ResultScene::Draw()
{
    Math::Matrix baseMat = Math::Matrix::Identity;
    SHADER.m_spriteShader.SetMatrix(baseMat);
    D3D.SetBlendState(BlendMode::Alpha);
	
	SHADER.m_spriteShader.DrawTex(&m_tex, 0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, nullptr);
	
	SHADER.m_spriteShader.DrawTex(&m_bookTex, 0, 0, Config::SCREEN_WIDTH, 900, nullptr);
	
	SHADER.m_spriteShader.DrawTex(&m_statusTex, -270, 250, 125,14, nullptr);

    int numWH = 64;
	SHADER.m_spriteShader.DrawTex(&m_atkTex, 70, 180, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_hpTex, 250, 180, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_defTex, 70, 80, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_criticalTex, 250, 80, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_cloverTex, 70, -20, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_atkPowertex, 250, -20, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_bulletSpeedtex, 70, -120, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_burstRateTex, 250, -120, numWH, numWH, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_criticalDmgTex, 250, -220, numWH, numWH, nullptr);

    Math::Rectangle rect1 = { 0,0,40,20 };//基礎
	SHADER.m_spriteShader.DrawTex(&m_statusListTex, -420, 190, 40, 20, &rect1);
    Math::Rectangle rect2 = { 0,20,40,20 };//戦闘
    SHADER.m_spriteShader.DrawTex(&m_statusListTex, -420, 50, 40, 20, &rect2);
    Math::Rectangle rect3 = { 0,40,40,20 };//昇格
    SHADER.m_spriteShader.DrawTex(&m_statusListTex, -420, -110, 40, 20, &rect3);

	Math::Rectangle rect4 = { 0,0,55,20 };//攻撃力
    SHADER.m_spriteShader.DrawTex(&m_BaseStatusTex, -420, 150, 55, 20, &rect4);
	Math::Rectangle rect5 = { 0,20,55,20 };//防御力
    SHADER.m_spriteShader.DrawTex(&m_BaseStatusTex, -200, 150, 55, 20, &rect5);
	Math::Rectangle rect6 = { 0,40,55,20 };//HP
    SHADER.m_spriteShader.DrawTex(&m_BaseStatusTex, -420, 110, 55, 20, &rect6);

    Math::Rectangle rect7 = { 0,0,134,20 }; //クリティカル率
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -420, 10, 134, 20, &rect7);
    Math::Rectangle rect8 = { 0,40,134,20 };    //レイジ上限
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -200, 10, 134, 20, &rect8);
    Math::Rectangle rect9 = { 0,20,134,20 };    //幸運クリティカル
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -420, -50, 134, 20, &rect9);
    Math::Rectangle rect10 = { 0,80,134,20 };    //攻撃強度
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -390, -150, 134, 20, &rect10);
    Math::Rectangle rect11 = { 0,60,134,20 };    //弾速
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -190, -150, 134, 20, &rect11);
    Math::Rectangle rect12 = { 0,100,134,20 };    //クリダメージ
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -410, -200, 134, 20, &rect12);
    Math::Rectangle rect13 = { 0,120,134,20 };    //弾発射間隔
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -200, -200, 134, 20, &rect13);

	//--SkillManagerからデータを取得---
	int ultMaxCost = m_skillManager->GetUltMaxCost();

	//--bulletからデータを取得---
	float currentBulletSpeed = SceneManager::Instance().GetCurrentBulletSpeed();

    // --- SceneManagerからデータを取得 ---
    int myMaterials = SceneManager::Instance().GetTotalMaterials(); //素材数
    //int currentAtkLv = SceneManager::Instance().GetAtkLevel();      //
	//int mustCost = SceneManager::Instance().GetMustCost();             //強化に必要な素材数

    // SceneManagerから現在の「レベルによる倍率」を取得
    float hpMult = SceneManager::Instance().GetHpMultiplier();
    float defMult = SceneManager::Instance().GetDefMultiplier();
    float atkMult = SceneManager::Instance().GetAtkMultiplier();

    long long currentAtk = SceneManager::Instance().GetCurrentAtk();
	long long currentHp = SceneManager::Instance().GetCurrentHp();
	int currentDef = SceneManager::Instance().GetCurrentDef();

    long long displayAtk = (long long)(currentAtk * atkMult);
    long long displayHp = (long long)(currentHp * hpMult);
    int       displayDef = (int)(currentDef * defMult);

	//damageCalcultionクラスからデータを取得
    //m_tempCalc->SetAtkPower(m_tempCalc->GetAtkPower() + SceneManager::Instance().GetAddedAtkPower());
    //m_tempCalc->SetCriticalRate(m_tempCalc->GetCriticalRate() + SceneManager::Instance().GetAddedCritRate());

    // --- 数値の取得 ---
    float currentAtkPower = m_tempCalc->GetAtkPower() + SceneManager::Instance().GetAddedAtkPower();      //攻撃強度
    float currentCritRate = m_tempCalc->GetCriticalRate() + SceneManager::Instance().GetAddedCritRate();  //クリティカル率
	float currentMaxRndRation = (int)SceneManager::Instance().GetLuckyCritRate(); //幸運クリティカル
	int   currentMaxRndInt = m_tempCalc->GetMaxRndInt(); //幸運クリティカルの整数値

	//クリティカル増加率の計算
    float currentCritDmg = m_tempCalc->GetCriticalIncreRate() + SceneManager::Instance().GetAddedCritIncreRate();

    // 1.7f * 100 = 170 (%)
    int displayCritDmg = (int)(currentCritDmg * 100.0f + 0.5f);


	float currentInterval = SceneManager::Instance().GetPlayerShotInterval(); //弾発射間隔  

	float baseInterval = SceneManager::Instance().GetBaseShotInterval(); //弾発射間隔の初期値
    int displayPercent = (int)((baseInterval / currentInterval) * 100.0f + 0.5f); // 四捨五入してint化

    // 所持素材数の描画 
    DrawNumber(m_numberTex, m_materialTex, myMaterials,30, Math::Vector2(100, -220), 64, 64, 0.5f,0.3f,true,true,false);
    
    //なうステージ表示
	int nowStage = SceneManager::Instance().GetCurrentStage() - 1;
	DrawNumber(m_numberTex, m_stageTex, nowStage, 40, Math::Vector2(200, 250), 108, 32, 0.8f, 0.4f, false, true, false);

    ///////////////////////////////
    // --必要素材数cost,Lvの描画--
    ///////////////////////////////
    // --- クリティカル率の強化コスト描画例 ---
    int critCost = SceneManager::Instance().GetCritRateCost();
	int critLv = SceneManager::Instance().GetCritRateLevel();
    DrawNumber(m_numberTex, m_costTex, critCost,20, Math::Vector2(340, 60), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, critLv,10, Math::Vector2(330, 90), 67, 32, 0.4f, 0.2f, false, true, false);

    // --- 攻撃強度の強化コスト ---
    int atkPowerCost = SceneManager::Instance().GetAtkPowerCost();
	int atkLv = SceneManager::Instance().GetAtkLevel();
    DrawNumber(m_numberTex, m_costTex, atkPowerCost,20, Math::Vector2(340, -40), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, atkLv,10, Math::Vector2(330, -10), 67, 32, 0.4f, 0.2f, false, true, false);

    // --- 弾速の強化コスト ---
    int bulletSpeedCost = SceneManager::Instance().GetBulletSpeedCost();
	int bulletSpeedLv = SceneManager::Instance().GetBulletSpeedLevel(); 
    DrawNumber(m_numberTex, m_costTex, bulletSpeedCost,20, Math::Vector2(150, -140), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, bulletSpeedLv,10, Math::Vector2(140, -110), 67, 32, 0.4f, 0.2f, false, true, false);

    // --- クリティカルダメージの強化コスト ---
    int critDmgCost = SceneManager::Instance().GetCritIncreCost();
	int critDmgLv = SceneManager::Instance().GetCritIncreLevel();
    DrawNumber(m_numberTex, m_costTex, critDmgCost,20, Math::Vector2(340, -240), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, critDmgLv,10, Math::Vector2(330, -210), 67, 32, 0.4f, 0.2f, false, true, false);

    // --- 弾発射間隔の強化コスト ---
    int shotIntervalCost = SceneManager::Instance().GetShotIntervalCost();
	int shotIntervalLv = SceneManager::Instance().GetShotIntervalLevel();
    DrawNumber(m_numberTex, m_costTex, shotIntervalCost,20, Math::Vector2(340, -140), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, shotIntervalLv,10, Math::Vector2(330, -110), 67, 32, 0.4f, 0.2f, false, true, false);

	// --攻撃力の強化コスト--
    int atkCost = SceneManager::Instance().GetAtkCost();
	int atkBaseLv = SceneManager::Instance().GetAtkBaseLevel(); 
    DrawNumber(m_numberTex, m_costTex, atkCost, 20, Math::Vector2(150, 160), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, atkBaseLv,10, Math::Vector2(140, 190), 67, 32, 0.4f, 0.2f, false, true, false);

    // --体力の強化コスト--
    int HpCost = SceneManager::Instance().GetHpCost();
	int HpLv = SceneManager::Instance().GetHpLevel();
    DrawNumber(m_numberTex, m_costTex, HpCost, 20, Math::Vector2(340, 160), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, HpLv,10, Math::Vector2(330, 190), 67, 32, 0.4f, 0.2f, false, true, false);

    // --防御の強化コスト--
    int defCost = SceneManager::Instance().GetDefCost();
	int defLv = SceneManager::Instance().GetDefLevel(); 
    DrawNumber(m_numberTex, m_costTex, defCost, 20, Math::Vector2(150, 60), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, defLv,10, Math::Vector2(140, 90), 67, 32, 0.4f, 0.2f, false, true, false);

	//--幸運クリティカルの強化コスト--
    int luckyCost = SceneManager::Instance().GetLuckyCritCost();
	int luckyLv = SceneManager::Instance().GetLuckyCritLevel();
    DrawNumber(m_numberTex, m_costTex, luckyCost, 20, Math::Vector2(150, -40), 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, luckyLv,10, Math::Vector2(140, -10), 67, 32, 0.4f, 0.2f, false, true, false);

    ///攻撃力表示
    DrawNumber(m_numberTex, m_BaseStatusTex, displayAtk,0, Math::Vector2(-380, 150), 55, 20, 0.5f,0.2f,false,false,false);
    // HP表示
    DrawNumber(m_numberTex, m_BaseStatusTex, displayHp,0, Math::Vector2(-380, 100), 55, 20, 0.5f,0.2f,false,false,false);
    // 防御力表示
    DrawNumber(m_numberTex, m_BaseStatusTex, displayDef,0, Math::Vector2(-170, 150), 55, 20, 0.5f,0.2f,false,false,false);
    //クリティカル率
	DrawNumber(m_numberTex, m_subStatusTex, currentCritRate,0, Math::Vector2(-350, 10), 55, 20, 0.5f, 0.3f, false, false,true);
    //レイジ上限
	DrawNumber(m_numberTex, m_subStatusTex, ultMaxCost,0, Math::Vector2(-150, 10), 55, 20, 0.5f, 0.3f, false, false, false);
    //幸運クリティカル
    DrawNumber(m_numberTex, m_subStatusTex, currentMaxRndRation,0, Math::Vector2(-350, -50), 55, 20, 0.5f, 0.3f, false, false, true);
    //攻撃強度
	DrawNumber(m_numberTex, m_subStatusTex, currentAtkPower,0, Math::Vector2(-380, -150), 55, 20, 0.5f, 0.3f, false, false, true);
	//弾速
	DrawNumber(m_numberTex, m_subStatusTex, currentBulletSpeed * 10.0f,0, Math::Vector2(-200, -150), 55, 20, 0.5f, 0.3f, false, false, true);
	//クリダメ
	DrawNumber(m_numberTex, m_subStatusTex, displayCritDmg,0, Math::Vector2(-380, -200), 55, 20, 0.5f, 0.3f, false, false, true);
	//弾発射間隔
	DrawNumber(m_numberTex, m_subStatusTex, displayPercent,0, Math::Vector2(-200, -200), 55, 20, 0.5f, 0.3f, false, false, true);
}

void ResultScene::Release()
{}

void ResultScene::DrawNumber(const KdTexture & tex, const KdTexture& tex2, int value, int add, Math::Vector2 startPos, float sabWidth, float sabHeight, float scale, float mojiScale, bool b_Multi, bool b_SecTex ,bool b_persent)
{
    std::string str = std::to_string(value); // 数値を文字列に変換して1文字ずつ処理する
    int numWidth = 64;      // 1文字あたりの横幅 (12分割)
    int numHeight = 64;         // 文字の高さ
    Math::Color col = { 0,0,0,1.0f };

    if (b_SecTex)
    {
        Math::Rectangle src = { 0,0,(long)sabWidth,(long)sabHeight };
        SHADER.m_spriteShader.DrawTex(&tex2, startPos.x - add, startPos.y, sabWidth * scale, sabHeight * scale, &src);
    }

    if (b_Multi)
    {
        //×表示
        Math::Rectangle srcRect = { 0, 0, numWidth, numHeight };
        SHADER.m_spriteShader.DrawTex(&tex, startPos.x , startPos.y , (numWidth / 2) * scale, (numHeight / 2) * scale, &srcRect,&col);
    }

    // 次の文字の描画位置をずらす
    startPos.x  += (numWidth * mojiScale); // 文字間隔を少し詰める調整

    for (char c : str)
    {
        //char型からint型に型変換
        int digit = c - '0';

        int srcX = (digit + 1) * numWidth;

        Math::Rectangle srcRect = { (long)srcX, 0, numWidth, numHeight };
        

        SHADER.m_spriteShader.DrawTex(&tex, startPos.x, startPos.y, (numWidth / 2) * scale, (numHeight / 2) * scale, &srcRect, &col);

        startPos.x += (numWidth * mojiScale);
    }

    if (b_persent)
    {
		//％表示
		Math::Rectangle srcRect = { 0, numHeight, numWidth, numHeight };
		SHADER.m_spriteShader.DrawTex(&tex, startPos.x , startPos.y, (numWidth / 2) * scale, (numHeight / 2) * scale, &srcRect, &col);   
    }
}
