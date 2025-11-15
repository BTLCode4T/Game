#include "Utils/Entity.h"

#include <SFML/Graphics.hpp>
#include <algorithm>

// ĐỊNH NGHĨA MỚI (9 tham số)
Entity::Entity(const string &type, const string &name, float x, float y, int maxHealth, float speed,
               const string &texturePath, float width, float height, sf::Vector2i frameNum,
               float frameTime) // <-- Thêm width, height ở đây
    : type(type), name(name), x(x), y(y), maxHealth(maxHealth), health(maxHealth), speed(speed), inventory("NONE"),
      skill("NONE"), texture(), animation(nullptr), pushV(0) {

    if (!texture.loadFromFile(texturePath)) {
        // DÒNG MỚI:
        std::cerr << "Loi: Khong the tai '" << texturePath << "'" << std::endl;
    }

    // 2. Tạo đối tượng Animation bằng con trỏ unique_ptr
    animation = std::make_unique<Animation>(texture, frameNum, frameTime);

    // 3. Đặt Kích thước (Scale) và Vị trí (Position)
    // Lấy kích thước của 1 khung hình (mà Animation đã cắt)
    sf::Vector2i frameSize = animation->getFrameSize(); // Lấy kích thước 1 frame (chính xác)

    // Chúng ta phải ép kiểu (cast) sang float để phép chia chính xác
    float scaleX = width / static_cast<float>(frameSize.x);
    float scaleY = height / static_cast<float>(frameSize.y);

    // Áp dụng scale và vị trí
    animation->setScale(sf::Vector2f(scaleX, scaleY));
    animation->setPosition({x, y});
}

void Entity::jump(const int MAX_JUMPS) {
    velocity.y = JUMP_VELOCITY;
    jumpsRemaining--;
    isOnGround = false;
}


// Hàm di chuyển (Move) (CẬP NHẬT: Di chuyển cả sprite)
void Entity::Move(bool leftPressed, bool rightPressed, float deltaTime, const std::vector<Obstacle> &obs,
                  const int MAX_JUMPS) {
   
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
    
    PhysicsSystem::Update(*animation, deltaTime, obs, *this);
    // Reset số lần nhảy nếu chạm đất
    if (isOnGround) {
        jumpsRemaining = MAX_JUMPS;
    }
    // Giới hạn trong cửa sổ
    const sf::Vector2f pos = animation->getPosition();
    if (pos.x < 0.f)
        animation->setPosition({0.f, pos.y});
    sf::FloatRect playerBounds = animation->getGlobalBounds();
    if (pos.x + playerBounds.size.x > WINDOW_WIDTH)
        animation->setPosition({WINDOW_WIDTH - playerBounds.size.x, pos.y});

}

// MỚI: Định nghĩa hàm SetTexture (hàm "edit" ảnh)
void Entity::SetTexture(const string &texturePath) {
    //if (!texture.loadFromFile(texturePath)) {
    //    cerr << "Loi: Khong tai duoc file ' " << texturePath << "' cho Entity '" << name << "'" << endl;
    //} else {
    //    // setTextur/e sẽ liên kết texture đã tải với sprite
    //    sprite.setTexture(texture);
    //    cout << name << " da tai anh: " << texturePath << endl;

    //    // Tùy chọn: Đặt tâm của sprite vào giữa
    //    sf::FloatRect bounds = sprite.getLocalBounds();

    //    // setOrigin phải dùng sf::Vector2f trong SFML 3.x
    //    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));
    //}
}

// vẽ
void Entity::Render(sf::RenderWindow &window) {
    window.draw(*animation);
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
