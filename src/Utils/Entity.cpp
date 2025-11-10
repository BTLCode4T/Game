#include "GamePlay/Entity/Entity.h"
#include <SFML/Graphics.hpp> // MỚI: Thêm thư viện
#include <algorithm>         // Cần thiết cho std::max/min

// Constructor (CẬP NHẬT: Thêm texturePath)
Entity::Entity(const string &type, const string &name, float x, float y, int maxHealth, float speed,
               const string &texturePath) // CẬP NHẬT
    : type(type), name(name), x(x), y(y), maxHealth(maxHealth), speed(speed) {

    health = maxHealth;
    inventory = "NONE";
    skill = "NONE";

    // MỚI: Tải ảnh và thiết lập Sprite
    SetTexture(texturePath);  // Gọi hàm "edit" để tải ảnh
    sprite.setPosition(x, y); // Đặt vị trí ban đầu cho sprite

    cout << "Entity '" << name << "' (" << type << ") duoc tao tai (" << x << ", " << y << ")." << endl;
}

// Hàm di chuyển (Move) (CẬP NHẬT: Di chuyển cả sprite)
void Entity::Move(float dx, float dy) {
    float moveX = dx * speed;
    float moveY = dy * speed;

    x += moveX;
    y += moveY;

    // CẬP NHẬT: Đồng bộ vị trí của sprite với vị trí logic (x, y)
    sprite.setPosition(x, y);

    cout << name << " di chuyen toi (" << x << ", " << y << ") (buoc di: " << moveX << "x, " << moveY << "y)." << endl;
}

// MỚI: Định nghĩa hàm SetTexture (hàm "edit" ảnh)
void Entity::SetTexture(const string &texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        cerr << "Loi: Khong tai duoc file ' " << texturePath << "' cho Entity '" << name << "'" << endl;
    } else {
        sprite.setTexture(texture);
        cout << name << " da tai anh: " << texturePath << endl;

        // Tùy chọn: Đặt tâm của sprite vào giữa
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
}

// MỚI: Định nghĩa hàm Render (hàm "hiện nhân vật")
void Entity::Render(sf::RenderWindow &window) {
    window.draw(sprite); // Đơn giản là vẽ sprite lên cửa sổ
}

// Hàm nhận sát thương
void Entity::TakeDamage(int amount) {
    // ... (Giữ nguyên code của bạn)
    if (amount < 0)
        return;
    health -= amount;
    cout << name << " nhan " << amount << " sat thuong.";

    if (health <= 0) {
        health = 0;
        cout << " Da chet!" << endl;
    } else {
        cout << " Mau con lai: " << health << "/" << maxHealth << endl;
    }
}

// Hàm hiển thị trạng thái
void Entity::DisplayStatus() const {
    // ... (Giữ nguyên code của bạn)
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

void Entity::SetName(const string &newName) {
    
    name = newName;
    cout << "Ten Entity duoc doi thanh: " << newName << endl;
}

void Entity::SetSpeed(float newSpeed) {
    
    if (newSpeed < 0)
        newSpeed = 0;
    speed = newSpeed;
    cout << name << " da thay doi toc do: " << newSpeed << endl;
}