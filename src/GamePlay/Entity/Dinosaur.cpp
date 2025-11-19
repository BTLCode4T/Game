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
    // --- CẤU HÌNH THANH MÁU (INIT) ---
    // 1. Kích thước thanh máu 
    sf::Vector2f barSize(100.f, 10.f);

    // 2. Thiết lập thanh nền (Màu đỏ)
    healthBarBg.setSize(barSize);
    healthBarBg.setFillColor(sf::Color::Red);
    healthBarBg.setOutlineColor(sf::Color::Black);
    healthBarBg.setOutlineThickness(1.0f);

    // 3. Thiết lập thanh hiện tại (Màu xanh lá)
    healthBarCurrent.setSize(barSize);
    healthBarCurrent.setFillColor(sf::Color::Green);
}

void Dinosaur::UpdateHealthBar() {
    if (!animation)
        return;

    // 1. Tính phần trăm máu còn lại
    // Ép kiểu float để chia lấy thập phân
    float healthPercent = static_cast<float>(getHealth()) / static_cast<float>(getMaxHealth());

    // Đảm bảo không âm
    if (healthPercent < 0)
        healthPercent = 0;

    // 2. Cập nhật độ dài thanh màu xanh
    // Lấy kích thước gốc của thanh nền nhân với phần trăm
    healthBarCurrent.setSize({healthBarBg.getSize().x * healthPercent, healthBarBg.getSize().y});

    // 3. Cập nhật vị trí (Luôn bay trên đầu khủng long)
    sf::FloatRect dinoBounds = animation->getGlobalBounds();

    // SFML 3.0 dùng .position.x, SFML 2.x dùng .left
    // Tính toán để thanh máu nằm giữa theo trục X
    float barX = dinoBounds.position.x + (dinoBounds.size.x / 2.0f) - (healthBarBg.getSize().x / 2.0f);
    float barY = dinoBounds.position.y - 20.0f; // Cách đỉnh đầu 20px

    healthBarBg.setPosition({barX, barY});
    healthBarCurrent.setPosition({barX, barY});
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

void Dinosaur::Render(sf::RenderWindow &window) {
    // 1. Vẽ hình ảnh khủng long trước (Gọi hàm cha)
    Entity::Render(window);

    // 2. Cập nhật vị trí thanh máu theo vị trí mới của khủng long
    UpdateHealthBar();

    // 3. Vẽ thanh máu (chỉ vẽ nếu máu > 0)
    if (getHealth() > 0) {
        window.draw(healthBarBg);      // Vẽ nền đỏ dưới
        window.draw(healthBarCurrent); // Vẽ máu xanh đè lên
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