#ifndef PlayerManager_H
#define PlayerManager_H

#include "GamePlay/Gun/gun.h"
#include "Utils/Entity.h"
#include <SFML/Window.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <memory>
class Trap;
class PlayerManager : public Entity {
  private:
    bool isAlive;
    std::unique_ptr<Gun> currentGun;
    sf::Clock damageCooldownClock;
    float damageCooldownTime;

  public:
    // Constructor
    PlayerManager(const string &name, float x, float y, int maxHealth, float speed, const string &texturePath,
                  float width, float height, sf::Vector2i frameNum, float frameTime);
    virtual ~PlayerManager() = default;

    void HandleInputPlayerManager(bool leftPressed, bool rightPressed, float deltaTime,
                                  const std::vector<Obstacle> &obs,
                                  const std::vector<std::unique_ptr<Dinosaur>> &dinosaurs, // <--- THÊM THAM SỐ NÀY
                                  const int MAX_JUMPS);

    bool CheckCollision(const Entity &other) const;    
    void HandleDinosaurCollision(const Entity &other); 
    void HandleTrapCollision(std::vector<std::unique_ptr<Trap>> &traps);

    void TakeDamage(int amount) override;
    void Die();
    bool IsAlive() const {
        return isAlive;
    }

    void setIsAlive(bool alive) {
        isAlive = alive;
    }

    void EquipGun(std::unique_ptr<Gun> gun);
    Gun *GetGun() const {
        return currentGun.get();
    }

    int GetHealth();
    int GetMaxHealth() const {
        return Entity::getMaxHealth();
    }
    bool IsImmune() const;
    void DisplayStatus() const override;
    void Render(sf::RenderWindow &window) override;

};

#endif // PlayerManager_H
