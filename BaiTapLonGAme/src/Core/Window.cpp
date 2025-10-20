#include "Window.h"
#include "Input.h"

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"MyGameWindowClass";

    WNDCLASS wc = {};

    // wc.lpfnWndProc con trỏ tới hàm xử lý thông điệp của cửa sổ là WindowProc, tức khi gọi hàm này, thì cửa sổ Window sẽ được xử lý bằng WindowProc
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
        return nullptr;

    // hwnd một con trỏ hoặc số đặc biệt mà Windows dùng để xác định cửa sổ nào bạn đang nói tới.
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"My WinAPI Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
        return nullptr;

    ShowWindow(hwnd, nCmdShow);
    
    return hwnd;
}
