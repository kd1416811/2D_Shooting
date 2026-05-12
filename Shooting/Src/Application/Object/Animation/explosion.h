#pragma once
#include "../BaseObject.h"

class Explosion : public BaseObject 
{
public:

    Explosion(){}
    ~Explosion(){}

    void Init() override;
    void Update() override;
    void Draw() override;

    void SetPos(const Math::Vector3& pos) { m_pos = pos; }

private:

    int m_frame;        // 現在のフレーム
    int m_animeIdx;     // アニメーションのコマ番号
    static constexpr int ANIME_MAX = 12; // 4x4の16分割を想定
};