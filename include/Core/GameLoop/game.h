#pragma once

#include "Utils/Constants.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory> // Cần thiết cho std::unique_ptr
#include <optional>



#include "GamePlay/UI/Scores.h"
#include "GamePlay/UI/UI.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "GamePlay/UI/StateUI.h"
#include "Utils/GameSate.h"

#include "Core/Input/Input.h"

using namespace std;

// --- 1. HẰNG SỐ CỬA SỔ (Giữ nguyên) ---

// --- 2. CẤU TRÚC CHO CHƯỚNG NGẠI VẬT (Obstacle) ---
// Phải được định nghĩa ở đây để PhysicsSystem và game.cpp có thể dùng.
struct Obstacle {
    // SỬA ĐỔI: Sử dụng unique_ptr cho cả Sprite VÀ Texture
    std::unique_ptr<sf::Sprite> sprite;
    std::unique_ptr<sf::Texture> texture; // <--- THAY ĐỔI Ở ĐÂY

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

// Hàm xử lý quay về menu khi nhấn phím Escape hoặc click nút Home (Giữ lại trong file chính/hàm tiện ích nếu muốn,
// nhưng để đơn giản, ta sẽ đặt nó vào GameManager nếu nó thao tác trên trạng thái game.)
// Tuy nhiên, vì nó cần 'window', 'event', 'currentState', 'btnHomeSprite', ta sẽ để nó là hàm tiện ích trong game.cpp
// hoặc đưa vào class. Tốt nhất là đưa logic xử lý trạng thái vào lớp quản lý.

class GameManager {
  private:
    sf::RenderWindow &window;         // Cửa sổ hiển thị game
    sf::Font &menuFont;               // Font chữ dùng cho menu và UI
    sf::Sprite &playerSprite;         // Nhân vật người chơi
    sf::Sprite &backgroundSprite;     // Nền của màn chơi
    sf::Sprite &sunSprite;            // Hình mặt trời (trang trí)
    sf::Sprite &treeSprite;           // Hình cây (trang trí)
    sf::RectangleShape &ground;       // Mặt đất
    sf::Sprite &btnHomeSprite;        // Nút trở về màn hình chính
    std::vector<Obstacle> &obstacles; // Danh sách các chướng ngại vật

    const int MAX_JUMPS = 2;
    InputManager inputManager;

    GameState currentState; // Trạng thái hiện tại của game (Menu, Playing, v.v.)
    sf::Vector2f velocity;  // Vận tốc di chuyển của nhân vật
    bool isOnGround;        // Nhân vật có đang chạm đất hay không
    int jumpsRemaining;     // Số lần nhảy còn lại
    sf::Clock clock;        // Đồng hồ đo thời gian (cho deltaTime, animation, v.v.)

  public:
    // ui
    MainMenuUI mainMenu;       // Màn hình menu chính
    HighScoresUI highScoresUI; // Màn hình bảng điểm cao
    HelpUI helpUI;             // Màn hình hướng dẫn
    SettingsUI settingsUI;     // Màn hình cài đặt
    
  public:
    GameManager(sf::RenderWindow &win,     // Cửa sổ hiển thị game
                sf::Font &font,            // Phông chữ cho UI
                sf::Sprite &player,        // Nhân vật người chơi
                sf::Sprite &bg,            // Nền chính
                sf::Sprite &sun,           // Mặt trời
                sf::Sprite &tree,          // Cây
                sf::RectangleShape &gr,    // Mặt đất
                sf::Sprite &btnHome,       // Nút quay về menu
                std::vector<Obstacle> &obs // Danh sách chướng ngại vật
                )
        : window(win),            // Gán cửa sổ
          menuFont(font),         // Gán phông chữ
          playerSprite(player),   // Gán sprite người chơi
          backgroundSprite(bg),   // Gán nền
          sunSprite(sun),         // Gán mặt trời
          treeSprite(tree),       // Gán cây
          ground(gr),             // Gán mặt đất
          btnHomeSprite(btnHome), // Gán nút Home
          obstacles(obs),         // Gán danh sách chướng ngại vật

          // Khởi tạo các UI – truyền sprite nền và phông chữ chung
          mainMenu(backgroundSprite, sunSprite, treeSprite, menuFont),
          highScoresUI(backgroundSprite, btnHomeSprite, menuFont), helpUI(backgroundSprite, btnHomeSprite, menuFont),
          settingsUI(backgroundSprite, btnHomeSprite, menuFont),

          // Thiết lập trạng thái ban đầu
          currentState(GameState::MainMenu), // Trạng thái: Menu chính
          velocity(0.f, 0.f),                // Tốc độ ban đầu = 0
          isOnGround(false),                 // Chưa đứng trên đất
          jumpsRemaining(MAX_JUMPS)          // Số lần nhảy còn lại = tối đa
    {
    }

    // Hàm chính chạy vòng lặp game
    void runGameLoop();

  private:
    // Hàm xử lý sự kiện
    void handleEvents();

    // Hàm cập nhật logic
    void update(float deltaTime);

    // Hàm vẽ
    void render();

    // Hàm tiện ích để quay về menu
    void handleReturnToMenu();

    // Hàm xử lý sự kiện cho từng trạng thái
    void handleMainMenuEvent();
    void handlePlayingEvent();
    void handleHighScoresEvent();

    // Hàm update cho từng trạng thái
    void updatePlaying(float deltaTime);
};