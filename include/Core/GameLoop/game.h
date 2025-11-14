#pragma once

#include "Utils/Constants.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory> // Cần thiết cho std::unique_ptr
#include <optional>

#include "GamePlay/Physics/PhysicsSystem.h"

#include "GamePlay/UI/Scores.h"
#include "GamePlay/UI/UI.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "GamePlay/UI/StateUI.h"
#include "Utils/GameSate.h"

#include "Core/Input/Input.h"

#include "GamePlay/Avatar/player.h"

using namespace std;

// --- 1. HẰNG SỐ CỬA SỔ (Giữ nguyên) ---

// --- 2. CẤU TRÚC CHO CHƯỚNG NGẠI VẬT (Obstacle) ---
// Phải được định nghĩa ở đây để PhysicsSystem và game.cpp có thể dùng.
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
    sf::RectangleShape ground2;       // Mặt đất thứ 2 để tạo hiệu ứng cuộn nền
    sf::Sprite &btnHomeSprite;        // Nút trở về màn hình chính
    std::vector<Obstacle> &obstacles; // Danh sách các chướng ngại vật

    const int MAX_JUMPS = 2;
    InputManager inputManager;

    GameState currentState; // Trạng thái hiện tại của game (Menu, Playing, v.v.)
    sf::Vector2f velocity;  // Vận tốc di chuyển của nhân vật
    bool isOnGround;        // Nhân vật có đang chạm đất hay không
    int jumpsRemaining;     // Số lần nhảy còn lại
    sf::Clock clock;        // Đồng hồ đo thời gian (cho deltaTime, animation, v.v.)

  PlayerManager playerManager;

  public:
    // ui
    MainMenuUI mainMenu;       // Màn hình menu chính
    HighScoresUI highScoresUI; // Màn hình bảng điểm cao
    HelpUI helpUI;             // Màn hình hướng dẫn
    SettingsUI settingsUI;     // Màn hình cài đặt

  public:
    GameManager(sf::RenderWindow &win,
            sf::Font &font,
            sf::Sprite &player,
            sf::Sprite &bg,
            sf::Sprite &sun,
            sf::Sprite &tree,
            sf::RectangleShape &gr,
            sf::Sprite &btnHome,
            std::vector<Obstacle> &obs)
    : window(win),
      menuFont(font),
      playerSprite(player),
      backgroundSprite(bg),
      sunSprite(sun),
      treeSprite(tree),
      ground(gr),
      btnHomeSprite(btnHome),
      obstacles(obs),

      // Khởi tạo playerManager tại đây nè 👇
      playerManager("Meo_bao", WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, 10, 1.f, "assets/Images/a.png"),

      // Khởi tạo UI
      mainMenu(backgroundSprite, sunSprite, treeSprite, menuFont),
      highScoresUI(backgroundSprite, btnHomeSprite, menuFont),
      helpUI(backgroundSprite, btnHomeSprite, menuFont),
      settingsUI(backgroundSprite, btnHomeSprite, menuFont),

      currentState(GameState::MainMenu),
      velocity(0.f, 0.f),
      isOnGround(false),
      jumpsRemaining(MAX_JUMPS)
{
    ground2.setSize(sf::Vector2f(WINDOW_WIDTH, GROUND_HEIGHT));
    ground2.setFillColor(sf::Color(139, 69, 19));
    ground2.setPosition({WINDOW_WIDTH, GROUND_Y});
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
    // cuộn cuộn
    void updateScrollingBackground(float deltaTime);
};


