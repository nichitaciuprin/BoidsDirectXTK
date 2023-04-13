#pragma once

#include <iostream>
using namespace std;

// TODO
// SetPosition ignores invisible border
// Calling SetPosition(0,0,width,height) makes window position slightly right of monitor
class Console
{
public:
    static void Init()
    {
        if (consoleCreated) return;
        consoleCreated = true;

        auto curentWindow = GetActiveWindow();
        AllocConsole();
        SetFocus(curentWindow);
        hwnd = GetConsoleWindow();

        if (freopen_s(&fileIn ,"CONIN$" ,"r",stdin )) throw;
        if (freopen_s(&fileOut,"CONOUT$","w",stdout)) throw;
        if (freopen_s(&fileErr,"CONOUT$","w",stderr)) throw;
    }
    static void SetPosition(int x, int y, int width, int height)
    {
        SetWindowPos(hwnd,0,x,y,width,height,SWP_NOSIZE);
    }
    static void Show()
    {
        ShowWindow(hwnd, SW_NORMAL);
    }
    static void ShowFullscreen()
    {
        auto uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;
        SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, uFlags);
        ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    }
    static void Close()
    {
        CloseWindow(hwnd);
    }
private:
    static bool consoleCreated;
    static FILE* fileIn;
    static FILE* fileOut;
    static FILE* fileErr;
    static HWND hwnd;
};
bool Console::consoleCreated = false;
FILE* Console::fileIn = nullptr;
FILE* Console::fileOut = nullptr;
FILE* Console::fileErr = nullptr;
HWND Console::hwnd = nullptr;