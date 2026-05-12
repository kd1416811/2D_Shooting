#pragma once
#include "../BaseObject.h"

class EnemyDeathAnim : public BaseObject
{
public:
    EnemyDeathAnim() {}
    ~EnemyDeathAnim() override { Release(); }

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetPos(const Math::Vector3& pos) { m_pos = pos; }

private:
    float m_animCnt;
    float m_animSpeed ; // アニメーション速度
    int m_maxFrame ;      // 画像の枚数（例として6枚）
    KdTexture m_tex;
};