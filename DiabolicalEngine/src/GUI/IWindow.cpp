#include "IWindow.h"
DVector<IWindow*> IWindow::WindowList;

IWindow::IWindow()
{
    static size_t WindowIDCount = 0;
    WindowID = WindowIDCount++;
    IWindow::WindowList.PushBack(this);
}

IWindow::~IWindow() {
    for (size_t i = 0; i < IWindow::WindowList.Size(); i++)
    {
        if (IWindow::WindowList[i] == this)
        {
            IWindow::WindowList.RemoveAt(i);
        };
    }
}

bool IWindow::operator==(const IWindow& RHS)
{
    return WindowID == RHS.WindowID;
}

void IWindow::Tick()
{
    if (bEnabled) {
        Draw();
    }
}

bool IWindow::IsShown()
{
    return bEnabled;
}

bool IWindow::Enable()
{
    bEnabled = true;
    return true;
}

bool IWindow::Disable()
{
    bEnabled = false;
    return false;
}

void IWindow::Toggle()
{
    if (bEnabled) Disable();
    else Enable();
}
