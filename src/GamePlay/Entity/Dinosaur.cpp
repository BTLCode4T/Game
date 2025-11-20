#include "GamePlay/Entity/Dinosaur.h"
#include <iostream>
#include <cmath> // Dùng cho các phép tính toán học (ví dụ: sqrt, atan2)

// Constructor của Dinosaur
Dinosaur::Dinosaur(const string &name, float x, float y, int maxHealth, float speed, const string &texturePath,
                   float width, float height, sf::Vector2i frameNum, float frameTime)

    // 3. TÌM DÒNG GỌI ENTITY NÀY:
    // Gọi Constructor của class Entity (class cha)
    // Thiết lập type cố định là "Dinosaur"
    : Entity("Dinosaur", name, x, y, maxHealth, speed, texturePath, width, height, frameNum, frameTime),
      attackRange(2000.0f), // Tầm bắn (xa hơn một chút để bắn)
      shootCooldown(2.0f), // Bắn mỗi 2 giây
      timeSinceLastShot(0.0f)
{
    cout << "Dinosaur '" << name << "' da duoc tao tai (" << x << ", " << y << ")." << endl;
   
    // --- LOAD ẢNH CẦU LỬA ---
    if (!fireballTexture.loadFromFile("assets/Images/CauLua.png")) {
        // Nếu bạn để file cùng thư mục exe thì chỉ cần "CauLua.png"
        std::cerr << "Loi: Khong the tai 'CauLua.png'" << std::endl;
    }
    // --- CẤU HÌNH THANH MÁU (INIT) ---
    // 1. Kích thước thanh máu 
    sf::Vector2f barSize(100.f, 10.f);

    // 2. Thiết lập thanh nền (Màu đỏ)
    healthBarBg.setSize(barSize);
    healthBarBg.setFillColor(sf::Color::Red);
    healthBarBg.setOutlineColor(sf::Color::Black);
    healthBarBg.setOutlineThickness(1.0f);

    // 3. Thiết lập thanh hiện tại (Màu xanh lá)
    healthBarCurrent.setSize(barSize);
    healthBarCurrent.setFillColor(sf::Color::Green);
}
// Trong file Dinosaur.cpp

void Dinosaur::UpdateAttack(float deltaTime, sf::Vector2f playerPos) {
    timeSinceLastShot += deltaTime;

    sf::Vector2f myPos = animation->getPosition();
    float myX = myPos.x;
    float myY = myPos.y;

    // 1. Logic Bắn đạn
    float dist = std::sqrt(std::pow(playerPos.x - myX, 2) + std::pow(playerPos.y - myY, 2));

    if (dist < attackRange + 200.0f && timeSinceLastShot >= shootCooldown) {

        Fireball fireball;

        // --- KHỞI TẠO ANIMATION ---
        // sf::Vector2i(4, 1): Nghĩa là ảnh có 4 cột (4 frames ngang) và 1 hàng.
        // 0.1f: Thời gian chuyển giữa các frame (tốc độ chớp tắt của lửa).
        fireball.animation = std::make_unique<Animation>(fireballTexture, sf::Vector2i(4, 1), 0.1f);

        // Chỉnh tâm về giữa để xoay cho đẹp
        // Lưu ý: Animation kế thừa Sprite nên dùng getLocalBounds bình thường
        // Tuy nhiên, bounds của Animation là kích thước 1 frame, không phải cả ảnh to -> Rất chuẩn.
        sf::IntRect rect = fireball.animation->getTextureRect();
        fireball.animation->setOrigin({(float)rect.size.x / 2.f, (float)rect.size.y / 2.f});

        // Chỉnh kích thước (Ví dụ muốn nó to cỡ 40x40)
        fireball.animation->setScale({40.f / rect.size.x, 40.f / rect.size.y});

        // Vị trí xuất phát
        fireball.animation->setPosition({myX + 350.f, myY + 130.f});

        // Tính hướng bắn
        sf::Vector2f direction = playerPos - sf::Vector2f(myX, myY);
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length != 0) {
            direction /= length;
            fireball.velocity = direction * 500.0f;

            // Xoay đầu đạn về phía người chơi
            float angleVal = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
            fireball.animation->setRotation(sf::degrees(angleVal));

            fireballs.push_back(std::move(fireball));
            timeSinceLastShot = 0.0f;

            // Audio::Get().Play("fire_shoot");
        }
    }
    // 2. Cập nhật đạn
    for (auto &fb : fireballs) {
        // Di chuyển
        fb.animation->move(fb.velocity * deltaTime);

        // --- QUAN TRỌNG: CẬP NHẬT ANIMATION ---
        // Dòng này sẽ làm cho cầu lửa chuyển từ hình 1 -> 2 -> 3 -> 4 liên tục
        fb.animation->Update(deltaTime);

        // (Tùy chọn) Vừa bay vừa xoay vòng tròn nếu bạn thích lửa cuộn tròn
        // fb.animation->rotate(sf::degrees(360.0f * deltaTime));

        // Hủy đạn nếu bay xa
        sf::Vector2f pos = fb.animation->getPosition();
        if (pos.x < 0 || pos.x > 3000 || pos.y > 1000 || pos.y < -500) {
            fb.isDestroyed = true;
        }
    }

    // Xóa đạn hỏng
    fireballs.erase(
        std::remove_if(fireballs.begin(), fireballs.end(), [](const Fireball &fb) { return fb.isDestroyed; }),
        fireballs.end());
}

