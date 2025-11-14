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


void Dinosaur::ChasePlayer(float playerX, float playerY) {
  // cần sửa
}

// Ghi đè hàm DisplayStatus
void Dinosaur::DisplayStatus() const {
    // Gọi hàm DisplayStatus của class cha (Entity) để hiển thị thông tin chung
    Entity::DisplayStatus(); 

    // Thêm thông tin riêng của Dinosaur (ví dụ: Tầm tấn công)
    cout << " Pham vi tan cong: " << attackRange << endl;
    cout << "-----------------------------" << endl;
}