#pragma once
#include <iostream>
// #include <stdio.h>

FILE* fileIn;
FILE* fileOut;
FILE* fileErr;

bool consoleCreated = false;
void Console_MaybeCreate()
{
    if (consoleCreated) return;
    AllocConsole();
    auto consoleWindow = GetConsoleWindow();
    errno_t result;
    // throw;
    result = freopen_s(&fileIn ,"CONIN$" ,"r",stdin ); if (result != 0) throw;
    result = freopen_s(&fileOut,"CONOUT$","w",stdout); if (result != 0) throw;
    result = freopen_s(&fileErr,"CONOUT$","w",stderr); if (result != 0) throw;
    // std::cout << "Hello\n";
    SetWindowPos(consoleWindow,0,2000,0,0,0,SWP_NOSIZE);
    consoleCreated = true;
}
void Console_Write(int msg)
{
    Console_MaybeCreate();
    std::cout << msg;
}
void Console_WriteLine(int msg)
{
    Console_MaybeCreate();
    std::cout << msg << '\n';
}