#include "DamageEffect.h"
#include"../../Concept/define.h"
KdTexture DamageEffect::s_numberTex;

void DamageEffect::Init()
{
	m_damage = 0;
	m_alpha = 1.0f;		
	m_speed = 2.0f;		//浮き上がる速度
	m_lifeTime = 60;	//表示時間(１秒)
	m_deltaTime = 1.0f / 60.0f;
	m_ObjScale = 1.0f;
	m_criticalFlg = false;
	m_aliveFlg = true;
	m_color = { 1.0f,1.0f,0.0f,1.0f };//初期は白色
	s_numberTex.Load("Textures/number2.png");
	srand(timeGetTime());

	m_objType = objectType::damageEffect;
}

void DamageEffect::Draw()
{
	if (!m_aliveFlg || m_damage <= 0) return;

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);

	//ダメージ数値をカンマ区切りの文字列に変換
	std::string damageStr = FormatComma(m_damage);

	int numW = 64;
	int numH = 64;
	Math::Color col = { 1.0f,1.0f,0.0f,m_alpha };

	//クリティカルなら虹色に
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f,1.0f);
	Math::Color critivalCol = {dist(gen),dist(gen) ,dist(gen) ,m_alpha};

	Math::Vector2 pivot = { 0.5f,0.5f };

	// --１文字ずつループ描画--
	float totalW = damageStr.length() * (numW * 0.4f); // 0.4fは重なり具合の調整
	float currentX = m_pos.x - (totalW / 2.0f);

	for (char c : damageStr)
	{
		int texIdx = 0;//切り取り位置初期化
		float charWidth = (float)numW * 0.4f;	//次の数字までの距離
		float currentY = m_pos.y;

		//文字が何か判定
		if (c >= '0' && c <= '9')
		{
			texIdx = (c + 1) - '0';
		}
		else if (c == ',')
		{
			charWidth *= 0.7f;		// カンマは幅を狭くする
			currentY -= 10.0f;	// カンマ表示位置を少し下げる
			texIdx = 11;
		}
		else
		{
			continue;
		}

		//描画

		Math::Matrix moveMat = Math::Matrix::CreateTranslation(currentX, currentY, 0);

		// Updateで作ったm_scaleがあれば反映させる
		Math::Matrix finalMat = m_scale * moveMat;

		// シェーダーに行列をセット
		SHADER.m_spriteShader.SetMatrix(finalMat);

		// 切り抜き範囲
		Math::Rectangle rect = { texIdx * numW, 0, numW, numH };

		// 行列をセットした後は、座標(0,0)描画する
		if (!m_criticalFlg)
		{
			SHADER.m_spriteShader.DrawTex(&s_numberTex, 0, 0, One_Half(numW), One_Half(numH), &rect, &m_color, pivot);
		}
		else
		{
			SHADER.m_spriteShader.DrawTex(&s_numberTex, 0,0, One_Half(numW), One_Half(numH), &rect, &critivalCol, pivot);
		}

		currentX += charWidth;
	}

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void DamageEffect::Update()
{
	if (!m_aliveFlg)return;

	m_pos.y += m_speed;//上に
	m_alpha -= m_deltaTime;
	m_lifeTime--;

	if (m_lifeTime <= 0)
	{
		m_aliveFlg = false;
	}

	m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_scale = Math::Matrix::CreateScale(m_ObjScale);
	m_mat = m_scale * m_trans;
}

void DamageEffect::SetDamage(long long damage, const Math::Vector3 & pos, bool b_critical, float speedRate)
{
	m_damage = damage;
	m_pos = pos;
	m_pos.x += (rand() % 60) - 30;
	m_pos.y += 30;
	m_criticalFlg = b_critical;
	m_speed = 2.0f * speedRate;
}

std::string DamageEffect::FormatComma(long long value)
{
	std::string str_damage = std::to_string(value);

	int commaPoint = str_damage.length() - 3;

	while (commaPoint > 0)
	{
		str_damage.insert(commaPoint, ",");
		commaPoint -= 3;
	}
	return str_damage;
}
