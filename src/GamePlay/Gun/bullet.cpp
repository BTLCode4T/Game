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