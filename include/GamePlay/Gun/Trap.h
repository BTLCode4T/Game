#pragma once
#include "Utils/Entity.h"

class Trap : public Entity {
  private:
    int damage;     // Sát thương gây ra
    bool triggered; // Kiểm tra bẫy đã sập chưa

  public:
    Trap(float x, float y, int damageVal);
    void Reset();
    void Update(float deltaTime);
    bool IsTriggered() const {
        return triggered;
    }

    // Hàm kích hoạt bẫy (khi người chơi dẫm vào)
    void Trigger();

    int GetDamage() const {
        return damage;
    }
};