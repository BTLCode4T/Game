#ifndef PlayerManager_H
#define PlayerManager_H

#include "Utils/Entity.h"
#include <SFML/Window.hpp>
#include <SFML/System/Clock.hpp>

class PlayerManager : public Entity {
  private:
    bool isAlive;
    sf::Clock damageCooldownClock; 
    float damageCooldownTime;
  public:
    // Constructor
    PlayerManager(const string &name, float x, float y, int maxHealth, float speed, const string &texturePath,
                  float width, float height, sf::Vector2i frameNum, float frameTime);
    virtual ~PlayerManager() = default;

    void HandleInputPlayerManager(bool leftPressed, bool rightPressed, float deltaTime,
                                  const std::vector<Obstacle> &obs, const int MAX_JUMPS);

    bool CheckCollision(const Entity &other) const;    // va chạm
    void HandleDinosaurCollision(const Entity &other); // chạm vào khủng long
    // xóa GetLevel() vì chưa cần
    void Render(sf::RenderWindow &window);
    void TakeDamage(int amount) override;
    void Die();
    bool IsAlive() const {
        return isAlive;
    }
    // AddExperience( vì chưa cần)
    int GetHealth();
    int GetMaxHealth() const {
        return Entity::getMaxHealth();
    }
    bool IsImmune() const;
    void DisplayStatus() const override;
};

#endif // PlayerManager_H
