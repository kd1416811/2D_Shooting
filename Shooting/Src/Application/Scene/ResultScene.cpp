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

    // UpgradeItem { 座標, Lv, 初期コスト, 名前 }
    m_upgrades.clear();
    m_upgrades.push_back({ {70, 180},  0, 5, "AtkBase" });   // Idx 0: 攻撃力
    m_upgrades.push_back({ {70, 80},   0, 5, "Def" });       // Idx 1: 防御力
    m_upgrades.push_back({ {70, -20},  0, 20, "Lucky" });    // Idx 2: 幸運
    m_upgrades.push_back({ {70, -120}, 0, 10, "Speed" });    // Idx 3: 弾速
    m_upgrades.push_back({ {250, 180}, 0, 5, "HP" });        // Idx 4: HP
    m_upgrades.push_back({ {250, 80},  0, 5, "CritRate" });  // Idx 5: クリティカル率
    m_upgrades.push_back({ {250, -20}, 0, 5, "AtkPower" });  // Idx 6: 攻撃強度
    m_upgrades.push_back({ {250, -120},0, 20, "Interval" }); // Idx 7: 発射間隔
    m_upgrades.push_back({ {250, -220},0, 5, "CritDmg" });   // Idx 8: クリダメ


	m_tex.Load("Textures/Haikei.png");
    m_numberTex.Load("Textures/number4.png");
    m_materialTex.Load("Textures/Item.png");
	m_costTex.Load("Textures/Cost.png"); 
	m_bookTex.Load("Textures/book.png");
    m_statusTex.Load("Textures/status.png");
    m_statusListTex.Load("Textures/moji.png");
	m_BaseStatusTex.Load("Textures/BaseStatus.png");
	m_BaseStatusSecTex.Load("Textures/BaseStatus2.png");
	m_subStatusTex.Load("Textures/SubStatus.png");
	m_subStatusSecTex.Load("Textures/SubStatus2.png"); 
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
    m_statusPowerTex.Load("Textures/statusPower.png");
    m_enterTex.Load("Textures/enter2.png");
	m_playGameTex.Load("Textures/playGame.png");
    m_BackSpaceTex.Load("Textures/backSpace2.png");
    m_backTitleTex.Load("Textures/backTitle.png");
    m_cursorTex.Load("Textures/BlackFrame.png");
}

