#include "Core/GameLoop/game.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/UI/StateUI.h"

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

// In gameloop.cpp, sửa handleEvents(): XÓA phần deactivate input (nếu có), chỉ giữ TextEntered và KeyPressed Enter.
// ĐẶC BIỆT: Không dùng mousePos ở đây, vì mousePos chỉ có trong handleHighScoresEvent.

// In gameloop.cpp, sửa handleEvents(): Đổi Return thành Enter.

void GameManager::handleEvents() {
    while (auto eventOpt = window.pollEvent()) {
        const auto &event = *eventOpt;

        if (event.is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        // Xử lý nhập liệu văn bản TRƯỚC khi gọi inputManager.ProcessEvent
        if (currentState == GameState::HighScores && highScoresUI.isCurrentlyInputting()) {
            // Bắt ký tự (chỉ khi đang nhập)
            if (const auto *text = event.getIf<sf::Event::TextEntered>()) {
                if (text->unicode != 13) // Bỏ qua ký tự Enter
                    highScoresUI.addCharToInput(text->unicode);
            }
            // Bắt phím Enter để kết thúc nhập (dùng event trực tiếp, không cần inputManager cho Enter)
            if (event.is<sf::Event::KeyPressed>() &&
                event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter) { // SỬA: Enter thay vì Return
                highScoresUI.handleInputFinished(highScoresUI.scoresList);
            }
        }


        // --- ĐOẠN NÀY ĐỂ XỬ LÝ CUỘN ---
        if (currentState == GameState::HighScores && event.is<sf::Event::KeyPressed>()) {
            auto keyEvent = event.getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Up) {
                highScoresUI.scrollUp();
            } else if (keyEvent->code == sf::Keyboard::Key::Down) {
                highScoresUI.scrollDown();
            }
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
        window.draw(ground2);

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

// Trong handleHighScoresEvent(): THÊM deactivate input SAU khi lấy mousePos, bên trong if (IsMousePressed).

void GameManager::handleHighScoresEvent() {
    handleReturnToMenu();
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        if (highScoresUI.getIsNotFoundVisible()) {
            // Nếu đang hiện ảnh Not Found -> Click để tắt nó đi
            highScoresUI.setIsNotFoundVisible(false);
            return; // Dừng hàm luôn, không xử lý các nút khác bên dưới
        }
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());

        // Deactivate nếu click không phải input box (THÊM VÀO ĐÂY, sau mousePos)
        if (highScoresUI.isCurrentlyInputting() &&
            !highScoresUI.getBtnTextInputSprite().getGlobalBounds().contains(mousePos)) {
            highScoresUI.setInputActive(false);
            highScoresUI.resetInputState();
        }

        // Lấy tham chiếu đến danh sách điểm số
        List &currentList = highScoresUI.scoresList;

        if (highScoresUI.getBtnTextInputSprite().getGlobalBounds().contains(mousePos)) {
            // Chỉ reset nếu trước đó CHƯA kích hoạt (người dùng mới bắt đầu nhập)
            if (!highScoresUI.isCurrentlyInputting()) {
                highScoresUI.setInputActive(true);
                highScoresUI.resetInputState(); // Reset sạch sẽ để nhập mới
            }
            // Nếu đang nhập rồi mà click lại vào ô đó thì KHÔNG làm gì cả (giữ nguyên text)
        }

        // 1. Điểm giảm dần
        if (highScoresUI.getBtnDecreasingScore().getGlobalBounds().contains(mousePos)) {
            decreasingScore(currentList);
            highScoresUI.resetScroll();
        }
        // 2. Điểm tăng dần
        else if (highScoresUI.getBtnIncreasingScore().getGlobalBounds().contains(mousePos)) {
            increasingScore(currentList);
            highScoresUI.resetScroll();
        }
        // 3. Thời gian giảm dần
        else if (highScoresUI.getBtnDecreasingTime().getGlobalBounds().contains(mousePos)) {
            decreasingTime(currentList);
            highScoresUI.resetScroll();
        }
        // 4. Thời gian tăng dần
        else if (highScoresUI.getBtnIncreasingTime().getGlobalBounds().contains(mousePos)) {
            increasingTime(currentList);
            highScoresUI.resetScroll();
        }
    }
}

void GameManager::updatePlaying(float deltaTime) {
    updateScrollingBackground(deltaTime);
    sf::Vector2f oldPos = playerSprite.getPosition();

    // Lặp qua tất cả chướng ngại vật và di chuyển chúng sang trái

    for (auto &obs : obstacles) {
        // Di chuyển bằng đúng tốc độ cuộn của nền
        obs.sprite->move({-SCROLL_SPEED * deltaTime, 0.f});
        const float obsWidth = obs.sprite->getGlobalBounds().size.x;
        // Kiểm tra xem mép phải của vật cản (vị trí X + chiều rộng)
        // đã đi qua mép trái màn hình (X = 0) hay chưa.
        if (obs.sprite->getPosition().x + obsWidth <= 0.f) {

            // Nếu đã ra khỏi, di chuyển nó về phía trước một khoảng.
            // Chúng ta dùng WINDOW_WIDTH (1000) vì các vật cản ban đầu của bạn
            // (ở 0, 250, 550, 800) dường như tạo thành một "mẫu"
            // lặp lại mỗi 1000 pixel.
            //
            // Việc này sẽ giữ nguyên khoảng cách tương đối giữa chúng.
            // Ví dụ: Vật cản ở 800 sẽ lặp lại ở ~1800.
            //         Vật cản ở 0 sẽ lặp lại ở ~1000.
            // Khoảng cách giữa chúng vẫn là 800.

            obs.sprite->move({static_cast<float>(WINDOW_WIDTH), 0.f});
        }
    }

    bool leftPressed =
        inputManager.IsKeyDown(sf::Keyboard::Scancode::Left) || inputManager.IsKeyDown(sf::Keyboard::Scancode::A);
    bool rightPressed =
        inputManager.IsKeyDown(sf::Keyboard::Scancode::Right) || inputManager.IsKeyDown(sf::Keyboard::Scancode::D);


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
            if (velocity.x < 0.f)
                velocity.x = 0.f;
        } else if (velocity.x < 0.f) {
            // Đang di chuyển sang trái -> ma sát đẩy sang phải
            velocity.x += FRICTION * deltaTime;
            // Kẹp lại
            if (velocity.x > 0.f)
                velocity.x = 0.f;
        }
    }

    // 4. Giới hạn tốc độ tối đa
    if (velocity.x > MAX_MOVE_SPEED) {
        velocity.x = MAX_MOVE_SPEED;
    } else if (velocity.x < -MAX_MOVE_SPEED) {
        velocity.x = -MAX_MOVE_SPEED;
    }

    PhysicsSystem::Update(playerSprite, velocity, deltaTime, obstacles, isOnGround);

    // Reset số lần nhảy nếu chạm đất
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
/* ============================================================
 * HÀM MỚI: Cập nhật nền cuộn
 * (Đây là code cho hàm bạn đã khai báo trong game.h)
 * ============================================================ */
void GameManager::updateScrollingBackground(float deltaTime) {
    // Di chuyển cả 2 mảng đất sang trái
    ground.move({-SCROLL_SPEED * deltaTime, 0.f});
    ground2.move({-SCROLL_SPEED * deltaTime, 0.f});

    // Lấy chiều rộng của mặt đất
    const float groundWidth = WINDOW_WIDTH; // Lấy từ Constants.h

    // Kiểm tra mảng đất 1 (ground)
    // Nếu nó đã đi hết ra khỏi màn hình (về bên trái)
    if (ground.getPosition().x + groundWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau mảng đất 2 (ground2)
        // Trừ 1.0f để tránh bị hở 1 pixel do sai số float
        ground.setPosition({ground2.getPosition().x + groundWidth - 0.f, GROUND_Y});
    }

    // Kiểm tra mảng đất 2 (ground2)
    // Nếu nó đã đi hết ra khỏi màn hình (về bên trái)
    if (ground2.getPosition().x + groundWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau mảng đất 1 (ground)
        ground2.setPosition({ground.getPosition().x + groundWidth - 0.f, GROUND_Y});
    }
}