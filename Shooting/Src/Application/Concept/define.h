#pragma once

namespace Config 
{
	static constexpr int SCREEN_WIDTH = 1280;       //画面の端から端までの長さ
	static constexpr int SCREEN_HEIGHT = 720;       //画面の上から下までの長さ

	static constexpr float PlayerMargin = 64.0f;    //自機の大きさ
	static constexpr float EnemyMargin = 64.0f;     //敵の大きさ
	static constexpr float BulletMargin = 16.0f;    //弾の大きさ

	// 半分にする関数
	//float One_Half(float v) { return v * 0.5f; }
}





