#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <memory> // Cần thiết cho std::unique_ptr
#include "Core/Constants.h"

using namespace std;

// --- 1. HẰNG SỐ CỬA SỔ (Giữ nguyên) ---

// --- 2. CẤU TRÚC CHO CHƯỚNG NGẠI VẬT (Obstacle) ---
// Phải được định nghĩa ở đây để PhysicsSystem và game.cpp có thể dùng.
struct Obstacle
{
    // SỬA ĐỔI: Sử dụng unique_ptr cho cả Sprite VÀ Texture
    std::unique_ptr<sf::Sprite> sprite;
    std::unique_ptr<sf::Texture> texture; // <--- THAY ĐỔI Ở ĐÂY

    Obstacle(const std::string &texturePath, float width, float height, float x, float y)
    {
        // TẢI TEXTURE BẰNG CON TRỎ THÔNG MINH
        texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(texturePath))
        {
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

// --- 3. LỚP GAME CHÍNH (Giữ nguyên) ---
class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::CircleShape circle;
};