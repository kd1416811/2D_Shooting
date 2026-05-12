#pragma once
#include"../MoveStrategy.h"

class MoveHoming :public MoveStrategy
{
public:

    void Update(MoveParam& param) override;
    void Draw(MoveParam& param)override;

private:
};