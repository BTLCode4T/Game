#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/Entity/Dinosaur.h" // <--- Đừng quên include này
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

        // Kiểm tra giao nhau
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

// Hàm cập nhật vật lý chính (Public) - ĐÃ SỬA: Thêm tham số dinosaurs
void PhysicsSystem::Update(sf::Sprite &playerSprite, float deltaTime, const std::vector<Obstacle> &obstacles,
                           const std::vector<std::unique_ptr<Dinosaur>> &dinosaurs, // <--- THAM SỐ QUAN TRỌNG
                           Entity &entity) {
    // 1. Lưu vị trí cũ
    sf::Vector2f oldPos = playerSprite.getPosition();

    // 2. Cập nhật vận tốc Y từ trọng lực
    if (!entity.getIsOnGround()) {
        float temp = entity.getVelocity().y;
        temp += GRAVITY * deltaTime;
        entity.setVelocityY(temp);
    }

    if (entity.getVelocity().y > MAX_FALL_VELOCITY) {
        entity.setVelocityY(MAX_FALL_VELOCITY);
    }

    // 3. Di chuyển theo trục X
    playerSprite.move({entity.getVelocity().x * deltaTime, 0.f});

    // 4. Kiểm tra và xử lý va chạm X (Với vật cản tĩnh)
    HandleXCollision(playerSprite, oldPos, obstacles, entity);

    // --- 4.1 XỬ LÝ VA CHẠM VỚI KHỦNG LONG (CHẶN KHÔNG CHO ĐI XUYÊN) ---
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
    for (const auto &dino : dinosaurs) {
        sf::FloatRect dinoBounds = dino->getBounds();
        auto intersection = playerBounds.findIntersection(dinoBounds);

        if (intersection) {
            sf::FloatRect overlap = *intersection;
            // Chỉ xử lý va chạm ngang
            if (overlap.size.x < overlap.size.y) {
                // Nếu người chơi đang ở bên trái khủng long
                if (playerBounds.position.x < dinoBounds.position.x) {
                    // SỬA: Trừ đi 0.1f để tạo khoảng cách cực nhỏ, tránh bị dính
                    float wallPos = dinoBounds.position.x - playerBounds.size.x - 0.1f;
                    playerSprite.setPosition({wallPos, playerSprite.getPosition().y});

                    // Chỉ chặn vận tốc nếu đang cố đi vào khủng long (velocity.x > 0)
                    // Nếu người chơi bấm nút lùi (velocity.x < 0) thì vẫn cho đi
                    if (entity.getVelocity().x > 0) {
                        entity.setVelocity(0.f, entity.getVelocity().y);
                    }
                } else {
                    // Bên phải
                    float wallPos = dinoBounds.position.x + dinoBounds.size.x + 0.1f;
                    playerSprite.setPosition({wallPos, playerSprite.getPosition().y});
                    if (entity.getVelocity().x < 0) {
                        entity.setVelocity(0.f, entity.getVelocity().y);
                    }
                }
            }
        }
    }
    // -----------------------------------------------------------------------

    // 5. Di chuyển theo trục Y
    playerSprite.move({0.f, entity.getVelocity().y * deltaTime});

    // 6. Kiểm tra và xử lý va chạm Y (Trọng lực, CNV, Mặt đất)
    HandleYCollision(playerSprite, obstacles, entity);

} // Đã đóng ngoặc hàm Update đúng