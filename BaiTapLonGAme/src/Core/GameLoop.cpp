#include "GameLoop.h"
#include "Input.h"
#include "Window.h"


// hàm này để chạy game, tức là chạy trong vòng lập để lập liên tục, để chạy hàm cảu input.h liên tục
void RunGameLoop(HWND hwnd)
{

    MSG msg = {};
    bool isRunning = true;  // điều khiển vòng lòng dừng và tiếp tục
    DWORD lastTime = GetTickCount(); //ta cần biết mất bao nhiêu thời gian giữa 2 khung hình để xử lý chuyển động

    // 🎮 Game Loop
    while (isRunning)
    {
        // Xử lý message Windows: vd đơn giản là nó sẽ gửi đến cho input, vào từ input sẽ thực hiện lệnh
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                isRunning = false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 🕒 DeltaTime
        DWORD currentTime = GetTickCount();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        (void)deltaTime;
        lastTime = currentTime;

    }
}