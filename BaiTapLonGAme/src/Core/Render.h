#pragma once
#include <windows.h>

//  Cấu trúc chứa thông tin render
struct RenderContext {
    HDC hdcWindow;      // DC của cửa sổ thật
    HDC hdcBuffer;      // DC dùng làm backbuffer
    HBITMAP hBitmap;    // Bitmap chứa nội dung backbuffer
    HBITMAP hOldBitmap; // Bitmap cũ (để restore lại)
    int width;
    int height;
};


//  =======================
//  DANH SÁCH HÀM RENDER CƠ BẢN
// =======================


// Hàm “chuẩn bị vẽ”
// - Tạo backbuffer (Memory DC) để vẽ ẩn trong RAM
// - Giúp chống nhấp nháy khi hiển thị (double buffering)
void BeginDraw(HWND hwnd);

// Hàm “kết thúc vẽ”
// - Sao chép nội dung từ backbuffer ra màn hình thật
// - Giải phóng DC tạm sau khi vẽ xong
void EndDraw(HWND hwnd);

// Hàm “xóa màn hình”
// - Tô toàn bộ background bằng một màu nhất định
// - Dùng đầu mỗi khung hình (frame)
void ClearScreen(HDC hdc, HWND hwnd, COLORREF color);

// Hàm “vẽ hình ảnh ở vị trí cụ thể”
// - Hiện một ảnh (bitmap) tại toạ độ x, y
// - Dùng cho vật thể, nhân vật, icon, item, v.v.
void DrawImageAt(HDC hdc, HBITMAP hBitmap, int x, int y);

// Hàm “vẽ hình ảnh phủ toàn màn hình”
// - Hiển thị ảnh background hoặc giao diện menu
// - Ảnh sẽ được Stretch (phóng to / thu nhỏ) vừa khung hình
void DrawImageFullScreen(HDC hdc, HBITMAP hBitmap, int width, int height);

// Hàm “tải hình ảnh từ file”
// - Đọc file BMP từ ổ đĩa và trả về HBITMAP
// - Dùng để load tài nguyên trước khi render
HBITMAP LoadImageFromFile(LPCWSTR path);

// Hàm “giải phóng hình ảnh”
// - Xóa ảnh khỏi bộ nhớ khi không còn dùng nữa
void FreeImage(HBITMAP hBitmap);

// Hàm “vẽ hình chữ nhật rỗng”
// - Dùng để debug hitbox, khung, hoặc UI viền
void DrawRect(HDC hdc, int x, int y, int w, int h, COLORREF color);

// Hàm “tô hình chữ nhật đầy màu”
// - Dùng cho background, thanh máu, khung menu, v.v.
void FillRectColor(HDC hdc, int x, int y, int w, int h, COLORREF color);

// Hàm “vẽ chữ lên màn hình”
// - Hiển thị text (HUD, FPS, thông báo, menu, tên người chơi)
void DrawTextAt(HDC hdc, LPCWSTR text, int x, int y, COLORREF color);

// Hàm “dọn tài nguyên Renderer”
// - Giải phóng backbuffer, DC, hoặc hình ảnh còn sót
// - Gọi khi thoát game hoặc đổi scene
void CleanupRenderer();