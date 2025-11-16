#ifndef BULLET_H
#define BULLET_H

#include "Utils/Entity.h"

class Bullet : public Entity {
  private:
    int damage;
    bool isDestroyed;
    sf::Vector2f velocity;
    float lifetime; // Đạn sẽ biến mất sau X giây
    sf::Clock lifeTimer; // Đồng hồ đếm thời gian sống

  public:
    Bullet(const string &texturePath, float x, float y, float width, float height,
           int damage, sf::Vector2f direction, float speed, float lifetime = 5.0f);

    virtual ~Bullet() = default;

    /**
     * @brief Cập nhật vị trí đạn bay
     */
    void UpdateBullet(float deltaTime);

    /**
     * @brief Kiểm tra xem đạn đã hết hạn chưa
     */
    bool IsExpired() const;

    int GetDamage() const { return damage; }
    void Destroy() { isDestroyed = true; }
    bool IsDestroyed() const { return isDestroyed; }
};

#endif // BULLET_H