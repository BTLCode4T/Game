// D:\NgocTien\Desktop\Y2\DSA-OOP\Game\include\GamePlay\Physics\PhysicsSystem.h
#include "GamePlay/Physics/PhysicsSystem.h"
#include <iostream>

// Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục X
void PhysicsSystem::HandleXCollision(sf::Sprite &playerSprite, const sf::Vector2f &oldPos,
                                     const std::vector<Obstacle> &obstacles, sf::Vector2f &velocity)
{
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

    for (const auto &obs : obstacles)
    {
        // Va chạm X: kiểm tra giao nhau sau khi di chuyển
        if (playerBounds.findIntersection(obs.sprite->getGlobalBounds()))
        {
            // Nếu va chạm X, đặt lại vị trí X về vị trí cũ
            playerSprite.setPosition({oldPos.x, playerSprite.getPosition().y});
            velocity.x = 0; // Dừng di chuyển ngang
            return;         // Thoát ngay khi tìm thấy va chạm (chỉ va chạm với một CNV là đủ)
        }
    }
}

// Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục Y
void PhysicsSystem::HandleYCollision(sf::Sprite &playerSprite, sf::Vector2f &velocity,
                                     const std::vector<Obstacle> &obstacles, bool &isOnGround)
{

    isOnGround = false; // Luôn reset trạng thái chạm đất trước khi kiểm tra

    // 1. Va chạm Y với TẤT CẢ CNV
    for (const auto &obs : obstacles)
    {
        sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
        sf::FloatRect cnvBounds = obs.sprite->getGlobalBounds();

        if (playerBounds.findIntersection(cnvBounds))
        {
            if (velocity.y > 0) // Đang rơi (va chạm từ trên xuống)
            {
                // Đặt nhân vật đứng trên chướng ngại vật
                playerSprite.setPosition(
                    {playerSprite.getPosition().x, cnvBounds.position.y - playerBounds.size.y});
                velocity.y = 0;
                isOnGround = true; // Coi như đang "trên mặt đất"
                // ⚠️ THAY ĐỔI QUAN TRỌNG: NGẮT NGAY SAU KHI ĐẶT VỊ TRÍ ĐỨNG VỮNG TRÊN 1 CNV
                // Nếu ta đã tìm thấy một bề mặt để đứng, ta không cần kiểm tra va chạm rơi với bề mặt khác
                // (bao gồm cả mặt đất).
                return;
            }
            else if (velocity.y < 0) // Đang nhảy (va chạm từ dưới lên - đụng đầu)
            {
                // Đặt nhân vật bên dưới chướng ngại vật
                playerSprite.setPosition(
                    {playerSprite.getPosition().x, cnvBounds.position.y + cnvBounds.size.y});
                velocity.y = 0; // Ngừng lực nhảy
                // Lưu ý: KHÔNG return ở đây, vì sau va chạm đụng đầu, người chơi vẫn phải kiểm tra va chạm rơi
                // (trọng lực sẽ đẩy xuống ở frame tiếp theo).
            }
        }
    }

    // 2. Va chạm Y với mặt đất (cuối cùng)
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
    if (playerBounds.position.y + playerBounds.size.y >= GROUND_Y)
    {
        playerSprite.setPosition({playerSprite.getPosition().x, GROUND_Y - playerBounds.size.y});
        velocity.y = 0;
        isOnGround = true;
    }
    // Ở đây không cần return vì đây là bước cuối cùng.
}
// Hàm cập nhật vật lý chính (Public)
void PhysicsSystem::Update(sf::Sprite &playerSprite, sf::Vector2f &velocity,
                           float deltaTime, const std::vector<Obstacle> &obstacles,
                           bool &isOnGround)
{

    // 1. Lưu vị trí cũ
    sf::Vector2f oldPos = playerSprite.getPosition();

    // 2. Cập nhật vận tốc Y từ trọng lực
    if (!isOnGround)
        velocity.y += GRAVITY * deltaTime;

    // *SỬA LỖI*: Giới hạn tốc độ rơi tối đa (Terminal Velocity)
    if (velocity.y > MAX_FALL_VELOCITY)
    { // Sử dụng hằng số từ Constants.h
        velocity.y = MAX_FALL_VELOCITY;
    }

    // 3. Di chuyển theo trục X
    playerSprite.move({velocity.x * deltaTime, 0.f});

    // 4. Kiểm tra và xử lý va chạm X
    HandleXCollision(playerSprite, oldPos, obstacles, velocity);

    // 5. Di chuyển theo trục Y
    playerSprite.move({0.f, velocity.y * deltaTime});

    // 6. Kiểm tra và xử lý va chạm Y (Trọng lực, CNV, Mặt đất)
    HandleYCollision(playerSprite, velocity, obstacles, isOnGround);

    // *LƯU Ý*: Logic giới hạn trong cửa sổ (giới hạn ngang) nên được giữ trong game.cpp
    // vì nó là logic của game loop, không phải logic vật lý thuần túy.
}