void Dinosaur::UpdateHealthBar() {
    if (!animation)
        return;

    // 1. Tính phần trăm máu còn lại
    // Ép kiểu float để chia lấy thập phân
    float healthPercent = static_cast<float>(getHealth()) / static_cast<float>(getMaxHealth());

    // Đảm bảo không âm
    if (healthPercent < 0)
        healthPercent = 0;

    // 2. Cập nhật độ dài thanh màu xanh
    // Lấy kích thước gốc của thanh nền nhân với phần trăm
    healthBarCurrent.setSize({healthBarBg.getSize().x * healthPercent, healthBarBg.getSize().y});

    // 3. Cập nhật vị trí (Luôn bay trên đầu khủng long)
    sf::FloatRect dinoBounds = animation->getGlobalBounds();

    // SFML 3.0 dùng .position.x, SFML 2.x dùng .left
    // Tính toán để thanh máu nằm giữa theo trục X
    float barX = dinoBounds.position.x + (dinoBounds.size.x / 2.0f) - (healthBarBg.getSize().x / 2.0f);
    float barY = dinoBounds.position.y - 20.0f; // Cách đỉnh đầu 20px

    healthBarBg.setPosition({barX, barY});
    healthBarCurrent.setPosition({barX, barY});
}

void Dinosaur::ChasePlayer(float playerX, float playerY) {
    // Lấy vị trí X hiện tại của khủng long (từ sprite)
    float currentX = animation->getPosition().x;

    // Tính khoảng cách theo chiều ngang
    float dx = playerX - currentX;
    float distance = std::abs(dx);

    // Lấy tốc độ của Entity (từ hàm GetSpeed() ta vừa thêm)
    float moveSpeed = GetSpeed();

    // Lấy vận tốc Y hiện tại (để giữ nguyên trọng lực)
    float currentVelY = getVelocity().y;

    // AI đơn giản:
    if (distance > attackRange) {
        // 1. NẾU ở ngoài tầm tấn công -> Đuổi theo
        float directionX = (dx > 0) ? 1.0f : -1.0f; // Quyết định hướng chạy

        // Đặt vận tốc: X là tốc độ di chuyển, Y là vận tốc rơi/nhảy hiện tại
        setVelocity(directionX * moveSpeed, currentVelY);

    } else {
        // 2. NẾU ở trong tầm tấn công -> Dừng lại (để chuẩn bị tấn công)
        setVelocity(0, currentVelY); // Dừng vận tốc X
    }
}

void Dinosaur::Render(sf::RenderWindow &window) {
    // 1. Vẽ hình ảnh khủng long trước (Gọi hàm cha)
    Entity::Render(window);

    // 2. Cập nhật vị trí thanh máu theo vị trí mới của khủng long
    UpdateHealthBar();

    // 3. Vẽ thanh máu (chỉ vẽ nếu máu > 0)
    if (getHealth() > 0) {
        window.draw(healthBarBg);      // Vẽ nền đỏ dưới
        window.draw(healthBarCurrent); // Vẽ máu xanh đè lên
    }
    // 4. Vẽ Đạn cầu lửa
    for (const auto &fb : fireballs) {
        window.draw(*fb.animation); 
    }
}

// Ghi đè hàm DisplayStatus
void Dinosaur::DisplayStatus() const {
    // Gọi hàm DisplayStatus của class cha (Entity) để hiển thị thông tin chung
    Entity::DisplayStatus(); 

    // Thêm thông tin riêng của Dinosaur (ví dụ: Tầm tấn công)
    cout << " Pham vi tan cong: " << attackRange << endl;
    cout << "-----------------------------" << endl;
}