#include "GamePlay/Entity/Entity.h"
#include <algorithm> // Cần thiết cho std::max/min

// Constructor
Entity::Entity(const string& type, const string& name, float x, float y,
    int maxHealth, float speed)
    : type(type), name(name), x(x), y(y), maxHealth(maxHealth), speed(speed) {

    health = maxHealth;
    inventory = "NONE";
    skill = "NONE";
    cout << "Entity '" << name << "' (" << type << ") duoc tao tai (" << x << ", " << y << ")." << endl;
}

// Hàm di chuyển (Move) 
void Entity::Move(float dx, float dy) {
    float moveX = dx * speed;
    float moveY = dy * speed;

    x += moveX;
    y += moveY;

    cout << name << " di chuyen toi (" << x << ", " << y << ") (buoc di: " << moveX << "x, " << moveY << "y)." << endl;
}

// Hàm nhận sát thương
void Entity::TakeDamage(int amount) {
    if (amount < 0) return;
    health -= amount;
    cout << name << " nhan " << amount << " sat thuong.";

    if (health <= 0) {
        health = 0;
        cout << " Da chet!" << endl;
    }
    else {
        cout << " Mau con lai: " << health << "/" << maxHealth << endl;
    }
}

// Hàm hiển thị trạng thái
void Entity::DisplayStatus() const {
    cout << "--- Trang thai " << name << " (" << type << ") ---" << endl;
    cout << "  Mau: " << health << "/" << maxHealth << endl;
    cout << "  Vi tri: (" << x << ", " << y << ")" << endl;
    cout << "  Toc do di chuyen: " << speed << endl;
    cout << "  Kho do (Code): " << inventory << endl;
    cout << "  Ky nang (Code): " << skill << endl;
    cout << "-----------------------------" << endl;
}

// --- Triển khai các Setter còn lại ---

void Entity::SetHealth(int newHealth) {
    health = std::max(0, std::min(newHealth, maxHealth));
    cout << name << " da thay doi mau hien tai thanh: " << health << "/" << maxHealth << endl;
}

void Entity::SetName(const string& newName) {
    name = newName;
    cout << "Ten Entity duoc doi thanh: " << newName << endl;
}

void Entity::SetSpeed(float newSpeed) {
    if (newSpeed < 0) newSpeed = 0;
    speed = newSpeed;
    cout << name << " da thay doi toc do: " << newSpeed << endl;
}