#pragma once
#include "IWindow.h"
class TestWindow :
    public IWindow
{
public:
    TestWindow();
    void Draw() override;
    static unsigned int Tex;
};

