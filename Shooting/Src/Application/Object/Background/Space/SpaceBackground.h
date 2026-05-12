#pragma once
#include"../BackgroundBase.h"

struct  Star
{
    Math::Vector2 pos;
    float size;
    float brightness;
    float speed;
    Math::Color col;
};

struct Nebula
{
    Math::Vector2 pos;
    float rotation;
    float speed;
    float rotSpeed;
    float size;
    Math::Color col;
    Math::Matrix m_trans;
    Math::Matrix m_rotation;
    Math::Matrix m_mat;
};

struct Planet
{
    Math::Vector2 pos;
    float size;     
    float speed;     
    float rotation;  
    float rotSpeed;  
    Math::Color col; 
    Math::Matrix m_trans;
    Math::Matrix m_rotation;
    Math::Matrix m_mat;
};

class SpaceBackground : public BackgroundBase 
{
public:

    SpaceBackground();
    ~SpaceBackground();

    void Init() override;
    void Draw()override;
    void Update()override;

private:

    float Rnd();

    //星の数
    static const int StarCount = 300;
    std::vector<Star>m_stars;

    //星雲の数
    static const int nebulaCnt = 6;
    std::vector<Nebula> m_nebulas;

    //惑星の数
    static const int planetCnt = 2;
    std::vector<Planet> m_planets; 


    KdTexture m_nebulaTex;
    KdTexture m_starTex;
    KdTexture m_planetTex;

    Math::Matrix m_trans;
    Math::Matrix m_mat;
};