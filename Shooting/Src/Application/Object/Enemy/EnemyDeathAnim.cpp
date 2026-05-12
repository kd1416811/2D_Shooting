#include "EnemyDeathAnim.h"
#include "../../Concept/define.h"

void EnemyDeathAnim::Init()
{
    m_tex.Load("Textures/Enemy/EnemyAnim/DEATH.png");
    m_aliveFlg = true;
    m_animCnt = 0;
	m_animSpeed = 0.14f; // アニメーション速度
    m_maxFrame = 7;
    m_objType = objectType::enemyDeathEffect; // エフェクト扱いに
}

void EnemyDeathAnim::Update()
{
    if (!m_aliveFlg) return;

    // アニメーション進行
    m_animCnt += m_animSpeed;

    // 最後のフレームを超えたら消える
    if (m_animCnt >= m_maxFrame)
    {
        m_aliveFlg = false;
    }

    m_scale = Math::Matrix::CreateScale(2.0f); // 敵と同じサイズに
    m_trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);
    m_mat = m_scale * m_trans;
}

void EnemyDeathAnim::Draw()
{
    if (!m_aliveFlg) return;

    // アニメーション切り出し計算
    // ※Config::EnemyMargin_x などを使っていますが、死亡画像の1枚サイズに合わせて調整してください
    int srcX = (int)m_animCnt * (int)Config::EnemyMargin_x;
    Math::Rectangle srcRect = { srcX, 0, (int)Config::EnemyMargin_x, (int)Config::EnemyMargin_y };

    SHADER.m_spriteShader.SetMatrix(m_mat);
    SHADER.m_spriteShader.DrawTex(&m_tex, srcRect, 1.0f);
}

void EnemyDeathAnim::Release()
{
    m_tex.Release();
}
