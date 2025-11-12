#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include <algorithm> // Dùng cho std::max/min
#include <SFML/Graphics.hpp> // MỚI: Thêm thư viện đồ họa SFML

using namespace std;

class Entity {
private:
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

    // --- Thuộc hính Đồ họa (MỚI) ---
    sf::Texture texture; // Biến "ảnh" (lưu trữ ảnh)
    sf::Sprite sprite;   // Biến "nhân vật" (dùng để vẽ)

public:
    // Constructor (Cập nhật: thêm đường dẫn ảnh)
    Entity(const string& type, const string& name, float x, float y,
           int maxHealth, float speed, const string& texturePath); // CẬP NHẬT


    // Destructor ảo
    virtual ~Entity() = default;

    // --- Hàm chức năng ---
    void Move(float dx, float dy); // Sẽ được cập nhật để di chuyển sprite
    void TakeDamage(int amount);


    // --- Hàm chức năng MỚI (Render và SetTexture) ---

    /**
     * @brief Hàm "edit" ảnh - Tải một ảnh mới cho Entity.
     * @param texturePath Đường dẫn tới file ảnh (ví dụ: "assets/player.png")
     */
    void SetTexture(const string& texturePath); // MỚI

    /**
     * @brief Hàm "hiện con nhân vật ra" - Vẽ nhân vật lên cửa sổ game.
     * @param window Cửa sổ RenderWindow mà bạn muốn vẽ lên.
     */
    void Render(sf::RenderWindow& window); // MỚI

    // --- Hàm cập nhật & Setters ---
    void AddSkill(const string& skillCode) {
        skill = skillCode;
        cout << name << " hoc duoc ky nang moi: " << skillCode << endl;
    }
    void AddInventory(const string& itemCode) {

        inventory = itemCode;
        cout << name << " nhat duoc: " << itemCode << endl;
    }

    // Khai báo
    void SetHealth(int newHealth);
    void SetName(const string& newName);
    void SetSpeed(float newSpeed);

    // --- Getters ---
    float GetX() const { return x; }
    float GetY() const { return y; }
    string GetName() const { return name; }

    // Hàm hiển thị trạng thái
    virtual void DisplayStatus() const;
};

#endif // ENTITY_H