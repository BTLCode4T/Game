@echo off
echo Dang chay du an

REM --- Chạy lệnh biên dịch tất cả các file cần thiết ---
g++ src\main.cpp src\Core\Window.cpp src\Core\GameLoop.cpp src\Core\Render.cpp src\Core\Input.cpp -o main.exe -lgdi32 -std=c++17 -Wall

IF %ERRORLEVEL% NEQ 0 (
    echo Loi khong bien dich code
    pause
    exit /b
)

echo Bien dich thanh cong
echo Chay truong trinh
main.exe
pause
