#include "MoveUltimate.h"

void MoveUltimate::Init(MoveParam& param)
{
	m_uvOffset = 0.0f; // 初期値
	m_scrollSpeed = 0.005f; // 1フレームにずらす量
	m_animCnt = 0;
	param.radius = 150.0f;
	m_aliveFlg = true;
	m_magicCircleTex.Load("Textures/Player/mss01-2.png");
	m_railgunTex.Load("Textures/Player/Charge2.png");
}

void MoveUltimate::Update(MoveParam& param)
{
	if (!m_aliveFlg)return;

	// ★1. UVオフセットを更新
	m_uvOffset += m_scrollSpeed;

	// 1.0を超えたらリセット
	if (m_uvOffset > 1.0f)
	{
		m_uvOffset -= 1.0f;
	}

	m_animCnt += m_animSpeed;
	if (m_animCnt >= 10.0f) m_animCnt = 10.0f; // 画像が10コマと想定

	if (m_animCnt < 3.0f) {
		param.radius = 20.0f;  // 出始めは小さい
	}
	else if (m_animCnt < 8.0f) {
		param.radius = 150.0f; // 雷がバリバリ出ている間は大きい
	}
	else {
		param.radius = 0.0f;   // 消え際（最後のコマ）は判定をなくす
	}

	m_scale = Math::Matrix::CreateScale(2.0f); // 魔法陣のサイズを2倍にする
	m_rotation = Math::Matrix::CreateRotationZ(0);
    m_trans = Math::Matrix::CreateTranslation(param.pos.x, param.pos.y + 100.0f, param.pos.z);
    m_mat = m_scale * m_rotation * m_trans;

	param.m_mat = m_mat; // 呼び出し元にも変換行列を渡す

	param.p_tex = &m_magicCircleTex; // 呼び出し元にもテクスチャを渡す
}

void MoveUltimate::Draw(MoveParam & param)
{
  
}
