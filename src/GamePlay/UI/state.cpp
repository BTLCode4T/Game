// state.cpp

#include "GamePlay/UI/state.h"
#include "Core/GameLoop/json.h" // Bao gồm nếu cần cho sự kiện đóng cửa sổ
#include <iostream>

// Hàm tiện ích để quay về menu
void GameStateManager::handleReturnToMenu(const std::optional<sf::Event> &event) {
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

// ----------------- XỬ LÝ SỰ KIỆN -----------------

void GameStateManager::handleMainMenuEvent(const std::optional<sf::Event>& event) {
    if (!event.has_value()) return;

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
}

void GameStateManager::handlePlayingEvent(const std::optional<sf::Event>& event) {
    // Xử lý nút Home/Escape
    handleReturnToMenu(event);

    // Logic nhảy
    if (event.has_value()) {
        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space && jumpsRemaining > 0) {
                velocity.y = JUMP_VELOCITY;
                jumpsRemaining--;   // Giảm số lần nhảy
                isOnGround = false; // Reset trạng thái đất
            }
        }
    }
}

void GameStateManager::handleHighScoresEvent(const std::optional<sf::Event>& event) {
    // Xử lý nút Home/Escape
    handleReturnToMenu(event);

    // *** LOGIC XỬ LÝ CLICK SẮP XẾP ***
    if (event.has_value()) {
        if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButton->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);
                
                // Lấy tham chiếu đến danh sách điểm số (đã được sửa thành public)
                List& currentList = highScoresUI.scoresList; 
                
                // 1. Điểm giảm dần
                if (highScoresUI.getBtnDecreasingScore().getGlobalBounds().contains(mousePos)) {
                    decreasingScore(currentList);
                } 
                // 2. Điểm tăng dần
                else if (highScoresUI.getBtnIncreasingScore().getGlobalBounds().contains(mousePos)) {
                    increasingScore(currentList);
                } 
                // 3. Thời gian giảm dần
                else if (highScoresUI.getBtnDecreasingTime().getGlobalBounds().contains(mousePos)) {
                    decreasingTime(currentList);
                } 
                // 4. Thời gian tăng dần
                else if (highScoresUI.getBtnIncreasingTime().getGlobalBounds().contains(mousePos)) {
                    increasingTime(currentList);
                }
            }
        }
    }
}

void GameStateManager::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        // Sự kiện đóng cửa sổ (Luôn xử lý)
        if (event->is<sf::Event::Closed>()) {
            JsonProcessor dataManager;
            dataManager.updateAndWriteData("data/data.json");
            std::cout << "=====================================================" << std::endl;
            window.close();
        }

        // Xử lý input theo trạng thái game
        switch (currentState) {
            case GameState::MainMenu: {
                handleMainMenuEvent(event);
                break;
            }
            case GameState::Playing: {
                handlePlayingEvent(event);
                break;
            }
            case GameState::HighScores: {
                handleHighScoresEvent(event);
                break;
            }
            case GameState::Help:
            case GameState::Settings: {
                handleReturnToMenu(event);
                break;
            }
        } // kết thúc switch(currentState)
    } // kết thúc while(pollEvent)
}

// ----------------- CẬP NHẬT LOGIC -----------------

void GameStateManager::updatePlaying(float deltaTime) {
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
}

void GameStateManager::update(float deltaTime) {
    switch (currentState) {
        case GameState::MainMenu: {
            // Cập nhật menu (hiệu ứng hover, ...)
            break;
        }
        case GameState::Playing: {
            updatePlaying(deltaTime);
            break;
        }
        case GameState::HighScores:
        case GameState::Help:
        case GameState::Settings: {
            // Cập nhật logic không cần thiết (thường không cần)
            break;
        }
    } // kết thúc switch(currentState)
}

// ----------------- VẼ -----------------

void GameStateManager::render() {
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
}

// ----------------- VÒNG LẶP CHÍNH -----------------

void GameStateManager::runGameLoop() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // 1. Xử lý Sự kiện (Input)
        handleEvents();

        // 2. Cập nhật Logic Game (Update)
        update(deltaTime);

        // 3. Vẽ (Render)
        render();
    } // kết thúc while(window.isOpen)
}