#include "GamePlay/Avatar/player.h" // Đảm bảo đường dẫn include là chính xác
#include <SFML/Window/Keyboard.hpp> // Cần thiết cho sf::Keyboard::Key
#include <algorithm>
#include <cmath>
#include <iostream>
#include "GamePlay/Gun/Trap.h"
// Constructor của PlayerManager
PlayerManager::PlayerManager(const std::string &name, float x, float y, int maxHealth, float speed,
                             const std::string &texturePath, float width, float height, sf::Vector2i frameNum,
                             float frameTime)
    : Entity("PlayerManager", name, x, y, maxHealth, speed, texturePath, width, height, frameNum, frameTime),
   isAlive(true),
   currentGun(nullptr), damageCooldownTime(2.0f)
{
    
  
}

// Hàm xử lý Input
void PlayerManager::HandleInputPlayerManager(
    bool leftPressed, bool rightPressed, float deltaTime, const std::vector<Obstacle> &obs,
    const std::vector<std::unique_ptr<Dinosaur>> &dinosaurs, 
    const int MAX_JUMPS) {

    this->Move(leftPressed, rightPressed, deltaTime, obs, dinosaurs, MAX_JUMPS);
}
int PlayerManager::GetHealth() {
    return Entity::getHealth();
}
// Ghi đè hàm nhận sát thương (ĐÃ SỬA LỖI 3: GetHealth)
void PlayerManager::TakeDamage(int amount) {
    if (damageCooldownClock.getElapsedTime().asSeconds() < damageCooldownTime) {
        return; // Không làm gì cả, thoát hàm
    }

    if (!isAlive)
        return;
    int currentHealth = GetHealth();
    int newHealth = currentHealth - amount;
    Entity::SetHealth(newHealth);
    // 1. Gọi hàm của class cha để trừ máu thực tế
    //Entity::TakeDamage(amount);
    damageCooldownClock.restart();
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

    // Mở rộng sang trái phải mỗi bên 5 pixel
    // Điều này đảm bảo dù PhysicsSystem đã đẩy ra, ta vẫn phát hiện được va chạm
    playerBounds.position.x -= 5.0f;
    playerBounds.size.x += 10.0f;

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

            this->setVelocity(-500.0f, -300.0f);
            this->setIsOnGround(false);

            // 4. Reset đồng hồ để bắt đầu đếm ngược lại từ đầu
            damageCooldownClock.restart();
        }
    }
}
void PlayerManager::EquipGun(std::unique_ptr<Gun> gun) {
    currentGun = std::move(gun);
}
// Ghi đè hàm DisplayStatus
void PlayerManager::DisplayStatus() const {
    Entity::DisplayStatus();

  
    std::cout << "Trang thai: " << (isAlive ? "**Song**" : "**Chet**") << std::endl;
    std::cout << "-----------------------------" << std::endl;
}
void PlayerManager::Render(sf::RenderWindow &window) {
    // 1. Mặc định là sẽ vẽ
    bool isVisible = true;

    // 2. Kiểm tra xem có đang trong thời gian "Bất tử" (Cooldown) không
    // Nếu chưa hết thời gian cooldown thì xử lý chớp tắt
    float elapsed = damageCooldownClock.getElapsedTime().asSeconds();
    if (isAlive && elapsed < damageCooldownTime) {
        // Logic chớp tắt:
        // Chia thời gian cho 0.1s (tốc độ chớp).
        // Nếu kết quả là số lẻ thì ẩn đi (isVisible = false), số chẵn thì hiện.
        // Ví dụ: 0.1s -> ẩn, 0.2s -> hiện, 0.3s -> ẩn...
        if (static_cast<int>(elapsed / 0.1f) % 2 != 0) {
            isVisible = false;
        }
    }

    // 3. Chỉ vẽ nếu isVisible == true
    if (isVisible) {
        Entity::Render(window); // Vẽ nhân vật (Animation)

        if (currentGun) {
            currentGun->Render(window); // Vẽ súng
        }
    }
}


void PlayerManager::HandleTrapCollision(std::vector<std::unique_ptr<Trap>> &traps) {
    if (!isAlive)
        return;

    for (auto &trap : traps) {
        if (!trap->IsTriggered()) {

            // ⚠️ SỬA LỖI GẠCH CHÂN ĐỎ: Dùng findIntersection().has_value()
            if (this->getBounds().findIntersection(trap->getBounds()).has_value()) {

                // Logic này là đúng và đã được kiểm tra:
                trap->Trigger();
                this->TakeDamage(trap->GetDamage());

                // Áp dụng đẩy lùi
                this->setVelocity(this->getPushV(), -400.0f);
            }
        }
    }
}


nlohmann::json PlayerManager::SaveState() const {
    nlohmann::json j;

    j["name"] = GetName();
    j["current_health"] = Entity::getHealth(); 
    j["max_health"] = Entity::getMaxHealth();
    j["speed"] = GetSpeed();
    
    if (animation) {
        j["position_x"] = animation->getPosition().x;
        j["position_y"] = animation->getPosition().y;
    } else {
        j["position_x"] = 0.0f;
        j["position_y"] = 0.0f;
    }
    
    j["is_alive"] = isAlive;
    j["damage_cooldown_time"] = damageCooldownTime;

    if (currentGun) {
        j["has_gun"] = true;
        j["gun_name"] = currentGun->GetName();
    } else {
        j["has_gun"] = false;
    }

    return j;
}

void PlayerManager::LoadState(const nlohmann::json& j) {
    // 1. Tải vị trí
    float x = j.at("position_x").get<float>();
    float y = j.at("position_y").get<float>();
    if (animation) {
        animation->setPosition(sf::Vector2f(x, y));
    }

    // 2. Tải máu
    int savedHealth = j.at("current_health").get<int>();
    int currentHP = Entity::getHealth();
    
    if (currentHP > savedHealth) {
        Entity::TakeDamage(currentHP - savedHealth);
    } 

    // 3. Tải trạng thái sống
    isAlive = j.at("is_alive").get<bool>();
    if (j.contains("damage_cooldown_time")) {
        damageCooldownTime = j.at("damage_cooldown_time").get<float>();
    }

    // 4. Tải súng
    bool hasGun = j.at("has_gun").get<bool>();
    if (hasGun) {
        std::string gunName = j.at("gun_name").get<std::string>();
        std::cout << "[LoadState] Player loaded with gun name: " << gunName << "\n";
    }
}