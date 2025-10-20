#include "Window.h"
#include "Input.h"
#include "Render.h"
#include <iostream>

using namespace std;

// ví dụ ========== ===================================
int playerX = 100;
int playerY = 100;
int playerSize = 50;
int playerSpeed = 5;
// =================== ===================================

extern RenderContext g_RenderContext;

// sử lý của sổ window đã gọi
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
// ví dụ =================================== =================================== ===================================
    case WM_KEYDOWN: // Windows gửi khi bạn nhấn phím.
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);

        MovePlayer(wParam);                // gọi hàm di chuyển nhân vật
        InvalidateRect(hwnd, NULL, FALSE); // yêu cầu vẽ lại cửa sổ, báo Windows rằng cửa sổ cần vẽ lại, tức là sẽ gọi lại WM_PAINT
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;        // dùng để chứa thông tin khi ta vẽ
        BeginPaint(hwnd, &ps); // BeginPaint là cách bạn “mở khung vẽ”.

        BeginDraw(hwnd);                                                                                    // BeginDraw() = "Chuẩn bị giấy vẽ trong trí nhớ để vẽ trước."
        ClearScreen(g_RenderContext.hdcBuffer, hwnd, RGB(30, 30, 30));                                      // xóa toàn bộ màn hình (backbuffer) bằng một màu (xám tối).
        FillRectColor(g_RenderContext.hdcBuffer, playerX, playerY, playerSize, playerSize, RGB(255, 0, 0)); // Bạn đang “vẽ trước vào tấm hình ẩn”, chưa đưa lên màn hình thật.
        EndDraw(hwnd); // EndDraw() = “Đưa bức tranh đã vẽ xong trong trí nhớ ra màn hình thật.”

        EndPaint(hwnd, &ps); //đóng giấy phép vẽ” bạn mở lúc nãy bằng BeginPaint().
        std::cout << "WM_PAINT called, player pos = " << playerX << ", " << playerY << "\n";
    }
    break;
// =================================== =================================== ===================================

    case WM_DESTROY:
        CleanupRenderer();
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ví dụ ===================================
// đây là vd về việc di chuyển nhân vật
void MovePlayer(WPARAM key)
{
    switch (key)
    {
    case 'W':
        cout << "W";
        playerY -= playerSpeed;
        break;
    case 'S':
        cout << "s";
        playerY += playerSpeed;
        break;
    case 'A':
        cout << "a";
        playerX -= playerSpeed;
        break;
    case 'D':
        cout << "d";
        playerX += playerSpeed;
        break;
    }
}
//  ==========================================
