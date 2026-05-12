#include "MoveHoming.h"
#include "../../../Scene/GameScene.h"
#include"../../../Concept/define.h"

void MoveHoming::Update(MoveParam& param)
{
    if (!param.pOwner) return;

    Math::Vector3 targetPos = { 0, 0, 0 };
    bool found = false;

    // gameSceneのインスタンス経由で敵を探す例
    for (auto& obj : param.pOwner->GetObjList())
    {
        if (obj->GetObjType() == BaseObject::objectType::enemy && obj->GetAliveFlg()) {
            targetPos = obj->GetPos();
            found = true;
            break; // 最初に見つかった敵を追尾
        }
    }

    if (found) {
        // 2. ターゲットへの方向ベクトルを計算
        Math::Vector3 toTarget = targetPos - param.pos;
        toTarget.Normalize();

        // 3. 現在の移動方向に「ターゲットへの方向」を少しだけ混ぜる（補間）
        // 0.05f の値を大きくすると急カーブ、小さくすると緩やかなカーブになります
        float turnSpeed = 0.05f;
        param.dir = Math::Vector3::Lerp(param.dir, toTarget, turnSpeed);
        param.dir.Normalize();

		param.speed += 0.2f;//徐々に加速
    }

    // 4. 移動反映
    param.pos += param.dir * param.speed;

    float angle = atan2f(param.dir.y, param.dir.x);
    Math::Matrix rot = Math::Matrix::CreateRotationZ(angle);
    Math::Matrix trans = Math::Matrix::CreateTranslation(param.pos.x, param.pos.y + 10.0f, param.pos.z);

    param.m_mat = rot * trans;
}

void MoveHoming::Draw(MoveParam& param)
{
    if (!param.p_tex)return;

    SHADER.m_spriteShader.SetMatrix(m_mat);
    SHADER.m_spriteShader.DrawTex(param.p_tex, { 0,0,(int)Config::BulletMargin_y,(int)Config::BulletMargin_y }, 1.0f);
}
