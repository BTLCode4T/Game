#ifndef PlayerManager_H
#define PlayerManager_H

#include "Utils/Entity.h"
#include <SFML/Window.hpp>

class PlayerManager : public Entity {
  private:
    bool isAlive;

  public:
    // Constructor
    PlayerManager(const string &name, float x, float y, int maxHealth, float speed, const string &texturePath);
    virtual ~PlayerManager() = default;

    void HandleInputPlayerManager(bool leftPressed, bool rightPressed, float deltaTime,
                                  const std::vector<Obstacle> &obs, const int MAX_JUMPS);

    bool CheckCollision(const Entity &other) const;    // va chạm
    void HandleDinosaurCollision(const Entity &other); // chạm vào khủng long
    // xóa GetLevel() vì chưa cần
    void TakeDamage(int amount) override;
    void Die();
    bool IsAlive() const {
        return isAlive;
    }
    // AddExperience( vì chưa cần)
    int GetHealth();
    void DisplayStatus() const override;
};

#endif // PlayerManager_H