void ResultScene::Update()
{
	// 入力処理
    HandleInput();

    for (int i = 0; i < 9; i++) {
        if (m_statScales[i] > 1.0f) {
            // 1.0に向かって減衰
            m_statScales[i] += (1.0f - m_statScales[i]) * 0.15f;

            // 1.0に十分近づいたらピタッと止める
            if (m_statScales[i] < 1.001f) m_statScales[i] = 1.0f;
        }
    }

    // シェイクの減衰（これは全体で共通の変数ならループの外でOK）
    if (m_statShake > 0.1f) 
    {
        m_statShake *= 0.85f;
    }
    else {
        m_statShake = 0.0f;
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
	
    //ステータス一覧ラベル
	SHADER.m_spriteShader.DrawTex(&m_statusTex, -270, 250, 175,20, nullptr);
    //ステータス強化のラベル
    SHADER.m_spriteShader.DrawTex(&m_statusPowerTex, 210, 250, 175, 20, nullptr);

    //エンターキー表示(ゲーム画面に)
	SHADER.m_spriteShader.DrawTex(&m_enterTex, 570, -250, 156, 192, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_playGameTex, 580, -220, 76, 16, nullptr);

	//バックスペースキー表示(タイトルに戻るキー表示)
	SHADER.m_spriteShader.DrawTex(&m_BackSpaceTex, 570, 280, 128, 128, nullptr);
	SHADER.m_spriteShader.DrawTex(&m_backTitleTex, 570, 280, 94, 16, nullptr);

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

    // ハイライト用の色（選択中なら黄色、そうでなければ黒/白）
    Math::Color highlightCol = { 1.0f, 1.0f, 0.0f, 1.0f }; // 黄色
    Math::Color normalCol = { 0.0f, 0.0f, 0.0f, 1.0f };    // 通常（黒）
    KdTexture* subStatus = &m_subStatusTex;
    KdTexture* subStatusSec = &m_subStatusSecTex;
	KdTexture* baseStatus = &m_BaseStatusTex;
	KdTexture* baseStatusSec = &m_BaseStatusSecTex;

	Math::Color atkBaseCol = (m_selectIdx == 0) ? highlightCol : normalCol;//攻撃力   
	Math::Color defCol = (m_selectIdx == 1) ? highlightCol : normalCol;     //防御力
	Math::Color luckyCol = (m_selectIdx == 2) ? highlightCol : normalCol;   //幸運
	Math::Color speedCol = (m_selectIdx == 3) ? highlightCol : normalCol;   //弾速
	Math::Color hpCol = (m_selectIdx == 4) ? highlightCol : normalCol;      //HP
	Math::Color critRateCol = (m_selectIdx == 5) ? highlightCol : normalCol; //クリティカル率
	Math::Color atkPowerCol = (m_selectIdx == 6) ? highlightCol : normalCol; //攻撃強度
	Math::Color intervalCol = (m_selectIdx == 7) ? highlightCol : normalCol;  //弾発射間隔
	Math::Color critDmgCol = (m_selectIdx == 8) ? highlightCol : normalCol;   //クリダメ

	Math::Rectangle rect4 = { 0,0,55,20 };//攻撃力
    KdTexture* atkDrawTex = (m_selectIdx == 0) ? baseStatusSec : baseStatus;
    SHADER.m_spriteShader.DrawTex(atkDrawTex, -420, 150, 55, 20, &rect4, &atkBaseCol);
	
    Math::Rectangle rect5 = { 0,20,55,20 };//防御力
    KdTexture* defDrawTex = (m_selectIdx == 1) ? baseStatusSec : baseStatus;
    SHADER.m_spriteShader.DrawTex(defDrawTex, -200, 150, 55, 20, &rect5, &defCol);
	
    Math::Rectangle rect6 = { 0,40,55,20 };//HP
	KdTexture* hpDrawTex = (m_selectIdx == 4) ? baseStatusSec : baseStatus;
    SHADER.m_spriteShader.DrawTex(hpDrawTex, -420, 110, 55, 20, &rect6, &hpCol);

    Math::Rectangle rect7 = { 0,0,134,20 }; //クリティカル率
	KdTexture* critRateDrawTex = (m_selectIdx == 5) ? subStatusSec : subStatus;
    SHADER.m_spriteShader.DrawTex(critRateDrawTex, -420, 10, 134, 20, &rect7, &critRateCol);
    
    Math::Rectangle rect8 = { 0,40,134,20 };    //レイジ上限
    SHADER.m_spriteShader.DrawTex(&m_subStatusTex, -200, 10, 134, 20, &rect8, &normalCol );
    
    Math::Rectangle rect9 = { 0,20,134,20 };    //幸運クリティカル
	KdTexture* luckyDrawTex = (m_selectIdx == 2) ? subStatusSec : subStatus;
    SHADER.m_spriteShader.DrawTex(luckyDrawTex, -420, -50, 134, 20, &rect9, &luckyCol);
    
    Math::Rectangle rect10 = { 0,80,134,20 };    //攻撃強度
	KdTexture* atkPowerDrawTex = (m_selectIdx == 6) ? subStatusSec : subStatus;
    SHADER.m_spriteShader.DrawTex(atkPowerDrawTex, -390, -150, 134, 20, &rect10, &atkPowerCol);
    
    Math::Rectangle rect11 = { 0,60,134,20 };    //弾速
	KdTexture* speedDrawTex = (m_selectIdx == 3) ? subStatusSec : subStatus;
    SHADER.m_spriteShader.DrawTex(speedDrawTex, -190, -150, 134, 20, &rect11, &speedCol);
    
    Math::Rectangle rect12 = { 0,100,134,20 };    //クリダメージ
	KdTexture* critDmgDrawTex = (m_selectIdx == 8) ? subStatusSec : subStatus;
    SHADER.m_spriteShader.DrawTex(critDmgDrawTex, -410, -200, 134, 20, &rect12, &critDmgCol);
    
    Math::Rectangle rect13 = { 0,120,134,20 };    //弾発射間隔
	KdTexture* intervalDrawTex = (m_selectIdx == 7) ? subStatusSec : subStatus; 
    SHADER.m_spriteShader.DrawTex(intervalDrawTex, -200, -200, 134, 20, &rect13, &intervalCol);

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
	Math::Color matCol = (myMaterials >= GetNeedCost(m_selectIdx)) ? Math::Color(0.0f, 1.0f, 0.0f, 1.0f) : Math::Color(1.0f, 0.0f, 0.0f, 1.0f); // 緑色（十分な素材数）か赤色（不足している場合）
    DrawNumber(m_numberTex, m_materialTex, myMaterials,30, Math::Vector2(100, -220),matCol, 64, 64, 0.5f,0.3f,true,true,false);
   

    ///////////////////////////////
    // --必要素材数cost,Lvの描画--
    ///////////////////////////////
    // --- クリティカル率の強化コスト描画例 ---
    int critCost = SceneManager::Instance().GetCritRateCost();
	int critLv = SceneManager::Instance().GetCritRateLevel();
    DrawNumber(m_numberTex, m_costTex, critCost,20, Math::Vector2(340, 60), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, critLv,10, Math::Vector2(330, 90), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

    // --- 攻撃強度の強化コスト ---
    int atkPowerCost = SceneManager::Instance().GetAtkPowerCost();
	int atkLv = SceneManager::Instance().GetAtkLevel();
    DrawNumber(m_numberTex, m_costTex, atkPowerCost,20, Math::Vector2(340, -40), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, atkLv,10, Math::Vector2(330, -10), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

    // --- 弾速の強化コスト ---
    int bulletSpeedCost = SceneManager::Instance().GetBulletSpeedCost();
	int bulletSpeedLv = SceneManager::Instance().GetBulletSpeedLevel(); 
    DrawNumber(m_numberTex, m_costTex, bulletSpeedCost,20, Math::Vector2(150, -140), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, bulletSpeedLv,10, Math::Vector2(140, -110), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

    // --- クリティカルダメージの強化コスト ---
    int critDmgCost = SceneManager::Instance().GetCritIncreCost();
	int critDmgLv = SceneManager::Instance().GetCritIncreLevel();
    DrawNumber(m_numberTex, m_costTex, critDmgCost,20, Math::Vector2(340, -240), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, critDmgLv,10, Math::Vector2(330, -210), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

    // --- 弾発射間隔の強化コスト ---
    int shotIntervalCost = SceneManager::Instance().GetShotIntervalCost();
	int shotIntervalLv = SceneManager::Instance().GetShotIntervalLevel();
    DrawNumber(m_numberTex, m_costTex, shotIntervalCost,20, Math::Vector2(340, -140), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, shotIntervalLv,10, Math::Vector2(330, -110), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

	// --攻撃力の強化コスト--
    int atkCost = SceneManager::Instance().GetAtkCost();
	int atkBaseLv = SceneManager::Instance().GetAtkBaseLevel(); 
    DrawNumber(m_numberTex, m_costTex, atkCost, 20, Math::Vector2(150, 160),normalCol, 115 , 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, atkBaseLv,10, Math::Vector2(140, 190), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

    // --体力の強化コスト--
    int HpCost = SceneManager::Instance().GetHpCost();
	int HpLv = SceneManager::Instance().GetHpLevel();
    DrawNumber(m_numberTex, m_costTex, HpCost, 20, Math::Vector2(340, 160), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, HpLv,10, Math::Vector2(330, 190), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

    // --防御の強化コスト--
    int defCost = SceneManager::Instance().GetDefCost();
	int defLv = SceneManager::Instance().GetDefLevel(); 
    DrawNumber(m_numberTex, m_costTex, defCost, 20, Math::Vector2(150, 60), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, defLv,10, Math::Vector2(140, 90), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);

	//--幸運クリティカルの強化コスト--
    int luckyCost = SceneManager::Instance().GetLuckyCritCost();
	int luckyLv = SceneManager::Instance().GetLuckyCritLevel();
    DrawNumber(m_numberTex, m_costTex, luckyCost, 20, Math::Vector2(150, -40), normalCol, 115, 32, 0.4f, 0.2f, false, true, false);
    DrawNumber(m_numberTex, m_lvTex, luckyLv,10, Math::Vector2(140, -10), normalCol, 67, 32, 0.4f, 0.2f, false, true, false);


    // 演出用の共通変数
    Math::Matrix mat;
    Math::Vector2 pos;
    float scaleAni;

    ///攻撃力表示
    {
        scaleAni = m_statScales[0]; // 攻撃力用の拡大率
        pos = Math::Vector2(-380, 150);
        if (m_selectIdx == 0) pos.y += m_statShake; // 選択中なら揺らす

        // 行列作成：拡大してから移動
        mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
        SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_BaseStatusTex, displayAtk, 0, Math::Vector2(0,0), atkBaseCol, 55, 20, 0.5f, 0.2f, false, false, false);
    }
    // HP表示
    {
		scaleAni = m_statScales[4]; // HP用の拡大率
		pos = Math::Vector2(-380, 100);
		if (m_selectIdx == 4) pos.y += m_statShake; // 選択中なら揺らす

		mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_BaseStatusTex, displayHp, 0, Math::Vector2(0,0), hpCol, 55, 20, 0.5f, 0.2f, false, false, false);
    }
    // 防御力表示
    {
		scaleAni = m_statScales[1]; // 防御力用の拡大率
		pos = Math::Vector2(-170, 150);
		if (m_selectIdx == 1) pos.y += m_statShake; // 選択中なら揺らす

		mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_BaseStatusTex, displayDef, 0, Math::Vector2(0,0), defCol, 55, 20, 0.5f, 0.2f, false, false, false);
    }
    //クリティカル率
    {
		scaleAni = m_statScales[5]; // クリティカル率用の拡大率
		pos = Math::Vector2(-350, 10);
        if (m_selectIdx == 5) pos.y += m_statShake; // 選択中なら揺らす

		mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_subStatusTex, currentCritRate, 0, Math::Vector2(0,0), critRateCol, 55, 20, 0.5f, 0.3f, false, false, true);
    
        SHADER.m_spriteShader.SetMatrix(baseMat);
    }
    //レイジ上限
	DrawNumber(m_numberTex, m_subStatusTex, ultMaxCost,0, Math::Vector2(-150, 10), normalCol, 55, 20, 0.5f, 0.3f, false, false, false);
    //幸運クリティカル
    {
		scaleAni = m_statScales[2]; // 幸運クリティカル用の拡大率
		pos = Math::Vector2(-350, -50);
		if (m_selectIdx == 2) pos.y += m_statShake; // 選択中なら揺らす

		mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_subStatusTex, currentMaxRndRation, 0, Math::Vector2(0, 0), luckyCol, 55, 20, 0.5f, 0.3f, false, false, true);
    }
    
    //攻撃強度
    {
		scaleAni = m_statScales[6]; // 攻撃強度用の拡大率
		pos = Math::Vector2(-380, -150);
		if (m_selectIdx == 6) pos.y += m_statShake; // 選択中なら揺らす

		mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_subStatusTex, currentAtkPower, 0, Math::Vector2(0, 0), atkPowerCol, 55, 20, 0.5f, 0.3f, false, false, true);
    }
    //弾速
    {
        scaleAni = m_statScales[3]; // 弾速用の拡大率
        pos = Math::Vector2(-200, -150);
        if (m_selectIdx == 3) pos.y += m_statShake; // 選択中なら揺らす

        mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
        SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_subStatusTex, currentBulletSpeed * 10.0f, 0, Math::Vector2(0, 0), speedCol, 55, 20, 0.5f, 0.3f, false, false, true);
    }
    //クリダメ
    {
		scaleAni = m_statScales[8]; // クリダメ用の拡大率
		pos = Math::Vector2(-380, -200);
		if (m_selectIdx == 8) pos.y += m_statShake; // 選択中なら揺らす

		mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_subStatusTex, displayCritDmg, 0, Math::Vector2(0, 0), critDmgCol, 55, 20, 0.5f, 0.3f, false, false, true);
    }
    //弾発射間隔
    {
		scaleAni = m_statScales[7]; // 弾発射間隔用の拡大率
		pos = Math::Vector2(-200, -200);
		if (m_selectIdx == 7) pos.y += m_statShake; // 選択中なら揺らす

		mat = Math::Matrix::CreateScale(scaleAni) * Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
        DrawNumber(m_numberTex, m_subStatusTex, displayPercent, 0, Math::Vector2(0,0), intervalCol, 55, 20, 0.5f, 0.3f, false, false, true);
    }

    // --- 選択枠の描画 ---
    if (m_selectIdx < m_upgrades.size())
    {
        Math::Vector2 pos = m_upgrades[m_selectIdx].pos;

        // アイコンを囲うように少し大きめに描画 (画像サイズに合わせて調整)
        Math::Matrix mat = Math::Matrix::CreateTranslation(pos.x+ 50, pos.y, 0);
        SHADER.m_spriteShader.SetMatrix(mat);

        // 枠のサイズを指定
        SHADER.m_spriteShader.DrawTex(&m_cursorTex, 0, 0, 200, 100, nullptr);
    }

    SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void ResultScene::Release()
{}

void ResultScene::DrawNumber(const KdTexture & tex, const KdTexture& tex2, int value, int add, Math::Vector2 startPos, Math::Color col, float sabWidth, float sabHeight, float scale, float mojiScale, bool b_Multi, bool b_SecTex ,bool b_persent)
{
    std::string str = std::to_string(value); // 数値を文字列に変換して1文字ずつ処理する
    int numWidth = 64;      // 1文字あたりの横幅 (12分割)
    int numHeight = 64;         // 文字の高さ

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

void ResultScene::HandleInput()
{
    // --- カーソル移動 ---
    if (GetAsyncKeyState(VK_UP) & 0x0001)
    {
        // 一つ上へ。一番上なら一番下へループ
        m_selectIdx = (m_selectIdx - 1 + m_upgrades.size()) % m_upgrades.size();
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x0001)
    {
        // 一つ下へ。一番下なら一番上へループ
        m_selectIdx = (m_selectIdx + 1) % m_upgrades.size();
    }

    // 左右移動（画像に合わせて列をジャンプ）
    if ((GetAsyncKeyState(VK_LEFT) & 0x0001) || (GetAsyncKeyState(VK_RIGHT) & 0x0001))
    {
        
        if (m_selectIdx < 4) m_selectIdx += 4;
        else                m_selectIdx -= 4;

        // 項目数を超えないようにガード
        if (m_selectIdx >= (int)m_upgrades.size()) m_selectIdx = (int)m_upgrades.size() - 1;
        if (m_selectIdx < 0) m_selectIdx = 0;
    }

    // --- スペースキーで強化実行 ---
    if (GetAsyncKeyState(VK_SPACE) & 0x0001)
    {
        LevelUp(m_selectIdx);
    }
}

void ResultScene::LevelUp(int idx)
{
    //必要コストを取得
    int cost = GetNeedCost(idx);

    //素材が足りるかチェック
    if (SceneManager::Instance().GetTotalMaterials() < cost) return; // 素材不足

    // 素材を消費 (SceneManagerに消費関数が必要)
    SceneManager::Instance().SubMaterials(cost);


	//--- 強化成功時の演出開始 ---
    if (idx >= 0 && idx < 9) {
        m_statScales[idx] += (1.5f - m_statScales[idx]) * 1.5f;
        m_statShake = (float)(rand() % 20 + 1); // 上にランダムに飛ばす
    }


    switch (idx) 
    {
    case 0: SceneManager::Instance().UpgradePlayerAtk(); break;
    case 1: SceneManager::Instance().UpgradePlayerDef(); break;
    case 2: SceneManager::Instance().UpgradeLuckyCrit(); break;
    case 3: SceneManager::Instance().UpgradeBulletSpeed(); break;
    case 4: SceneManager::Instance().UpgradePlayerHp(); break;
    case 5: SceneManager::Instance().UpgradeCritRate(); break;
    case 6: SceneManager::Instance().UpgradeAtk(); break;
    case 7: SceneManager::Instance().UpgradeShotInterval(); break;
    case 8: SceneManager::Instance().UpgradeCritIncreRate(); break;
    }
}

int ResultScene::GetNeedCost(int idx)
{
    switch (idx) 
    {
    case 0: return SceneManager::Instance().GetAtkCost();
    case 1: return SceneManager::Instance().GetDefCost();
    case 2: return SceneManager::Instance().GetLuckyCritCost();
    case 3: return SceneManager::Instance().GetBulletSpeedCost();
    case 4: return SceneManager::Instance().GetHpCost();
    case 5: return SceneManager::Instance().GetCritRateCost();
    case 6: return SceneManager::Instance().GetAtkCost();
    case 7: return SceneManager::Instance().GetShotIntervalCost();
    case 8: return SceneManager::Instance().GetCritIncreCost();
    }

    return 999;
}
