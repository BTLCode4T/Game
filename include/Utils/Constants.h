#pragma once

// --- HẰNG SỐ CỬA SỔ (Lấy từ game.h) ---
const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 600;

// --- HẰNG SỐ VẬT LÝ VÀ CHUYỂN ĐỘNG (Lấy từ game.cpp và PhysicsSystem.h) ---
const float PLAYER_SIZE = 50.0f;
const float PLAYER_START_X = 100.0f;
const float PLAYER_START_Y = 500.0f;
const float GROUND_HEIGHT = 50.0f;
const float GROUND_Y = 450.0f;
const float GRAVITY = 3200.0f;
const float JUMP_VELOCITY = -1000.0f;
const float MAX_FALL_VELOCITY = 1500.0f; // Tốc độ rơi tối đa (để tránh lỗi xuyên vật thể)

// --- HẰNG SỐ CHƯỚNG NGẠI VẬT (Lấy từ game.cpp) ---
const float CNV_SIZE = 60.0f;
const float CNV_Y = GROUND_Y - CNV_SIZE;
const float CNV1_START_X = 200.0f;
const float CNV2_START_X = 550.0f;
const float CNV3_START_X = 800.0f;

// Đổi tên MOVE_SPEED thành tốc độ tối đa
const float MAX_MOVE_SPEED = 300.f; 

// Tốc độ mà nhân vật tăng tốc (càng cao càng "nhạy")
const float ACCELERATION = 2000.f;  

// Tốc độ mà nhân vật dừng lại (càng cao càng "trơn trượt" ít)
const float FRICTION = 1500.f;