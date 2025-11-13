#include "GamePlay/Entity/Dinosaur.h"
#include <iostream>
#include <cmath> // Dùng cho các phép tính toán học (ví dụ: sqrt, atan2)

// Constructor của Dinosaur
Dinosaur::Dinosaur(const string& name, float x, float y,
                   int maxHealth, float speed, const string& texturePath)
    // Gọi Constructor của class Entity (class cha)
    // Thiết lập type cố định là "Dinosaur"
    : Entity("Dinosaur", name, x, y, maxHealth, speed, texturePath),
      attackRange(50.0f) // Thiết lập tầm tấn công mặc định
{
    cout << "Dinosaur '" << name << "' da duoc tao tai (" << x << ", " << y << ")." << endl;
}

// Hàm AI (Ví dụ: Đuổi theo người chơi)
void Dinosaur::ChasePlayer(float playerX, float playerY) {
    // Tính toán khoảng cách theo trục X và Y
    float dx = playerX - GetX();
    float dy = playerY - GetY();

    // Tính toán độ lớn vector khoảng cách (sử dụng định lý Pitago)
    float distance = std::sqrt(dx * dx + dy * dy);

    // Nếu khoảng cách lớn hơn 1.0f (tránh rung lắc khi quá gần)
    if (distance > 1.0f) {
        // Chuẩn hóa vector di chuyển thành vector đơn vị
        // Chia dx và dy cho khoảng cách để được hướng di chuyển
        float moveX = dx / distance; 
        float moveY = dy / distance;
        
        // Gọi hàm Move của Entity. Khủng long sẽ di chuyển theo hướng của người chơi
        Move(moveX, moveY);
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