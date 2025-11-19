#pragma once

#include "Utils/Constants.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>

class Dinosaur;

class Entity;

struct Obstacle {
    // SỬA ĐỔI: Sử dụng unique_ptr cho cả Sprite VÀ Texture
    std::unique_ptr<sf::Sprite> sprite;
    std::unique_ptr<sf::Texture> texture; 

    Obstacle(const std::string &texturePath, float width, float height, float x, float y) {
        // TẢI TEXTURE BẰNG CON TRỎ THÔNG MINH
        texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(texturePath)) {
            std::cerr << "Loi: Khong the tai texture: " << texturePath << std::endl;
        }
        // Gán Sprite cho Texture thông qua con trỏ
        sprite = std::make_unique<sf::Sprite>(*texture); // <--- LƯU Ý: Dùng *texture

        // Căn chỉnh kích thước (Set size and position)
        sf::FloatRect bounds = sprite->getLocalBounds();

        sprite->setScale(sf::Vector2f(width / bounds.size.x, height / bounds.size.y));

        sprite->setPosition({x, y});
    }

    // Mặc định: sao chép (copy) bị cấm, chỉ cho phép di chuyển (move)
    Obstacle(const Obstacle &) = delete;
    Obstacle &operator=(const Obstacle &) = delete;

    // Cho phép di chuyển (Cần thiết cho std::vector::emplace_back)
    // KHÔNG CẦN định nghĩa lại hàm tạo di chuyển vì std::unique_ptr tự xử lý di chuyển
    Obstacle(Obstacle &&other) noexcept = default; // <--- THAY ĐỔI Ở ĐÂY (Hoặc xóa)

    Obstacle &operator=(Obstacle &&other) noexcept = default; // <--- THAY ĐỔI Ở ĐÂY (Hoặc xóa)
};

// Định nghĩa các hằng số cần thiết (Có thể lấy từ game.cpp)
// *LƯU Ý*: Nên khai báo các hằng số này trong một file riêng (ví dụ: Constants.h)
// để tránh trùng lặp và dễ quản lý, nhưng tôi tạm khai báo ở đây để đơn giản hóa.
// const float GROUND_Y = 450.0f;
// const float GRAVITY = 3200.0f;
// const float JUMP_VELOCITY = -1000.0f;
// const float MOVE_SPEED = 200.0f;

// Class PhysicsSystem sẽ đảm nhận việc cập nhật vị trí và xử lý va chạm
class PhysicsSystem {
  public:
    // Hàm cập nhật vật lý chính
    // Nó nhận vào:
    // 1. Đối tượng người chơi (sf::Sprite)
    // 2. Vận tốc hiện tại (sf::Vector2f)
    // 3. Thời gian đã trôi qua (float deltaTime)
    // 4. Danh sách các chướng ngại vật (std::vector<Obstacle>)
    // 5. Biến cờ kiểm tra đang trên mặt đất (bool& isOnGround)
    static void Update(sf::Sprite &playerSprite, float deltaTime, const std::vector<Obstacle> &obstacles,
                       const std::vector<std::unique_ptr<Dinosaur>> &dinosaurs,
                       Entity &entity);

  private:
    // Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục X
    static void HandleXCollision(sf::Sprite &playerSprite, const sf::Vector2f &oldPos,
                                 const std::vector<Obstacle> &obstacles, Entity &entity);

    // Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục Y
    static void HandleYCollision(sf::Sprite &playerSprite, const std::vector<Obstacle> &obstacles, Entity &entity);
};