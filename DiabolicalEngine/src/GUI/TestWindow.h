#pragma once
#include "IWindow.h"
class TestWindow :
    public IWindow
{
public:
    TestWindow();
    void Draw() override;
    uint32_t Tex = 0;
};

