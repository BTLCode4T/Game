#include "GamePlay/Physics/PhysicsSystem.h"
#include "Utils/Entity.h"
#include <iostream>

// Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục X
void PhysicsSystem::HandleXCollision(sf::Sprite &playerSprite, const sf::Vector2f &oldPos,
                                     const std::vector<Obstacle> &obstacles, Entity &entity) {

    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
    sf::Vector2f vel = entity.getVelocity();
    for (const auto &obs : obstacles) {
        if (entity.GetType() == "Dinosaur") {
            continue; // Nếu là Khủng long, bỏ qua vật cản này
        }
        // Va chạm X: kiểm tra giao nhau sau khi di chuyển
        if (playerBounds.findIntersection(obs.sprite->getGlobalBounds())) {
            playerSprite.setPosition({oldPos.x + entity.getPushV(),
                                      playerSprite.getPosition().y}); // Nếu va chạm X, đặt lại vị trí X về vị trí cũ
            vel.x = 0;                                                // Dừng di chuyển ngang
            entity.setVelocity(vel.x, vel.y);
            return;
        }
    }
    // đẩy người chơi theo chiều X
    float pushX = entity.getPushV();
    if (pushX != 0.f) {
        playerSprite.move({pushX, 0.f});
    }
}

// Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục Y
void PhysicsSystem::HandleYCollision(sf::Sprite &playerSprite, const std::vector<Obstacle> &obstacles, Entity &entity) {
    entity.setIsOnGround(false); // Luôn reset trạng thái chạm đất trước khi kiểm tra

    sf::Vector2f vel = entity.getVelocity();

    // 1. Va chạm Y với TẤT CẢ CNV
    for (const auto &obs : obstacles) {
        sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
        sf::FloatRect cnvBounds = obs.sprite->getGlobalBounds();
        if (playerBounds.findIntersection(cnvBounds)) {
            if (entity.GetType() == "Dinosaur") {
                continue; // Nếu là Khủng long, bỏ qua vật cản này
            }
            if (vel.y > 0) // Đang rơi (va chạm từ trên xuống)
            {
                // Đặt nhân vật đứng trên chướng ngại vật
                playerSprite.setPosition({playerSprite.getPosition().x, cnvBounds.position.y - playerBounds.size.y});

                vel.y = 0;
                entity.setVelocity(vel.x, vel.y);
                entity.setIsOnGround(true); // Coi như đang "trên mặt đất"
                return;
            } else if (vel.y < 0) // Đang nhảy (va chạm từ dưới lên - đụng đầu)
            {
                // Đặt nhân vật bên dưới chướng ngại vật
                playerSprite.setPosition({playerSprite.getPosition().x, cnvBounds.position.y + cnvBounds.size.y});
                vel.y = 0; // Ngừng lực nhảy
                entity.setVelocity(vel.x, vel.y);
            }
        }
    }



    // 2. Va chạm Y với mặt đất (cuối cùng)
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
    if (playerBounds.position.y + playerBounds.size.y >= GROUND_Y) {
        playerSprite.setPosition({playerSprite.getPosition().x, GROUND_Y - playerBounds.size.y});

        vel.y = 0;
        entity.setVelocity(vel.x, vel.y);

        entity.setIsOnGround(true);
    }
}

// Hàm cập nhật vật lý chính (Public)
void PhysicsSystem::Update(sf::Sprite &playerSprite, float deltaTime, const std::vector<Obstacle> &obstacles,
                           Entity &entity) {
    // 1. Lưu vị trí cũ
    sf::Vector2f oldPos = playerSprite.getPosition();

    // 2. Cập nhật vận tốc Y từ trọng lực
    if (!entity.getIsOnGround()) {
        float temp = entity.getVelocity().y;
        temp += GRAVITY * deltaTime;
        entity.setVelocityY(temp);
    }

    if (entity.getVelocity().y > MAX_FALL_VELOCITY) { // Sử dụng hằng số từ Constants.h
        entity.setVelocityY(MAX_FALL_VELOCITY);
    }

    // 3. Di chuyển theo trục X
    playerSprite.move({entity.getVelocity().x * deltaTime, 0.f});

    // 4. Kiểm tra và xử lý va chạm X
    HandleXCollision(playerSprite, oldPos, obstacles, entity);

    // 5. Di chuyển theo trục Y
    playerSprite.move({0.f, entity.getVelocity().y * deltaTime});

    // 6. Kiểm tra và xử lý va chạm Y (Trọng lực, CNV, Mặt đất)
    HandleYCollision(playerSprite, obstacles, entity);
}