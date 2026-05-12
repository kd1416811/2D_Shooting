#include "SpaceBackground.h"
#include"../../../Concept/define.h"

SpaceBackground::SpaceBackground(){}

SpaceBackground::~SpaceBackground(){}

void SpaceBackground::Init()
{
	srand(timeGetTime());

	int halfW = (Config::SCREEN_WIDTH / 2);
	int halfH = (Config::SCREEN_HEIGHT / 2);

	for (int i = 0 ; i < StarCount; ++i)
	{
		Star s;
		s.pos.x = (rand() % (Config::SCREEN_WIDTH)) - halfW;
		s.pos.y = (rand() % (Config::SCREEN_HEIGHT)) - halfW;

		float rad = (rand() % 100) / 150.0f;;
		s.size = 0.5f + rad * rad * 2.5f;
		s.speed = s.size * 2.0f;

		s.col = { Rnd(),Rnd(),0.8f,1.0f};

		m_stars.push_back(s);
	}

	for (int i = 0; i < nebulaCnt; ++i)
	{ 
		Nebula n;
		n.pos.x = (rand() % Config::SCREEN_WIDTH) - halfW;
		n.pos.y = (rand() % Config::SCREEN_HEIGHT) - halfW;
		n.size = 1500.0f; 
		n.rotation = (rand() % halfW) * 0.017f; // ラジアン
		n.speed = 0.2f + (i * 0.1f);          // 星よりも圧倒的に遅くする
		n.rotSpeed = 0.001f;

		n.col = { 0.2f, 0.4f, 0.8f, 0.15f };
		m_nebulas.push_back(n);
	}

	//for (int i = 0; i < planetCnt; ++i) 
	//{
	//	Planet p;
	//	p.pos.x = (rand() % Config::SCREEN_WIDTH) - halfW;
	//	p.pos.y = (rand() % Config::SCREEN_HEIGHT) - halfW;

	//	p.size = 100.0f + (rand() % 200);

	//	p.speed = 0.1f + (i * 0.05f);

	//	p.rotation = 0.0f;
	//	p.rotSpeed = 0.005f; // ゆっくり自転させる

	//	if (i == 0) p.col = { 0.7f, 0.8f, 1.0f, 1.0f }; // 薄い青
	//	else       p.col = { 1.0f, 0.5f, 0.3f, 1.0f }; // オレンジ

	//	m_planets.push_back(p);
	//}
	
	//背景
	m_tex.Load("Textures/Background/Space/spaceBackground.png");
	m_starTex.Load("Textures/Background/Space/star.png");
	m_nebulaTex.Load("Textures/Background/Space/nebula.png");
	//m_planetTex.Load("Textures/Background/Space/planet.png");
}

void SpaceBackground::Draw()
{
	D3D.SetBlendState(BlendMode::NoBlend);

	//背景
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(&m_tex, { 0,0,1280,720 }, 1.0f);


	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	D3D.SetBlendState(BlendMode::Add);
	
	Math::Vector2 pivot = { 0.5f,0.5f };

	//星雲
	Math::Rectangle nebulaRect = { 0,0,512,512 };

	for (const auto& n : m_nebulas) 
	{
		SHADER.m_spriteShader.DrawTex(
			&m_nebulaTex,
			n.pos.x, n.pos.y, n.size, n.size,
			&nebulaRect, &n.col, pivot
		);
	}

	//惑星

	//Math::Rectangle planetRect = { 0,0,283,213 };
	//for (const auto& p : m_planets) 
	//{
	//	SHADER.m_spriteShader.DrawTex(
	//		&m_planetTex,
	//		p.pos.x, p.pos.y, p.size, p.size,
	//		&planetRect, &p.col, pivot
	//	);
	//}

	//星
	Math::Rectangle starRect = { 0,0,1,1 };

	for (const auto& s : m_stars)
	{
		SHADER.m_spriteShader.DrawTex(
			&m_starTex, 
			(s.pos.x - s.size), (s.pos.y - s.size),
			(s.size * 2), (s.size * 2), 
			&starRect, &s.col, pivot
		);
	}
}
void SpaceBackground::Update()
{
	float halfW = Config::SCREEN_WIDTH / 2.0f;
	float halfH = Config::SCREEN_HEIGHT / 2.0f;

	for (auto& s : m_stars)
	{
		s.pos.x += s.speed;

		// 画面外に出たら左に戻す
		if (s.pos.x > halfW) {
			s.pos.x = -(halfW) - (s.size * 2);
			s.pos.y = (rand() % (Config::SCREEN_HEIGHT)) - halfH;
		}
	}

	for (auto& n : m_nebulas) 
	{
		n.pos.x += n.speed;    // ゆっくり流れる
		n.rotation += n.rotSpeed; // ゆっくり回転

		// 右端（画面幅+サイズ半分）を超えたら左へ
		if (n.pos.x > halfW + n.size / 2) {
			n.pos.x = -(halfW)-(n.size / 2);
		}

		n.m_trans = Math::Matrix::CreateTranslation(n.pos.x, n.pos.y, 0.0f);
		n.m_rotation = Math::Matrix::CreateRotationZ(n.rotation);
		n.m_mat = n.m_rotation * n.m_trans;
	}

	//for (auto& p : m_planets) 
	//{
	//	p.pos.x += p.speed;     
	//	p.rotation += p.rotSpeed; 

	//	// 右端（画面幅+サイズ半分）を超えたら左へ
	//	if (p.pos.x > halfW + p.size / 2) {
	//		p.pos.x = -halfW - p.size / 2;
	//		p.pos.y = (rand() % Config::SCREEN_HEIGHT) - halfH; // ループ時に高さを変える
	//	}

	//	p.m_trans = Math::Matrix::CreateTranslation(p.pos.x, p.pos.y, 0.0f);
	//	p.m_rotation = Math::Matrix::CreateRotationZ(p.rotation);
	//	p.m_mat = p.m_rotation * p.m_trans;
	//}

	m_trans = Math::Matrix::CreateTranslation(0, 0, 0);
	m_mat = m_trans;
}

float SpaceBackground::Rnd()
{
	return rand() / (float)RAND_MAX;
}


