// state.h

#pragma once

#include "Core/Constants.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/UI/StateUI.h"
#include "GamePlay/UI/UI.h"
#include "Core/GameLoop/game.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <vector>

// 4. TRẠNG THÁI GAME (ENUM)
enum class GameState {
    MainMenu,   // Màn hình menu chính
    Playing,    // Trạng thái đang chơi game
    HighScores, // Màn hình bảng xếp hạng
    Help,       // Trợ giúp
    Settings    // Cài đặt
};

// Hàm xử lý quay về menu khi nhấn phím Escape hoặc click nút Home (Giữ lại trong file chính/hàm tiện ích nếu muốn, 
// nhưng để đơn giản, ta sẽ đặt nó vào GameStateManager nếu nó thao tác trên trạng thái game.)
// Tuy nhiên, vì nó cần 'window', 'event', 'currentState', 'btnHomeSprite', ta sẽ để nó là hàm tiện ích trong game.cpp hoặc đưa vào class.
// Tốt nhất là đưa logic xử lý trạng thái vào lớp quản lý.

class GameStateManager {
public:
    // Hằng số
    const int MAX_JUMPS = 2;

    // Các thành phần Game
    sf::RenderWindow& window;
    sf::Font& menuFont;
    sf::Sprite& playerSprite;
    sf::Sprite& backgroundSprite;
    sf::Sprite& sunSprite;
    sf::Sprite& treeSprite;
    sf::RectangleShape& ground;
    sf::Sprite& btnHomeSprite;
    std::vector<Obstacle>& obstacles; // Danh sách chướng ngại vật

    // Các trạng thái UI
    MainMenuUI mainMenu;
    HighScoresUI highScoresUI;
    HelpUI helpUI;
    SettingsUI settingsUI;

    // Biến trạng thái game
    GameState currentState;
    sf::Vector2f velocity;
    bool isOnGround;
    int jumpsRemaining;
    sf::Clock clock;

    // Constructor để khởi tạo các tham chiếu và đối tượng UI
    GameStateManager(sf::RenderWindow& win, sf::Font& font, sf::Sprite& player, sf::Sprite& bg, sf::Sprite& sun, 
                     sf::Sprite& tree, sf::RectangleShape& gr, sf::Sprite& btnHome, std::vector<Obstacle>& obs)
        : window(win), menuFont(font), playerSprite(player), backgroundSprite(bg), sunSprite(sun), 
          treeSprite(tree), ground(gr), btnHomeSprite(btnHome), obstacles(obs),
          // Khởi tạo các UI, truyền các Sprite/Font chung vào
          mainMenu(backgroundSprite, sunSprite, treeSprite, menuFont),
          highScoresUI(backgroundSprite, btnHomeSprite, menuFont),
          helpUI(backgroundSprite, btnHomeSprite, menuFont),
          settingsUI(backgroundSprite, btnHomeSprite, menuFont),
          // Khởi tạo trạng thái ban đầu
          currentState(GameState::MainMenu), velocity(0.f, 0.f), isOnGround(false), jumpsRemaining(MAX_JUMPS) {}


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
    void handleReturnToMenu(const std::optional<sf::Event> &event);

    // Hàm xử lý sự kiện cho từng trạng thái
    void handleMainMenuEvent(const std::optional<sf::Event>& event);
    void handlePlayingEvent(const std::optional<sf::Event>& event);
    void handleHighScoresEvent(const std::optional<sf::Event>& event);
    
    // Hàm update cho từng trạng thái
    void updatePlaying(float deltaTime);
};