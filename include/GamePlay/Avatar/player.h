#ifndef PlayerManager_H
#define PlayerManager_H

#include "Utils/Entity.h"
#include <SFML/Window.hpp>
#include "GamePlay/Gun/gun.h"

class PlayerManager : public Entity {
  private:
    bool isAlive;
    std::unique_ptr<Gun> currentGun;

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
    void TakeDamage(int amount) override;
    void Die();
    bool IsAlive() const {
        return isAlive;
    }
    void EquipGun(std::unique_ptr<Gun> gun);
    Gun* GetGun() const { return currentGun.get(); }
    // AddExperience( vì chưa cần)
    int GetHealth();
    void DisplayStatus() const override;
    void Render(sf::RenderWindow &window) override;
};

#endif // PlayerManager_H
