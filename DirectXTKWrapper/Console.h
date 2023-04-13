#pragma once

#include <iostream>
using namespace std;

class Console
{
public:
    static void Init(int x, int y, int width, int height)
    {
        if (consoleCreated) return;
        consoleCreated = true;

        auto curentWindow = GetActiveWindow();
        AllocConsole();
        SetFocus(curentWindow);
        auto consoleWindow = GetConsoleWindow();
        SetWindowPos(consoleWindow,0,x,y,width,height,SWP_NOSIZE);

        errno_t result;
        result = freopen_s(&fileIn ,"CONIN$" ,"r",stdin ); if (result != 0) throw;
        result = freopen_s(&fileOut,"CONOUT$","w",stdout); if (result != 0) throw;
        result = freopen_s(&fileErr,"CONOUT$","w",stderr); if (result != 0) throw;

        // FOR FULL
        // SetWindowLongPtr(consoleWindow, GWL_STYLE, WS_POPUP);
        // SetWindowLongPtr(consoleWindow, GWL_EXSTYLE, WS_EX_TOPMOST);
        // auto uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;
        // SetWindowPos(consoleWindow, HWND_TOP, 0, 0, 0, 0, uFlags);
        // ShowWindow(consoleWindow, SW_SHOWMAXIMIZED);
    }
private:
    static bool consoleCreated;
    static FILE* fileIn;
    static FILE* fileOut;
    static FILE* fileErr;
};
bool Console::consoleCreated = false;
FILE* Console::fileIn = nullptr;
FILE* Console::fileOut = nullptr;
FILE* Console::fileErr = nullptr;
