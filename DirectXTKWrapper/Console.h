#pragma once

#include <iostream>
using namespace std;

class Console
{
public:
    static void WriteLine(string msg)
    {
        Init();
        std::cout << msg << '\n';
    }
    static void WriteLineInt(int msg)
    {
        Init();
        std::cout << msg << '\n';
    }
    static void WriteLineLong(long msg)
    {
        Init();
        std::cout << msg << '\n';
    }
private:
    static void Init()
    {
        if (consoleCreated) return;
        auto curentWindow = GetActiveWindow();
        AllocConsole();
        SetFocus(curentWindow);
        auto consoleWindow = GetConsoleWindow();
        errno_t result;
        result = freopen_s(&fileIn ,"CONIN$" ,"r",stdin ); if (result != 0) throw;
        result = freopen_s(&fileOut,"CONOUT$","w",stdout); if (result != 0) throw;
        result = freopen_s(&fileErr,"CONOUT$","w",stderr); if (result != 0) throw;
        SetWindowPos(consoleWindow,0,2000,0,0,0,SWP_NOSIZE);
        consoleCreated = true;
    }
    static bool consoleCreated;
    static FILE* fileIn;
    static FILE* fileOut;
    static FILE* fileErr;
};