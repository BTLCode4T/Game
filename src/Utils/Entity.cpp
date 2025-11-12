#include "GamePlay/Entity/Entity.h"
#include <SFML/Graphics.hpp> // Thư viện đồ họa SFML
#include <algorithm>         // Cần thiết cho std::max/min

// Constructor (Đã sửa lỗi khởi tạo sf::Sprite)
Entity::Entity(const string &type, const string &name, float x, float y, int maxHealth, float speed,
               const string &texturePath)
    : type(type), name(name), x(x), y(y), maxHealth(maxHealth), speed(speed), 
      // KHẮC PHỤC LỖI 1: Khởi tạo sprite bằng cách liên kết nó với texture
      sprite(texture) 
{ 
    health = maxHealth;
    inventory = "NONE";
    skill = "NONE";

    // MỚI: Tải ảnh và thiết lập Sprite
    SetTexture(texturePath);  // Gọi hàm "edit" để tải ảnh (sẽ tự động thiết lập texture cho sprite)
    // Dùng sf::Vector2f cho setPosition
    sprite.setPosition(sf::Vector2f(x, y)); 

    cout << "Entity '" << name << "' (" << type << ") duoc tao tai (" << x << ", " << y << ")." << endl;
}

// Hàm di chuyển (Move) (CẬP NHẬT: Di chuyển cả sprite)
void Entity::Move(float dx, float dy) {
    float moveX = dx * speed;
    float moveY = dy * speed;

    x += moveX;
    y += moveY;

    // CẬP NHẬT: Đồng bộ vị trí của sprite với vị trí logic (x, y)
    sprite.setPosition(sf::Vector2f(x, y));

    cout << name << " di chuyen toi (" << x << ", " << y << ") (buoc di: " << moveX << "x, " << moveY << "y)." << endl;
}

// MỚI: Định nghĩa hàm SetTexture (hàm "edit" ảnh)
void Entity::SetTexture(const string &texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        cerr << "Loi: Khong tai duoc file ' " << texturePath << "' cho Entity '" << name << "'" << endl;
    } else {
        // setTextur/e sẽ liên kết texture đã tải với sprite
        sprite.setTexture(texture); 
        cout << name << " da tai anh: " << texturePath << endl;

        // Tùy chọn: Đặt tâm của sprite vào giữa
        sf::FloatRect bounds = sprite.getLocalBounds();
        
        // setOrigin phải dùng sf::Vector2f trong SFML 3.x
        sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f)); 
    }
}

// MỚI: Định nghĩa hàm Render (hàm "hiện nhân vật")
void Entity::Render(sf::RenderWindow &window) {
    window.draw(sprite); // Đơn giản là vẽ sprite lên cửa sổ
}

// Hàm nhận sát thương
void Entity::TakeDamage(int amount) {
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