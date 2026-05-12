#include "MoveWave.h"
#include"../../../Concept/define.h"

void MoveWave::Update(MoveParam& param)
{
    m_angle += 0.2f;

    // 進行方向に対して垂直なベクトルを計算
    Math::Vector3 sideDir = { -param.dir.y, param.dir.x, 0 };
    param.pos += (param.dir * param.speed) + (sideDir * sin(m_angle) * 10.0f);

    param.m_mat = Math::Matrix::CreateTranslation(param.pos.x, param.pos.y + 10.0f, param.pos.z);
   //m_trans = Math::Matrix::CreateTranslation(param.pos.x, param.pos.y, 0);
   //m_mat = m_trans;
}

void MoveWave::Draw(MoveParam& param)
{
    if (!param.p_tex)return;

    //SHADER.m_spriteShader.SetMatrix(m_mat);
    //SHADER.m_spriteShader.DrawTex(param.p_tex, { 0,0,(int)Config::BulletMargin_y,(int)Config::BulletMargin_y }, 1.0f);
}
