#ifndef ENTITY_H
#define ENTITY_H

#include "Utils/Constants.h"

#include "Core/Input/Input.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include <SFML/Graphics.hpp> // MỚI: Thêm thư viện đồ họa SFML
#include <algorithm>         // Dùng cho std::max/min
#include <iostream>
#include <string>
#include "GamePlay/UI/UI.h"

using namespace std;

class Entity {
  private:
   
    sf::Vector2f velocity;  // Vận tốc di chuyển của nhân vật
    // --- Thuộc tính Chung ---
    string type;
    string name;
    float x;
    float y;
    int health;
    int maxHealth;
    float speed;
    string inventory;
    string skill;

  

  public:
    // --- Thuộc hính Đồ họa (MỚI) ---
    sf::Texture texture; // Biến "ảnh" (lưu trữ ảnh)
    sf::Sprite sprite;   // Biến "nhân vật" (dùng để vẽ)
   InputManager inputManager; // hàm input
    // Constructor (Cập nhật: thêm đường dẫn ảnh)
    Entity(const string &type, const string &name, float x, float y, int maxHealth, float speed,
           const string &texturePath); // CẬP NHẬT

    // Destructor ảo
    virtual ~Entity() = default;

    // --- Hàm chức năng ---
    void Move(bool leftPressed,  bool rightPressed, float deltaTime,const std::vector<Obstacle>& obs, bool &isOnGround,  const int MAX_JUMPS, int &jumpsRemaining); // Sẽ được cập nhật để di chuyển sprite
    void jump( bool &isOnGround,  const int MAX_JUMPS, int &jumpsRemaining);
    virtual void TakeDamage(int amount);
    /**
     * @brief Hàm "edit" ảnh - Tải một ảnh mới cho Entity.
     * @param texturePath Đường dẫn tới file ảnh (ví dụ: "assets/player.png")
     */
    void SetTexture(const string &texturePath); // MỚI

    /**
     * @brief Hàm "hiện con nhân vật ra" - Vẽ nhân vật lên cửa sổ game.
     * @param window Cửa sổ RenderWindow mà bạn muốn vẽ lên.
     */
    void Render(sf::RenderWindow &window); // MỚI

    // --- Hàm cập nhật & Setters ---
    void AddSkill(const string &skillCode) {
        skill = skillCode;
        cout << name << " hoc duoc ky nang moi: " << skillCode << endl;
    }
    void AddInventory(const string &itemCode) {

        inventory = itemCode;
        cout << name << " nhat duoc: " << itemCode << endl;
    }

    // Khai báo
    void SetHealth(int newHealth);
    void SetName(const string &newName);
    void SetSpeed(float newSpeed);

    // --- Getters ---
    float GetX() const {
        return x;
    }
    float GetY() const {
        return y;
    }
    string GetName() const {
        return name;
    }

    int getHealth() const{
        return health;
    }

    // Hàm hiển thị trạng thái
    virtual void DisplayStatus() const;
};

#endif // ENTITY_H