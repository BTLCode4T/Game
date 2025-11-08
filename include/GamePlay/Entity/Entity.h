#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include <algorithm> // Dùng cho std::max/min

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

public:
    // Constructor
    Entity(const string& type, const string& name, float x, float y,
        int maxHealth, float speed);

    // Destructor ảo
    virtual ~Entity() = default;

    // --- Hàm chức năng ---
    void Move(float dx, float dy);
    void TakeDamage(int amount);

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