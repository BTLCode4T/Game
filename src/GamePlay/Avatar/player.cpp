#include "GamePlay/Avatar/player.h" // Đảm bảo đường dẫn include là chính xác
#include <iostream>
#include <cmath> 
#include <algorithm>
#include <SFML/Window/Keyboard.hpp> // Cần thiết cho sf::Keyboard::Key

// Constructor của Player
Player::Player(const string& name, float x, float y,
               int maxHealth, float speed, const string& texturePath,
               int initialLevel)
    : Entity("Player", name, x, y, maxHealth, speed, texturePath),
      level(initialLevel), experience(0), isAlive(true)
{
    std::cout << "Player '" << name << "' da duoc tao (Level " << level << ")." << std::endl;
}

// Hàm xử lý Input (ĐÃ SỬA LỖI 2: Thêm Key::)
void Player::HandleInput() {
    if (!isAlive) return;
    
    float dx = 0.0f;
    float dy = 0.0f;

    // SFML check phím (SỬA LỖI: Thêm Key:: trước tên phím)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dy -= 1.0f; 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dy += 1.0f; 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dx -= 1.0f; 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dx += 1.0f; 

    if (dx != 0.0f || dy != 0.0f) {
        Move(dx, dy); // Gọi hàm Move của Entity
    }
}
int player::GetHealth(){
    return *this.health;
}
// Ghi đè hàm nhận sát thương (ĐÃ SỬA LỖI 3: GetHealth)
void Player::TakeDamage(int amount) {
    if (!isAlive) return; 
    
    // 1. Gọi hàm của class cha để trừ máu thực tế
    Entity::TakeDamage(amount); 

    // 2. Kiểm tra máu sau khi bị trừ.
    // Lỗi GetHealth đã được giải quyết bằng cách đảm bảo nó là public trong Entity.h
    if (GetHealth() <= 0) { 
        Die(); 
    }
}

// Hàm xử lý chết
void Player::Die() {
    if (!isAlive) return;
    
    isAlive = false;
    std::cout << "\n=============================================" << std::endl;
    std::cout << "!!! PLAYER '" << GetName() << "' DA CHET! GAME OVER !!!" << std::endl;
    std::cout << "=============================================\n" << std::endl;
}

// Hàm kiểm tra va chạm (sử dụng khoảng cách giữa 2 tâm)
bool Player::CheckCollision(const Entity& other) const {
    if (!isAlive) return false;
    
    float dx = GetX() - other.GetX();
    float dy = GetY() - other.GetY();
    float distance = std::sqrt(dx * dx + dy * dy);

    const float COLLISION_THRESHOLD = 30.0f; 
    
    return distance < COLLISION_THRESHOLD;
}

// Hàm xử lý va chạm cụ thể với Khủng long: Gây 1 sát thương
void Player::HandleDinosaurCollision(const Entity& other) {
    if (!isAlive) return;

    if (CheckCollision(other)) {
        std::cout << GetName() << " va cham voi " << other.GetName() << " (Khung long)!" << std::endl;
        
        TakeDamage(1); 
    }
}

// Hàm thêm kinh nghiệm
void Player::AddExperience(int amount) {
    if (!isAlive || amount <= 0) return;
    
    experience += amount;
    std::cout << GetName() << " nhan duoc " << amount << " EXP. Tong: " << experience << std::endl;

    while (experience >= 100 * level) {
        experience -= 100 * level; 
        level++;
        std::cout << "*** " << GetName() << " da len CAP DO " << level << "!!! ***" << std::endl;
    }
}

// Ghi đè hàm DisplayStatus
void Player::DisplayStatus() const {
    Entity::DisplayStatus();

    std::cout << "  Cap do: " << level << std::endl;
    std::cout << "  Kinh nghiem: " << experience << std::endl;
    std::cout << "  Trang thai: " << (isAlive ? "**Song**" : "**Chet**") << std::endl;
    std::cout << "-----------------------------" << std::endl;
}