/* --- 1. BAO GỒM THƯ VIỆN --- */
// Bao gồm các thư viện cốt lõi, UI, vật lý, SFML và các thư viện chuẩn C++
#include "Core/GameLoop/game.h"
#include "Core/Constants.h"
#include "Core/GameLoop/json.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/UI/StateUI.h" // THÊM INCLUDE CHO CÁC LỚP UI MỚI
#include "GamePlay/UI/UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

/* ---------------------. HẰNG SỐ GAME --- -------------------------------/
//  Khai báo các hằng số trong Core/Constants.h
// -------------------------------------------------------------------------------//


/* ------------------------- 3. HÀM TIỆN ÍCH GIAO DIỆN ------------------------- */
//  Các hàm tiện ích cho UI, khai báo trong UI.h
// -------------------------------------------------------------------------------//

/* --- 4. TRẠNG THÁI GAME (ENUM) --- */
// Enum quản lý các màn hình hoặc trạng thái của game
enum class GameState {
    MainMenu,   // Màn hình menu chính
    Playing,    // Trạng thái đang chơi game
    HighScores, // Màn hình bảng xếp hạng
    Help,       // Trợ giúp
    Settings    // Cài đặt
};

// Hàm xử lý quay về menu khi nhấn phím Escape hoặc click nút Home
void handleReturnToMenu(sf::RenderWindow &window, const std::optional<sf::Event> &event, GameState &currentState,
                        const sf::Sprite &btnHomeSprite) {
    if (!event.has_value()) {
        return;
    }

    // --- 1. Xử lý Phím Escape ---
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
            currentState = GameState::MainMenu; // Quay lại Menu
            return;                             // Thoát ngay sau khi chuyển trạng thái
        }
    }

    // --- 2. Xử lý Click chuột (Nút Home) ---
    if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButton->button == sf::Mouse::Button::Left) {
            // Lấy tọa độ chuột trong cửa sổ
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

            // Kiểm tra va chạm với nút Home
            if (btnHomeSprite.getGlobalBounds().contains(mousePos)) {
                currentState = GameState::MainMenu; // Quay lại Menu
            }
        }
    }
}

