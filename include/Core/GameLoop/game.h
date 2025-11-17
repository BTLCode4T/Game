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
#include "GamePlay/Gun/gun.h"
#include "GamePlay/Gun/bullet.h"

#include "GamePlay/Avatar/player.h"
#include "GamePlay/Entity/Dinosaur.h"
#include "GamePlay/Map/map.h"
using namespace std;

class GameManager {
  private:
    sf::RenderWindow &window;     // Cửa sổ hiển thị game
    sf::Font &menuFont;           // Font chữ dùng cho menu và UI
    sf::Sprite &playerSprite;     // Nhân vật người chơi
    sf::Sprite &backgroundSprite; // Nền của màn chơi
    sf::Sprite &backgroundSprite2;
    sf::Sprite &sunSprite;        // Hình mặt trời (trang trí)
    sf::Sprite &treeSprite;       // Hình cây (trang trí)
    // cuộn
    sf::RectangleShape &ground; // Mặt đất
    sf::RectangleShape ground2; // Mặt đất thứ 2 để tạo hiệu ứng cuộn nền


    sf::Sprite &btnHomeSprite;        // Nút trở về màn hình chính
    std::vector<Obstacle> &obstacles; // Danh sách các chướng ngại vật

    std::vector<sf::Sprite> heartSprites;

    sf::Texture healthTexture_full;  // Texture cho tim đầy
    sf::Texture healthTexture_empty; // Texture cho tim rỗng
    std::vector<std::unique_ptr<Bullet>> bullets;
    InputManager inputManager;

    GameState currentState; // Trạng thái hiện tại của game (Menu, Playing, v.v.)

    sf::Clock clock; // Đồng hồ đo thời gian (cho deltaTime, animation, v.v.)

    PlayerManager playerManager;

    std::vector<std::unique_ptr<Dinosaur>> dinosaurs;
    Map map;

  public:
    // ui
    MainMenuUI mainMenu;       // Màn hình menu chính
    HighScoresUI highScoresUI; // Màn hình bảng điểm cao
    HelpUI helpUI;             // Màn hình hướng dẫn
    SettingsUI settingsUI;     // Màn hình cài đặt
    GameOverUI gameOverUI;     // Màn hình gemOver


  public:
    GameManager(sf::RenderWindow &win, sf::Font &font, sf::Sprite &player, sf::Sprite &bg,sf::Sprite &bg2, sf::Sprite &sun,
                sf::Sprite &tree, sf::RectangleShape &gr, sf::Sprite &btnHome, std::vector<Obstacle> &obs)
        : window(win), menuFont(font), playerSprite(player), backgroundSprite(bg) ,backgroundSprite2(bg2), sunSprite(sun), treeSprite(tree),
          ground(gr), btnHomeSprite(btnHome), obstacles(obs),

          // Khởi tạo playerManager tại đây nè 👇
          playerManager("Meo_bao", 1000.f, WINDOW_HEIGHT / 2.f, 3, 1.f, "assets/Images/sprite_0-sheet.png",
                        PLAYER_SIZE,
                        PLAYER_SIZE,        // Rộng, Cao
                        sf::Vector2i(6, 1), // <-- VÍ DỤ: Ảnh player ("a.png") có 6 khung hình ngang, 1 dọc
                        0.1f),              // <-- VÍ DỤ: 0.1 giây mỗi khung
          // Khởi tạo UI
          mainMenu(backgroundSprite, sunSprite, treeSprite, menuFont),
          highScoresUI(backgroundSprite, btnHomeSprite, menuFont), helpUI(backgroundSprite, btnHomeSprite, menuFont),
          settingsUI(backgroundSprite, btnHomeSprite, menuFont), gameOverUI(backgroundSprite, menuFont),

