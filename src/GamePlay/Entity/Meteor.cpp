#include "GamePlay/Entity/Meteor.h"
#include <cmath> // Để dùng std::sqrt

Meteor::Meteor(sf::Texture &texture, sf::Vector2f startPos, sf::Vector2f targetPos, float speed, int damage,
               sf::Vector2i frameNum, float frameTime)
    : damage(damage), isDestroyed(false) {
    animation = std::make_unique<Animation>(texture, frameNum, frameTime);

    // Đặt tâm về giữa animation để xoay cho đẹp
    sf::IntRect rect = animation->getTextureRect();
    animation->setOrigin({(float)rect.size.x / 2.f, (float)rect.size.y / 2.f});

    // Đặt vị trí ban đầu
    animation->setPosition(startPos);

    // Đặt kích thước cho thiên thạch (ví dụ: 80x80 pixels)
    // Bạn có thể chỉnh số 80.0f này tùy ý
    animation->setScale({100.0f / rect.size.x, 100.0f / rect.size.y});

    // Tính toán hướng bay xéo từ startPos đến targetPos
    sf::Vector2f direction = targetPos - startPos;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length; // Chuẩn hóa vector
    }
    velocity = direction * speed;

    // Xoay thiên thạch hướng về phía mục tiêu (tùy chọn)
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
    animation->setRotation(sf::degrees(0)); // +90 để ảnh đứng thẳng nếu cần
}

void Meteor::Update(float deltaTime) {
    if (isDestroyed)
        return;

    animation->move(velocity * deltaTime);
    animation->Update(deltaTime); // Cập nhật frame animation

    // Đánh dấu hủy nếu bay ra khỏi màn hình
    sf::Vector2f currentPos = animation->getPosition();
    if (currentPos.y > 1000 || currentPos.x < -100 || currentPos.x > 2000) { // Giả định WINDOW_HEIGHT là 768
        isDestroyed = true;
    }
}

void Meteor::Render(sf::RenderWindow &window) {
    if (isDestroyed)
        return;
    window.draw(*animation);
}

const sf::FloatRect Meteor::GetGlobalBounds() const {
    return animation->getGlobalBounds();
}