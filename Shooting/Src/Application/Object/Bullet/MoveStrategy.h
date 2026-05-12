#pragma once
#include"../../Object/BaseObject.h"

// 弾の座標や方向を操作するための情報をまとめた構造体
struct MoveParam
{
    Math::Vector3& pos;
    Math::Vector3& dir;
    float speed;
    float deltaTime;
    KdTexture* p_tex;
    Math::Matrix& m_mat;
	float& radius; // 追加：弾の当たり判定用の半径

    class GameScene* pOwner;
};

// 移動戦略の基底クラス
class MoveStrategy :public BaseObject
{
public:

    MoveStrategy(){}
    virtual ~MoveStrategy() {}

    virtual void Init(MoveParam& param){}
    virtual void Update(MoveParam& param) = 0;
    virtual void Draw(MoveParam& param) = 0;
};