#pragma once
#include <iostream>
// #include <stdio.h>

void Console_Test()
{
    AllocConsole();
    auto consoleWindow = GetConsoleWindow();
    auto resultIn  = freopen("CONIN$" ,"r",stdin);  if (resultIn  == NULL) return;
    auto resultOut = freopen("CONOUT$","w",stdout); if (resultOut == NULL) return;
    auto resultErr = freopen("CONOUT$","w",stderr); if (resultErr == NULL) return;
    std::cout << "Hello\n";
    SetWindowPos(consoleWindow,0,2000,0,0,0,SWP_NOSIZE);
}