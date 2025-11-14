#include "Utils/Entity.h"

#include <SFML/Graphics.hpp>
#include <algorithm>

Entity::Entity(const string &type, const string &name, float x, float y, int maxHealth, float speed,
               const string &texturePath)
    : type(type), name(name), x(x), y(y), maxHealth(maxHealth), speed(speed),

      sprite(texture) {
    health = maxHealth;
    inventory = "NONE";
    skill = "NONE";

    sprite = createSprite(texture, "assets/Images/a.png", PLAYER_SIZE, PLAYER_SIZE, PLAYER_START_X, PLAYER_START_Y);
}

void Entity::jump(bool &isOnGround, const int MAX_JUMPS, int &jumpsRemaining) {
    velocity.y = JUMP_VELOCITY;
    jumpsRemaining--;
    isOnGround = false;
}

// Hàm di chuyển (Move) (CẬP NHẬT: Di chuyển cả sprite)
void Entity::Move(bool leftPressed, bool rightPressed, float deltaTime, const std::vector<Obstacle> &obs,
                  bool &isOnGround, const int MAX_JUMPS, int &jumpsRemaining) {
    if (leftPressed) {
        // Tăng tốc sang trái (velocity.x giảm)
        velocity.x -= ACCELERATION * deltaTime;
    } else if (rightPressed) {
        // Tăng tốc sang phải (velocity.x tăng)
        velocity.x += ACCELERATION * deltaTime;
    } else {
        // 3. Không nhấn phím -> Áp dụng ma sát (Giảm tốc)
        if (velocity.x > 0.f) {
            // Đang di chuyển sang phải -> ma sát đẩy sang trái
            velocity.x -= FRICTION * deltaTime;
            // Kẹp lại, tránh bị "trôi" ngược
            if (velocity.x < 0.f)
                velocity.x = 0.f;
        } else if (velocity.x < 0.f) {
            // Đang di chuyển sang trái -> ma sát đẩy sang phải
            velocity.x += FRICTION * deltaTime;
            // Kẹp lại
            if (velocity.x > 0.f)
                velocity.x = 0.f;
        }
    }
    // 4. Giới hạn tốc độ tối đa
    if (velocity.x > MAX_MOVE_SPEED) {
        velocity.x = MAX_MOVE_SPEED;
    } else if (velocity.x < -MAX_MOVE_SPEED) {
        velocity.x = -MAX_MOVE_SPEED;
    }
    x += velocity.x * deltaTime;
    y += velocity.y * deltaTime;

    PhysicsSystem::Update(sprite, velocity, deltaTime, obs, isOnGround);
    // Reset số lần nhảy nếu chạm đất
    if (isOnGround) {
        jumpsRemaining = MAX_JUMPS;
    }
    // Giới hạn trong cửa sổ
    const sf::Vector2f pos = sprite.getPosition();
    if (pos.x < 0.f)
        sprite.setPosition({0.f, pos.y});
    sf::FloatRect playerBounds = sprite.getGlobalBounds();
    if (pos.x + playerBounds.size.x > WINDOW_WIDTH)
        sprite.setPosition({WINDOW_WIDTH - playerBounds.size.x, pos.y});

    // CẬP NHẬT: Đồng bộ vị trí của sprite với vị trí logic (x, y)
    sprite.setPosition(sf::Vector2f(x, y));
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

// vẽ
void Entity::Render(sf::RenderWindow &window) {
    sprite.setPosition(sf::Vector2f(x, y));
    window.draw(sprite);
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
