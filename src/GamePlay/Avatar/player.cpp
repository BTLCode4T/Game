#include "GamePlay/Avatar/player.h" // Đảm bảo đường dẫn include là chính xác
#include <SFML/Window/Keyboard.hpp> // Cần thiết cho sf::Keyboard::Key
#include <algorithm>
#include <cmath>
#include <iostream>

// Constructor của PlayerManager
PlayerManager::PlayerManager(const std::string &name, float x, float y, int maxHealth, float speed, const std::string &texturePath)
    : Entity("PlayerManager", name, x, y, maxHealth, speed, texturePath),
   isAlive(true)
{
  
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

    float dx = GetX() - other.GetX();
    float dy = GetY() - other.GetY();
    float distance = std::sqrt(dx * dx + dy * dy);

    const float COLLISION_THRESHOLD = 30.0f;

    return distance < COLLISION_THRESHOLD;
}

// Hàm xử lý va chạm cụ thể với Khủng long: Gây 1 sát thương
void PlayerManager::HandleDinosaurCollision(const Entity &other) {
    if (!isAlive)
        return;

    if (CheckCollision(other)) {
        std::cout << GetName() << " va cham voi " << other.GetName() << " (Khung long)!" << std::endl;

        TakeDamage(1);
    }
}

// Ghi đè hàm DisplayStatus
void PlayerManager::DisplayStatus() const {
    Entity::DisplayStatus();

  
    std::cout << "  Trang thai: " << (isAlive ? "**Song**" : "**Chet**") << std::endl;
    std::cout << "-----------------------------" << std::endl;
}