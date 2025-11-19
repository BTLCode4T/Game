#include "GamePlay/Gun/bullet.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Bullet::Bullet(const string &texturePath, float x, float y, float width, float height,
               int damage, sf::Vector2f direction, float speed, float lifetime)
    : Entity("Bullet", "Bullet", x, y, 1, 0.f, // type, name, x, y, maxHealth, speed
             texturePath, width, height, sf::Vector2i(1, 1), 1.f), // 1 frame
      damage(damage), lifetime(lifetime), isDestroyed(false)
{
    // Chuẩn hóa vector hướng (direction) và nhân với tốc độ
    float mag = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (mag != 0) {
        this->velocity = (direction / mag) * speed;
    } else {
        this->velocity = sf::Vector2f(speed, 0); // dự phòng
    }

    // Xoay hình ảnh viên đạn theo hướng bay
    float angleDeg = std::atan2(this->velocity.y, this->velocity.x) * 180.f / M_PI;
    animation->setRotation(sf::degrees(angleDeg));
}

void Bullet::UpdateBullet(float deltaTime) {
    // Di chuyển sprite đạn theo vận tốc
    animation->move(velocity * deltaTime);
}

bool Bullet::IsExpired() const {
    return lifeTimer.getElapsedTime().asSeconds() >= lifetime;
}
// ===============================================
// ĐỊNH NGHĨA HÀM LƯU/TẢI TRẠNG THÁI
// ===============================================

nlohmann::json Bullet::SaveState() const {
    nlohmann::json j;
    
    j["damage"] = damage;
    j["is_destroyed"] = isDestroyed;
    
    if (animation) {
        j["position_x"] = animation->getPosition().x;
        j["position_y"] = animation->getPosition().y;
    } else {
        j["position_x"] = 0.0f;
        j["position_y"] = 0.0f;
    }
    
    j["velocity_x"] = velocity.x;
    j["velocity_y"] = velocity.y;
    
    float remainingTime = lifetime - lifeTimer.getElapsedTime().asSeconds();
    if (remainingTime < 0) remainingTime = 0;
    j["remaining_lifetime"] = remainingTime;
    
    return j;
}

void Bullet::LoadState(const nlohmann::json& j) {
    damage = j.at("damage").get<int>();
    isDestroyed = j.at("is_destroyed").get<bool>();
    
    float x = j.at("position_x").get<float>();
    float y = j.at("position_y").get<float>();
    
    // --- SỬA LỖI SFML 3.0.2: Dùng sf::Vector2f(x, y) ---
    if (animation) {
        animation->setPosition(sf::Vector2f(x, y));
    }
    
    velocity.x = j.at("velocity_x").get<float>();
    velocity.y = j.at("velocity_y").get<float>();
    
    // Cập nhật góc xoay
    if (animation) {
        float angleDeg = std::atan2(velocity.y, velocity.x) * 180.f / M_PI;
        animation->setRotation(sf::degrees(angleDeg));
    }

    lifetime = j.at("remaining_lifetime").get<float>();
    lifeTimer.restart(); 
}