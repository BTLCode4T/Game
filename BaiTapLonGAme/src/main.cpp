#include <iostream>
#include <windows.h>
#include "Core/GameLoop.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include "Core/Render.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // Tạo cửa sổ game
    HWND hwnd = CreateGameWindow(hInstance, nCmdShow);
    if (!hwnd)
        return -1;

    //Chạy game loop
    RunGameLoop(hwnd);

    return 0;
}