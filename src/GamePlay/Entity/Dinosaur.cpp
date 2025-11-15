#include "GamePlay/Entity/Dinosaur.h"
#include <iostream>
#include <cmath> // Dùng cho các phép tính toán học (ví dụ: sqrt, atan2)

// Constructor của Dinosaur
Dinosaur::Dinosaur(const string &name, float x, float y, int maxHealth, float speed, const string &texturePath,
                   float width, float height, sf::Vector2i frameNum, float frameTime)

    // 3. TÌM DÒNG GỌI ENTITY NÀY:
    // Gọi Constructor của class Entity (class cha)
    // Thiết lập type cố định là "Dinosaur"
    : Entity("Dinosaur", name, x, y, maxHealth, speed, texturePath, width, height, frameNum, frameTime),
      attackRange(50.0f) // Thiết lập tầm tấn công mặc định
{
    cout << "Dinosaur '" << name << "' da duoc tao tai (" << x << ", " << y << ")." << endl;
}


void Dinosaur::ChasePlayer(float playerX, float playerY) {
    // Lấy vị trí X hiện tại của khủng long (từ sprite)
    float currentX = animation->getPosition().x;

    // Tính khoảng cách theo chiều ngang
    float dx = playerX - currentX;
    float distance = std::abs(dx);

    // Lấy tốc độ của Entity (từ hàm GetSpeed() ta vừa thêm)
    float moveSpeed = GetSpeed();

    // Lấy vận tốc Y hiện tại (để giữ nguyên trọng lực)
    float currentVelY = getVelocity().y;

    // AI đơn giản:
    if (distance > attackRange) {
        // 1. NẾU ở ngoài tầm tấn công -> Đuổi theo
        float directionX = (dx > 0) ? 1.0f : -1.0f; // Quyết định hướng chạy

        // Đặt vận tốc: X là tốc độ di chuyển, Y là vận tốc rơi/nhảy hiện tại
        setVelocity(directionX * moveSpeed, currentVelY);

    } else {
        // 2. NẾU ở trong tầm tấn công -> Dừng lại (để chuẩn bị tấn công)
        setVelocity(0, currentVelY); // Dừng vận tốc X
    }
}

// Ghi đè hàm DisplayStatus
void Dinosaur::DisplayStatus() const {
    // Gọi hàm DisplayStatus của class cha (Entity) để hiển thị thông tin chung
    Entity::DisplayStatus(); 

    // Thêm thông tin riêng của Dinosaur (ví dụ: Tầm tấn công)
    cout << " Pham vi tan cong: " << attackRange << endl;
    cout << "-----------------------------" << endl;
}