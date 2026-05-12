#pragma once

class BackgroundBase
{
public:

    BackgroundBase();
    virtual ~BackgroundBase();

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

protected:

    KdTexture m_tex;
};