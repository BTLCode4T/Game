#ifndef METEOR_H
#define METEOR_H

#include "GamePlay/UI/Animation.h" // Để dùng Animation cho thiên thạch
#include "Utils/Entity.h"

class Meteor {
  private:
    std::unique_ptr<Animation> animation;
    sf::Vector2f velocity; // Vận tốc rơi (bao gồm hướng xéo)
    int damage;            // Sát thương gây ra cho người chơi
    bool isDestroyed;      // Đánh dấu để xóa

  public:
    Meteor(sf::Texture &texture, sf::Vector2f startPos, sf::Vector2f targetPos, float speed, int damage,
           sf::Vector2i frameNum, float frameTime);

    void Update(float deltaTime);
    void Render(sf::RenderWindow &window);

    const sf::FloatRect GetGlobalBounds() const;
    bool IsDestroyed() const {
        return isDestroyed;
    }
    void Destroy() {
        isDestroyed = true;
    }
    int GetDamage() const {
        return damage;
    }
};

#endif // METEOR_H