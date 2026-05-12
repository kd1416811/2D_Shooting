#pragma once
#include"../MoveStrategy.h"

class MoveWave : public MoveStrategy
{
    
public:

    MoveWave(){}

    void Update(MoveParam& param) override;
    void Draw(MoveParam& param) override;

private:

    float m_angle = 0.0f;

};