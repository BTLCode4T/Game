#include "GamePlay/Avatar/player.h" // Đảm bảo đường dẫn include là chính xác
#include <SFML/Window/Keyboard.hpp> // Cần thiết cho sf::Keyboard::Key
#include <algorithm>
#include <cmath>
#include <iostream>

// Constructor của PlayerManager
PlayerManager::PlayerManager(const std::string &name, float x, float y, int maxHealth, float speed,
                             const std::string &texturePath, float width, float height, sf::Vector2i frameNum,
                             float frameTime)
    : Entity("PlayerManager", name, x, y, maxHealth, speed, texturePath, width, height, frameNum, frameTime),
      isAlive(true), damageCooldownClock(), damageCooldownTime(1.5f) {
    
}

// Hàm xử lý Input
void PlayerManager::HandleInputPlayerManager(bool leftPressed, bool rightPressed, float deltaTime,
                                             const std::vector<Obstacle> &obs, const int MAX_JUMPS) {
    this->Move(leftPressed, rightPressed, deltaTime, obs, MAX_JUMPS);
}
int PlayerManager::GetHealth() {
    return Entity::getHealth();
}
// Ghi đè hàm nhận sát thương (ĐÃ SỬA LỖI 3: GetHealth)
void PlayerManager::TakeDamage(int amount) {
    if (!isAlive)
        return;

    // 1. Gọi hàm của class cha để trừ máu thực tế
    Entity::TakeDamage(amount);

    // 2. Kiểm tra máu sau khi bị trừ.
    // Lỗi GetHealth đã được giải quyết bằng cách đảm bảo nó là public trong Entity.h
    if (GetHealth() <= 0) {
        Die();
    }
}

// Hàm xử lý chết
void PlayerManager::Die() {
    if (!isAlive)
        return;

    isAlive = false;
    std::cout << "\n=============================================" << std::endl;
    std::cout << "!!! PlayerManager '" << GetName() << "' DA CHET! GAME OVER !!!" << std::endl;
    std::cout << "=============================================\n" << std::endl;
}

// Hàm kiểm tra va chạm (sử dụng khoảng cách giữa 2 tâm)
bool PlayerManager::CheckCollision(const Entity &other) const {
    if (!isAlive)
        return false;

     // 1. Lấy khung hình chữ nhật của bản thân
    sf::FloatRect playerBounds = this->getBounds();

    // 2. Lấy khung hình chữ nhật của đối tượng kia
    sf::FloatRect otherBounds = other.getBounds();

    // 3. Kiểm tra xem 2 hình chữ nhật có giao nhau không
    // findIntersection trả về std::optional, dùng .has_value() để kiểm tra có giao nhau không
    return playerBounds.findIntersection(otherBounds).has_value();
}

// Hàm xử lý va chạm cụ thể với Khủng long: Gây 1 sát thương
void PlayerManager::HandleDinosaurCollision(const Entity &other) {
    if (!isAlive)
        return;

    // 1. Kiểm tra Cooldown TRƯỚC (để tiết kiệm hiệu năng)
    if (damageCooldownClock.getElapsedTime().asSeconds() > damageCooldownTime) {
        // 2. Nếu hết thời gian chờ -> Mới kiểm tra va chạm vật lý
        if (CheckCollision(other)) {
            std::cout << GetName() << " va cham voi " << other.GetName() << " (Khung long)!" << std::endl;
            // 3. Trừ máu
            TakeDamage(1);
            // 4. Reset đồng hồ để bắt đầu đếm ngược lại từ đầu
            damageCooldownClock.restart();
        }
    }
}

// Ghi đè hàm DisplayStatus
void PlayerManager::DisplayStatus() const {
    Entity::DisplayStatus();

  
    std::cout << "  Trang thai: " << (isAlive ? "**Song**" : "**Chet**") << std::endl;
    std::cout << "-----------------------------" << std::endl;
}
bool PlayerManager::IsImmune() const {
    return damageCooldownClock.getElapsedTime().asSeconds() <= damageCooldownTime;
}
void PlayerManager::Render(sf::RenderWindow &window) {
    bool drawPlayer = true; // Mặc định là vẽ

    // Dùng hàm vừa tạo ở Bước 1
    if (IsImmune()) {
        // Lấy thời gian đã trôi qua trong 1.5s miễn nhiễm
        float immuneTime = damageCooldownClock.getElapsedTime().asSeconds();

        // fmod: phép chia lấy dư
        // 0.2f là tổng thời gian 1 chu kỳ (0.1s sáng, 0.1s tối)
        float blinkCycle = std::fmod(immuneTime, 0.2f);

        // Nếu thời gian trong chu kỳ > 0.1f (nửa sau) thì TẮT VẼ
        if (blinkCycle > 0.1f) {
            drawPlayer = false;
        }
    }

    // 'animation' là biến public của class Entity (cha)
    if (drawPlayer && animation) {
        window.draw(*animation);
    }
}