#pragma once
#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void MovePlayer(WPARAM key);