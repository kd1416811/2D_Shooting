#pragma once
#include"../MoveStrategy.h"

class MoveUltimate : public MoveStrategy
{
public:
	
	MoveUltimate() {}
	~MoveUltimate() {}

	void Init(MoveParam& param) override;
	void Update(MoveParam& param) override;
	void Draw(MoveParam& param) override;

	float GetAnimeCnt() const { return m_animCnt; }
	KdTexture* GetRailgunTex() { return &m_railgunTex; }
	bool IsAnimationFinished() const {
		// 画像が10コマの場合、9.9...を超えたら終了とみなす
		return m_animCnt >= 10.0f;
	}

private:

	KdTexture m_railgunTex;
	KdTexture m_magicCircleTex; // 魔法陣のテクスチャ
	float m_uvOffset;           // UVスクロールのオフセット値（0.0f ～ 1.0f）
	float m_scrollSpeed;        // スクロール速度

	const float m_animSpeed = 0.2f; // アニメーション速度
};