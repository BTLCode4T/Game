#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Core/GameLoop/game.h"
#include "Core/Constants.h"


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
    static void Update(sf::Sprite& playerSprite, sf::Vector2f& velocity,
                       float deltaTime, const std::vector<Obstacle>& obstacles,
                       bool& isOnGround);

private:
    // Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục X
    static void HandleXCollision(sf::Sprite& playerSprite, const sf::Vector2f& oldPos,
                                 const std::vector<Obstacle>& obstacles, sf::Vector2f& velocity);

    // Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục Y
    static void HandleYCollision(sf::Sprite& playerSprite, sf::Vector2f& velocity,
                                 const std::vector<Obstacle>& obstacles, bool& isOnGround);
};