          currentState(GameState::MainMenu) {
        ground2.setSize(sf::Vector2f(WINDOW_WIDTH, GROUND_HEIGHT));
        ground2.setFillColor(sf::Color::Transparent);
        ground2.setPosition({WINDOW_WIDTH, GROUND_Y});
        auto myGun = std::make_unique<Gun>("Pistol",                                     // Tên súng
                                           "assets/Images/gun/hedgecutter_by_ashmo.png", // ĐƯỜNG DẪN ẢNH SÚNG
                                           50.f, 25.f,                                   // Rộng, Cao
                                           sf::Vector2i(1, 1), 0.1f,                     // 1 frame
                                           1000,                                         // 1000000 viên đạn
                                           0.5f                                          // Bắn 0.2 giây/viên
        );
        // Đưa súng cho player
        playerManager.EquipGun(std::move(myGun));
        // 2. Sửa hàm CreateBullet (đã thêm ở game.h)
        // Thêm định nghĩa hàm này vào gameloop.cpp

        dinosaurs.emplace_back(std::make_unique<Dinosaur>("Rex",
                                                          -100.0f, // Vị trí X
                                                          WINDOW_HEIGHT / 2.f,
                                                          100,                             // Máu
                                                          0.0f,                           // Tốc độ
                                                          "assets/Images/raptor-runn.png", // ĐƯỜNG DẪN ẢNH
                                                          500.0f,                          // Rộng
                                                          600.0f,             // Dàiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii
                                                          sf::Vector2i(6, 1), // <-- CHỈNH SỐ FRAME Ở ĐÂY
                                                          0.1f));
                                                          */
        // >>>>>>>> ĐÃ CHUYỂN SANG gameLoop.cpp <<<<<<<< void GameManager::SpawnInitialEntities()                     


        // 1. Load ảnh tim đầy
        if (!healthTexture_full.loadFromFile("assets/Images/heart.png")) {
            std::cerr << "Loi: Khong the tai 'assets/Images/Heart.png'" << std::endl;
        }
        // 2. Load ảnh tim rỗng
        if (!healthTexture_empty.loadFromFile("assets/Images/border.png")) {
            std::cerr << "Loi: Khong the tai 'assets/Images/Heart-empty.png'" << std::endl;
        }

        // 3. Tạo các sprite trái tim (Bạn có thể đã có đoạn này)
        for (int i = 0; i < playerManager.GetMaxHealth(); ++i) {
            // Sửa lỗi C2512: Cung cấp texture ngay khi tạo sprite
            sf::Sprite heartSprite(healthTexture_full);

            // Sửa lỗi C2660: Dùng dấu {} để tạo sf::Vector2f
            float heartSpacing = 40.f; // Khoảng cách giữa các trái tim
            float rightPadding = 40.f; // Khoảng cách từ lề phải (nên bằng hoặc lớn hơn heartSpacing)
            int maxHearts = playerManager.GetMaxHealth();

            heartSprite.setPosition(
                {WINDOW_WIDTH - ((maxHearts - i) * heartSpacing) - rightPadding + heartSpacing, 20.f});
            heartSprite.setScale({2.0f, 2.0f});

            heartSprites.push_back(heartSprite);
        }
    }
    void CreateBullet(float x, float y, int damage, sf::Vector2f direction, float speed) {
        auto bullet = std::make_unique<Bullet>("assets/Images/bullet/image6.png", // ĐƯỜNG DẪN ẢNH ĐẠN
                                               x, y, 30.f, 40.f,                  // vị trí, rộng, cao
                                               damage, direction, speed);
                                               cout<<"hi";
        bullets.push_back(std::move(bullet));
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
    void handleSettingsEvent();
    void handlGameoverEvent();


    // Hàm update cho từng trạng thái
    void updatePlaying(float deltaTime);

    // Void reset
    void ResetGame();

    // >> MỚI: Hàm tạo lại các thực thể động (như Khủng long) <<
    void SpawnInitialEntities();

    void updateHealthBarUI();
    // cuộn cuộn
    void updateScrollingBackground(float deltaTime);
};
