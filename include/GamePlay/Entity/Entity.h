#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include <algorithm>        // Dùng cho std::max/min
#include <SFML/Graphics.hpp> // Thư viện đồ họa SFML

class Entity {
private:
    // --- Thuộc tính Chung ---
    std::string type;
    std::string name;
    float x;
    float y;
    int health;
    int maxHealth;
    float speed;
    std::string inventory;
    std::string skill;

    // --- Thuộc tính Đồ họa ---
    sf::Texture texture; // Ảnh nhân vật
    sf::Sprite sprite;   // Sprite để vẽ ảnh

public:
    // Constructor (Cập nhật: thêm đường dẫn ảnh)
    Entity(const std::string& type, const std::string& name,
           float x, float y, int maxHealth, float speed,
           const std::string& texturePath);

    // Destructor ảo
    virtual ~Entity() = default;

    // --- Chức năng chính ---
    void Move(float dx, float dy);
    void TakeDamage(int amount);
    void DisplayStatus() const;

    // --- Hàm MỚI (SFML) ---
    void SetTexture(const std::string& texturePath);
    void Render(sf::RenderWindow& window);

    // --- Setter ---
    void SetHealth(int newHealth);
    void SetName(const std::string& newName);
    void SetSpeed(float newSpeed);

    void AddSkill(const std::string& skillCode) {
        skill = skillCode;
        std::cout << name << " hoc duoc ky nang moi: " << skillCode << std::endl;
    }

    void AddInventory(const std::string& itemCode) {
        inventory = itemCode;
        std::cout << name << " nhat duoc: " << itemCode << std::endl;
    }

    // --- Getter ---
    float GetX() const { return x; }
    float GetY() const { return y; }
    std::string GetName() const { return name; }
};

#endif // ENTITY_H
