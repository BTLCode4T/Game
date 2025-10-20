#include <iostream>
#include <windows.h>
#include "Render.h"

RenderContext g_RenderContext = {0};

//  Bắt đầu vẽ (tạo backbuffer)
void BeginDraw(HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    g_RenderContext.width  = rect.right - rect.left;
    g_RenderContext.height = rect.bottom - rect.top;

    g_RenderContext.hdcWindow = GetDC(hwnd);
    g_RenderContext.hdcBuffer = CreateCompatibleDC(g_RenderContext.hdcWindow);
    g_RenderContext.hBitmap   = CreateCompatibleBitmap(g_RenderContext.hdcWindow, g_RenderContext.width, g_RenderContext.height);
    g_RenderContext.hOldBitmap = (HBITMAP)SelectObject(g_RenderContext.hdcBuffer, g_RenderContext.hBitmap);

    std::cout << "BeginDraw: Backbuffer created, size " 
              << g_RenderContext.width << "x" << g_RenderContext.height << "\n";
}

//  Kết thúc vẽ (blit backbuffer ra màn hình)
void EndDraw(HWND hwnd)
{
    BitBlt(g_RenderContext.hdcWindow, 0, 0,
           g_RenderContext.width, g_RenderContext.height,
           g_RenderContext.hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(g_RenderContext.hdcBuffer, g_RenderContext.hOldBitmap);
    DeleteObject(g_RenderContext.hBitmap);
    DeleteDC(g_RenderContext.hdcBuffer);
    ReleaseDC(hwnd, g_RenderContext.hdcWindow);

    g_RenderContext.hdcBuffer = nullptr;
    g_RenderContext.hdcWindow = nullptr;
    g_RenderContext.hBitmap   = nullptr;
    g_RenderContext.hOldBitmap = nullptr;

    std::cout << "EndDraw: Frame rendered to window\n";
}

//  Xóa màn hình với màu nhất định
void ClearScreen(HDC hdc, HWND hwnd, COLORREF color)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    HBRUSH brush = CreateSolidBrush(color);
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);

    std::cout << "ClearScreen: Cleared with color 0x" << std::hex << color << std::dec << "\n";
}

//  Vẽ bitmap tại vị trí (x, y)
void DrawImageAt(HDC hdc, HBITMAP hBitmap, int x, int y)
{
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBitmap);

    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BitBlt(hdc, x, y, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBmp);
    DeleteDC(memDC);

    std::cout << "DrawImageAt: Bitmap drawn at (" << x << "," << y << ") size " 
              << bmp.bmWidth << "x" << bmp.bmHeight << "\n";
}

//  Vẽ bitmap full screen
void DrawImageFullScreen(HDC hdc, HBITMAP hBitmap, int width, int height)
{
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBitmap);

    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    StretchBlt(hdc, 0, 0, width, height, memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

    SelectObject(memDC, oldBmp);
    DeleteDC(memDC);

    std::cout << "DrawImageFullScreen: Bitmap stretched to " << width << "x" << height << "\n";
}

//  Load BMP từ file
HBITMAP LoadImageFromFile(LPCWSTR path)
{
    HBITMAP hBmp = (HBITMAP)LoadImageW(nullptr, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    std::cout << "LoadImageFromFile: " << (hBmp ? "Success" : "Failed") << "\n";
    return hBmp;
}

//  Giải phóng bitmap
void FreeImage(HBITMAP hBitmap)
{
    if (hBitmap)
    {
        DeleteObject(hBitmap);
        std::cout << "FreeImage: Bitmap deleted\n";
    }
}

//  Vẽ hình chữ nhật
void DrawRect(HDC hdc, int x, int y, int w, int h, COLORREF color)
{
    std::cout << "DrawRect: (" << x << "," << y << ") size " << w << "x" << h 
              << " color 0x" << std::hex << color << std::dec << "\n";

    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    Rectangle(hdc, x, y, x + w, y + h);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
}

//  Tô hình chữ nhật
void FillRectColor(HDC hdc, int x, int y, int w, int h, COLORREF color)
{
    HBRUSH brush = CreateSolidBrush(color);
    RECT rect = {x, y, x + w, y + h};
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);

    std::cout << "FillRectColor: (" << x << "," << y << ") size " << w << "x" << h 
              << " color 0x" << std::hex << color << std::dec << "\n";
}

// Vẽ chữ
void DrawTextAt(HDC hdc, LPCWSTR text, int x, int y, COLORREF color)
{
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);
    TextOutW(hdc, x, y, text, lstrlenW(text));

    std::wcout << L"DrawTextAt: \"" << text << L"\" at (" << x << "," << y << ")\n";
}

//  Dọn renderer
void CleanupRenderer()
{
    if (g_RenderContext.hBitmap)
        DeleteObject(g_RenderContext.hBitmap);
    if (g_RenderContext.hdcBuffer)
        DeleteDC(g_RenderContext.hdcBuffer);
    
    g_RenderContext = {0};
    std::cout << "CleanupRenderer: Renderer cleaned\n";
}