/* --- 5. HÀM CHÍNH --- */
int main() {

    // ⚠️⚠️⚠️ PHẦN CHO HỆ ĐIỀU HÀNH: KHÔNG ĐỘNG VÀO ⚠️⚠️⚠️
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Dino Game Setup NT - SFML 3.0.0",
                            sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
    window.setFramerateLimit(60);          // Giới hạn FPS
    window.setVerticalSyncEnabled(true);   // Đồng bộ hóa với tần số quét
    sf::Image iconImage;

    // Tải icon game
    if (iconImage.loadFromFile("assets/icon.png")) {
        unsigned int iconWidth = iconImage.getSize().x;
        unsigned int iconHeight = iconImage.getSize().y;
        const std::uint8_t *iconPixels = iconImage.getPixelsPtr();
        std::size_t iconSize = iconWidth * iconHeight * 4;
        window.setIcon({iconWidth, iconHeight}, iconPixels);
    } else {
        std::cerr << "Loi: Khong the tai 'assets/Images/icon.png' de lam icon." << std::endl;
    }
    // ⚠️⚠️⚠️ KẾT THÚC PHẦN CHO HỆ ĐIỀU HÀNH ⚠️⚠️⚠️

    // ---------------------------- Tải các phần background ---------------------------------//

    // 1. Tải hình nền (Background)
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/Images/bg.png")) {
        std::cerr << "Loi: Khong the tai 'assets/Images/bg.png'" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
    backgroundSprite.setScale(
        sf::Vector2f((float)WINDOW_WIDTH / bgBounds.size.x, (float)WINDOW_HEIGHT / bgBounds.size.y));
    backgroundSprite.setPosition({0.f, 0.f});

    // Mặt trời
    sf::Texture sunTexture;
    sf::Sprite sunSprite = createSprite(sunTexture, "assets/Images/blend/x2sun9.png", 50.0f, 50.0f, 200.0f, 100.0f);
    sf::FloatRect sunBounds = sunSprite.getLocalBounds();

    // Cây
    sf::Texture treeTexture;
    sf::Sprite treeSprite = createSprite(treeTexture, "assets/Images/blend/cay.png", 154.0f, 272.0f, 50.0f, 100.0f);

    // ---------------------------- Kết thúc tải background ---------------------------------//

    // Tải nhân vật
    sf::Texture playerTexture;
    sf::Sprite playerSprite =
        createSprite(playerTexture, "assets/Images/a.png", PLAYER_SIZE, PLAYER_SIZE, PLAYER_START_X, PLAYER_START_Y);
    sf::FloatRect bounds = playerSprite.getLocalBounds();

    // Tạo mặt đất (Ground)
    sf::RectangleShape ground(sf::Vector2f(WINDOW_WIDTH, GROUND_HEIGHT));
    ground.setFillColor(sf::Color(139, 69, 19));
    ground.setPosition({0.f, GROUND_Y});

    // 1. Tải Texture cho chướng ngại vật (Obstacle)
    // Sử dụng vector và emplace_back (không đổi)
    std::vector<Obstacle> obstacles;
    obstacles.emplace_back("assets/Images/dat1.png", 60.0f, 60.0f, 250.0f, 450.0f - 60.0f);
    obstacles.emplace_back("assets/Images/dat2.png", 60.0f, 60.0f, 550.0f, 390.0f); // 450.0f - 60.0f
    obstacles.emplace_back("assets/Images/dat3.png", 60.0f, 60.0f, 800.0f, 390.0f); // 450.0f - 60.0f
    obstacles.emplace_back("assets/Images/blend/may.png", 160.0f, 38.0f, 0.0f, 450.0f - 138.0f);

    // ---------------------------- Nút Home ----------------------------//
    sf::Texture btnHome;
    sf::Sprite btnHomeSprite = createSprite(btnHome, "assets/Images/Home.png", 50.0f, 50.0f, 15.0f, 15.0f);

    /* ----------------------------------- Tải font ------------------------------*/
    sf::Font menuFont;
    if (!menuFont.openFromFile("assets/Images/font.ttf")) {
        std::cerr << "Loi: Khong the tai 'assets/Images/font.ttf'" << std::endl;
        return -1;
    }

    // ---------------------------- KHỞI TẠO CÁC TRẠNG THÁI UI --------------------------//
    MainMenuUI mainMenu(backgroundSprite, sunSprite, treeSprite, menuFont);
    HighScoresUI highScoresUI(backgroundSprite, btnHomeSprite, menuFont);
    HelpUI helpUI(backgroundSprite, btnHomeSprite, menuFont);
    SettingsUI settingsUI(backgroundSprite, btnHomeSprite, menuFont);
    // -------------------------------------------------------------------------------//

    // ------------------------------- BIẾN TRẠNG THÁI GAME --------------------------------------//
    GameState currentState = GameState::MainMenu; // Bắt đầu từ Menu chính
    sf::Vector2f velocity(0.f, 0.f);
    bool isOnGround = false;
    sf::Clock clock;
    const int MAX_JUMPS = 2; // Số lần nhảy tối đa
    int jumpsRemaining = MAX_JUMPS;

    // --- VÒNG LẶP GAME CHÍNH ---
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // --- XỬ LÝ SỰ KIỆN (INPUT) ---
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            // Sự kiện đóng cửa sổ
            if (event->is<sf::Event::Closed>()) {
                JsonProcessor dataManager;
                dataManager.updateAndWriteData("data/data.json");
                cout << "=====================================================" << endl;
                window.close();
            }

            // Xử lý input theo trạng thái game
            switch (currentState) {
            case GameState::MainMenu: {
                // --- Xử lý phím trong menu ---
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
                        playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});
                        velocity = {0.f, 0.f};
                        isOnGround = false;
                        currentState = GameState::Playing;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::H) {
                        currentState = GameState::HighScores;
                    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window.close();
                    }
                }

                // --- Xử lý click chuột cho menu ---
                if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                        if (mainMenu.getBtnNewSprite().getGlobalBounds().contains(mousePos)) {
                            playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});
                            velocity = {0.f, 0.f};
                            isOnGround = false;
                            currentState = GameState::Playing;
                        } else if (mainMenu.getBtnHighScoresSprite().getGlobalBounds().contains(mousePos)) {
                            currentState = GameState::HighScores;
                        } else if (mainMenu.getBtnHelpSprite().getGlobalBounds().contains(mousePos)) {
                            currentState = GameState::Help;
                        } else if (mainMenu.getBtnSettingsSprite().getGlobalBounds().contains(mousePos)) {
                            currentState = GameState::Settings;
                        }
                    }
                }
                break;
            }

            case GameState::Playing: {
                // --- Xử lý input khi đang chơi ---
                handleReturnToMenu(window, event, currentState, btnHomeSprite);

                // Logic nhảy
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Space && jumpsRemaining > 0) {
                        velocity.y = JUMP_VELOCITY;
                        jumpsRemaining--;   // Giảm số lần nhảy
                        isOnGround = false; // Reset trạng thái đất
                    }
                }
                break;
            }
            case GameState::HighScores:
            case GameState::Help:
            case GameState::Settings: {
                handleReturnToMenu(window, event, currentState, btnHomeSprite);
                break;
            }
            } // kết thúc switch(currentState)
        } // kết thúc while(pollEvent)

        // --- CẬP NHẬT LOGIC GAME (UPDATE) ---
        switch (currentState) {
        case GameState::MainMenu: {
            // Cập nhật menu (hiệu ứng hover, ...)
            break;
        }

        case GameState::Playing: {
            sf::Vector2f oldPos = playerSprite.getPosition();
            // Cập nhật vận tốc X từ input
            velocity.x = 0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
                velocity.x = -MOVE_SPEED;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
                velocity.x = MOVE_SPEED;

            // Gọi hệ thống vật lý
            PhysicsSystem::Update(playerSprite, velocity, deltaTime, obstacles, isOnGround);

            if (isOnGround) {
                jumpsRemaining = MAX_JUMPS;
            }

            // Giới hạn trong cửa sổ
            const sf::Vector2f pos = playerSprite.getPosition();
            if (pos.x < 0.f)
                playerSprite.setPosition({0.f, pos.y});

            sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
            if (pos.x + playerBounds.size.x > WINDOW_WIDTH)
                playerSprite.setPosition({WINDOW_WIDTH - playerBounds.size.x, pos.y});

            break;
        }
        case GameState::HighScores:
        case GameState::Help:
        case GameState::Settings: {
            // Cập nhật logic không cần thiết (thường không cần)
            break;
        }
        } // kết thúc switch(currentState)

        // -------------------------------- VẼ (RENDER) --------------------------------------
        window.clear(sf::Color::Black);

        switch (currentState) {
        case GameState::MainMenu: {
            mainMenu.Render(window, menuFont); // Render menu
            break;
        }
        case GameState::Playing: {
            window.draw(backgroundSprite); // nền
            window.draw(sunSprite);        // mặt trời
            window.draw(ground);           // mặt đất

            // Vẽ các chướng ngại vật
            for (const auto &obs : obstacles) {
                window.draw(*obs.sprite);
            }

            window.draw(playerSprite); // nhân vật
            break;
        }
        case GameState::HighScores: {
            highScoresUI.Render(window, menuFont);
            break;
        }
        case GameState::Help: {
            helpUI.Render(window, menuFont);
            break;
        }
        case GameState::Settings: {
            settingsUI.Render(window, menuFont);
            break;
        }
        } // kết thúc switch(currentState)

        window.display();
    } // kết thúc while(window.isOpen)

    return 0;
}
