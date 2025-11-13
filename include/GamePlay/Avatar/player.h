#ifndef PLAYER_H
#define PLAYER_H

#include "GamePlay/Entity/Entity.h" 
#include <SFML/Window.hpp>          

class Player : public Entity {
private:
    int level;
    int experience;
    bool isAlive; 

public:
    // Constructor
    Player(const string& name, float x, float y,
           int maxHealth, float speed, const string& texturePath,
           int initialLevel = 1);

    virtual ~Player() = default;

    // --- Hàm chức năng chính ---
    void HandleInput();
    bool CheckCollision(const Entity& other) const;
    void HandleDinosaurCollision(const Entity& other); 
    void Die();

    // --- Ghi đè hàm của Entity ---
    // Lỗi OVERRIDE được giải quyết bằng cách thêm 'virtual' vào Entity::TakeDamage
    void TakeDamage(int amount) override; 

    void DisplayStatus() const override;

    // --- Getters & Setters riêng ---
    int GetLevel() const { return level; }
    bool IsAlive() const { return isAlive; }
    void AddExperience(int amount);
    int GetHealth();
};

#endif // PLAYER_H