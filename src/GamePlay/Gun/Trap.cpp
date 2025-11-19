#include "GamePlay/Gun/Trap.h"
#include <iostream>

// Constructor
Trap::Trap(float x, float y, int damageVal)
    : Entity("Trap", "BearTrap", x, y, 100, 0.0f, "assets/Images/Bear_Trap.png", 60.0f, 60.0f, sf::Vector2i(4, 1),
             0.1f),
      damage(damageVal), triggered(false) {
    // Khởi tạo bẫy
}

void Trap::Update(float deltaTime) {
    // Nếu đã kích hoạt thì mới chạy animation
    if (triggered) {
        if (animation) {
            animation->Update(deltaTime);

            // Logic dừng animation ở frame cuối (tùy chọn)
            // Nếu không có logic này, bẫy sẽ kẹp đi kẹp lại liên tục
            // Bạn có thể thêm logic kiểm tra frame hiện tại trong Animation class
        }
    } else {
        // Nếu chưa kích hoạt, có thể reset frame về 0 (nếu cần)
    }

    // Cập nhật vị trí vẽ
    if (animation) {
        animation->setPosition({getX(), getY()});
    }
}

void Trap::Trigger() {
    if (!triggered) {
        triggered = true;
        std::cout << "Bẫy đã sập!" << std::endl;
        // Phát âm thanh ở đây nếu cần
    }
}
// Trong Trap.cpp

void Trap::Reset() {
    triggered = false; // Cho phép gây sát thương lại
    if (animation) {
        animation->Reset(); // Mở cái bẫy ra (dùng hàm vừa viết ở Bước 1)
    }
}