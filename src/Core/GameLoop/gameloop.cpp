#include "Core/GameLoop/game.h"
#include "GamePlay/Physics/PhysicsSystem.h"

// vòng lập
void GameManager::runGameLoop() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); // thời gian giữa 2 frame nè

        //============================================================================================================
        // 1. Xử lý Sự kiện (Input) // bắt buộc có, nếu  k sẽ k chạy đc :-)
        handleEvents();

        //============================================================================================================
        // 2. Cập nhật Logic Game (Update)
        update(deltaTime);

        //============================================================================================================
        // 3. Vẽ (Render)
        render();

        //============================================================================================================
    }
}

void GameManager::handleEvents() {
    while (auto eventOpt = window.pollEvent()) {
        const auto &event = *eventOpt;

        if (event.is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        inputManager.ProcessEvent(event);

        switch (currentState) {
        case GameState::MainMenu:
            handleMainMenuEvent();
            break;
        case GameState::Playing:
            handlePlayingEvent();
            break;
        case GameState::HighScores:
            handleHighScoresEvent();
            break;
        case GameState::Help:
        case GameState::Settings:
            handleReturnToMenu();
            break;
        }
    }

    inputManager.Update();
}

void GameManager::update(float dt) {
    // chứa các update
    switch (currentState) {
    case GameState::MainMenu:
        break;
    case GameState::Playing:
        updatePlaying(dt);
        break;
    case GameState::HighScores:

        break;
    case GameState::Help:
        break;
    case GameState::Settings:
        break;
    }
}
// Vẽ theo trạng thái game
void GameManager::render() {
    window.clear(sf::Color::Black);
    
    switch (currentState) {
    case GameState::MainMenu:
        mainMenu.Render(window, menuFont);
        break;

    case GameState::Playing:
        // Nền và hiệu ứng
        window.draw(backgroundSprite);
        window.draw(sunSprite);
        window.draw(ground);

        // Vẽ obstacles
        for (const auto &obs : obstacles) {
            window.draw(*obs.sprite);
        }

        // Vẽ player
        window.draw(playerSprite);
        break;

    case GameState::HighScores:
        highScoresUI.Render(window, menuFont);
        break;

    case GameState::Help:
        helpUI.Render(window, menuFont);
        break;

    case GameState::Settings:
        settingsUI.Render(window, menuFont);
        break;
    }

    window.display(); // Hiển thị frame vừa vẽ
}

void GameManager::handleReturnToMenu() {
    // bấm Q quay về menu
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Q)) {
        currentState = GameState::MainMenu;
        return;
    }

    // kiểm tra click trái
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());
        if (btnHomeSprite.getGlobalBounds().contains(mousePos)) {
            currentState = GameState::MainMenu;
        }
    }
}

void GameManager::handleMainMenuEvent() {

    // Enter
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Enter)) {
        playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});
        velocity = {0.f, 0.f};
        isOnGround = false;
        currentState = GameState::Playing;
    } else if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::H)) {
        currentState = GameState::HighScores;
    } else if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Escape)) {
        window.close();
    }

    // kiểm tra click chuột trái
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        // lấy vị trí chuột
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());

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

// lúc chơi
void GameManager::handlePlayingEvent() {
    handleReturnToMenu();

    // nhẩy
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Space) && jumpsRemaining > 0) {
        velocity.y = JUMP_VELOCITY;
        jumpsRemaining--;
        isOnGround = false;
    }
}

void GameManager::handleHighScoresEvent() {
    handleReturnToMenu();

    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());

        // Lấy tham chiếu đến danh sách điểm số
        List &currentList = highScoresUI.scoresList;

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

void GameManager::updatePlaying(float deltaTime) {
    sf::Vector2f oldPos = playerSprite.getPosition();

    bool leftPressed = inputManager.IsKeyDown(sf::Keyboard::Scancode::Left) || 
                       inputManager.IsKeyDown(sf::Keyboard::Scancode::A);
    bool rightPressed = inputManager.IsKeyDown(sf::Keyboard::Scancode::Right) || 
                        inputManager.IsKeyDown(sf::Keyboard::Scancode::D);

    // 2. Tăng/Giảm tốc độ (Gia tốc)
    if (leftPressed) {
        // Tăng tốc sang trái (velocity.x giảm)
        velocity.x -= ACCELERATION * deltaTime;
    } else if (rightPressed) {
        // Tăng tốc sang phải (velocity.x tăng)
        velocity.x += ACCELERATION * deltaTime;
    } else {
        // 3. Không nhấn phím -> Áp dụng ma sát (Giảm tốc)
        if (velocity.x > 0.f) {
            // Đang di chuyển sang phải -> ma sát đẩy sang trái
            velocity.x -= FRICTION * deltaTime;
            // Kẹp lại, tránh bị "trôi" ngược
            if (velocity.x < 0.f) velocity.x = 0.f; 
        } else if (velocity.x < 0.f) {
            // Đang di chuyển sang trái -> ma sát đẩy sang phải
            velocity.x += FRICTION * deltaTime;
            // Kẹp lại
            if (velocity.x > 0.f) velocity.x = 0.f; 
        }
    }

    // 4. Giới hạn tốc độ tối đa
    if (velocity.x > MAX_MOVE_SPEED) {
        velocity.x = MAX_MOVE_SPEED;
    } else if (velocity.x < -MAX_MOVE_SPEED) {
        velocity.x = -MAX_MOVE_SPEED;
    }

    PhysicsSystem::Update(playerSprite, velocity, deltaTime, obstacles, isOnGround);

    //Reset số lần nhảy nếu chạm đất
    if (isOnGround) {
        jumpsRemaining = MAX_JUMPS;
    }

    //Giới hạn trong cửa sổ
    const sf::Vector2f pos = playerSprite.getPosition();
    if (pos.x < 0.f)
        playerSprite.setPosition({0.f, pos.y});

    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
    if (pos.x + playerBounds.size.x > WINDOW_WIDTH)
        playerSprite.setPosition({WINDOW_WIDTH - playerBounds.size.x, pos.y});
}
