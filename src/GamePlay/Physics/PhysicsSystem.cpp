#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/Entity/Dinosaur.h" // <--- Đừng quên include này
#include "Utils/Entity.h"
#include <iostream>

// Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục X
void PhysicsSystem::HandleXCollision(sf::Sprite &playerSprite, const sf::Vector2f &oldPos,
                                     const std::vector<Obstacle> &obstacles, Entity &entity) {

    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

    // --- KHẮC PHỤC LỖI KẸT (QUAN TRỌNG NHẤT) ---
    // Vì ở hàm Y ta cho nhân vật lún xuống 15px (visualSink), nên ở đây ta phải
    // "nhấc" cái hitbox lên đúng 15px + thêm 2px an toàn nữa để nó thoát khỏi mặt đất.
    float visualSink = 15.0f;

    sf::FloatRect walkHitbox = playerBounds;
    walkHitbox.position.y -= (visualSink + 2.0f); // Nhấc lên cao hơn mặt đất một chút
    walkHitbox.size.y -= 10.0f;                   // Thu nhỏ chiều cao thêm xíu để đỡ vướng đầu
    // -------------------------------------------

    sf::Vector2f vel = entity.getVelocity();
    for (const auto &obs : obstacles) {
        if (entity.GetType() == "Dinosaur")
            continue;

        // Dùng walkHitbox đã được "nhấc lên" để kiểm tra va chạm
        // Lúc này nó sẽ chỉ đụng Tường thôi, không đụng Đất dưới chân nữa
        if (walkHitbox.findIntersection(obs.sprite->getGlobalBounds())) {

            playerSprite.setPosition({oldPos.x + entity.getPushV(), playerSprite.getPosition().y});
            vel.x = 0;
            entity.setVelocity(vel.x, vel.y);
            return;
        }
    }

    float pushX = entity.getPushV();
    if (pushX != 0.f) {
        playerSprite.move({pushX, 0.f});
    }
}

// Hàm riêng tư để xử lý va chạm với chướng ngại vật/mặt đất theo trục Y
//
void PhysicsSystem::HandleYCollision(sf::Sprite &playerSprite, const std::vector<Obstacle> &obstacles, Entity &entity) {
    entity.setIsOnGround(false);

    sf::Vector2f vel = entity.getVelocity();
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

    // Đồng bộ số này với bên HandleX (phải giống nhau)
    float visualSink = 15.0f;

    // 1. Va chạm Y với vật cản
    for (const auto &obs : obstacles) {
        sf::FloatRect obsBounds = obs.sprite->getGlobalBounds();

        // Cắt ngắn hitbox Y để logic rơi xuống mượt hơn
        sf::FloatRect fallHitbox = playerBounds;
        fallHitbox.size.y -= visualSink;

        if (fallHitbox.findIntersection(obsBounds)) {
            if (entity.GetType() == "Dinosaur")
                continue;

            // Điều kiện: Đang rơi VÀ Chân (đã trừ sink) phải cao hơn tâm khối một chút
            if (vel.y > 0 && (fallHitbox.position.y + fallHitbox.size.y) < (obsBounds.position.y + 40.0f)) {
                // Tính toán vị trí đặt chân:
                // Đặt sao cho hitbox nằm TRÊN mặt khối, rồi cộng thêm visualSink để dìm hình xuống
                float newY = (obsBounds.position.y - playerBounds.size.y) + visualSink;

                playerSprite.setPosition({playerSprite.getPosition().x, newY});

                vel.y = 0;
                entity.setVelocity(vel.x, vel.y);
                entity.setIsOnGround(true);
                return;
            } else if (vel.y < 0) // Đụng đầu
            {
                playerSprite.setPosition({playerSprite.getPosition().x, obsBounds.position.y + obsBounds.size.y});
                vel.y = 0;
                entity.setVelocity(vel.x, vel.y);
            }
        }
    }

    // 2. Va chạm Y với MẶT ĐẤT (Ground)
    // Cần tính lại bounds mới nhất
    playerBounds = playerSprite.getGlobalBounds();

    // Logic mặt đất: Cũng phải cho lún xuống tương tự
    if (playerBounds.position.y + playerBounds.size.y >= GROUND_Y + visualSink) {
        playerSprite.setPosition({playerSprite.getPosition().x, (GROUND_Y - playerBounds.size.y) + visualSink});

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