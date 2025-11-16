#include "Core/Audio/Audio.h"
#include "Core/Audio/MusicManager.h"
#include "Core/GameLoop/game.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/UI/StateUI.h"

// vòng lập
void GameManager::runGameLoop() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); // thời gian giữa 2 frame nè
        playerManager.setPushV(-SCROLL_SPEED * deltaTime);

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

        // phần này cần sửa lại tách hàm
        // riêng******************************************************************************** Xử lý nhập liệu văn bản
        // TRƯỚC khi gọi inputManager.ProcessEvent
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
        if (currentState == GameState::HighScores && event.is<sf::Event::KeyPressed>()) {
            auto keyEvent = event.getIf<sf::Event::KeyPressed>();
            if (keyEvent->code == sf::Keyboard::Key::Up) {
                highScoresUI.scrollUp();
            } else if (keyEvent->code == sf::Keyboard::Key::Down) {
                highScoresUI.scrollDown();
            }
        }
        //******************************************************************************************************************

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
            handleSettingsEvent();
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
        playerManager.Render(window);
        // (Tùy chọn: Bật dòng dưới để vẽ hitbox debug)
        /*drawSpriteBounds(window, *playerManager.animation);*/
        
         // --- VẼ KHỦNG LONG (THÊM VÀO ĐÂY) ---
        for (auto &dino_ptr : dinosaurs) {
            dino_ptr->Render(window);
            // (Nếu bạn dùng debug):
            // drawSpriteBounds(window, *dino_ptr->animation);
        }

         // vẽ máu
        for (const auto &heart : heartSprites) {
            window.draw(heart);
        }

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
        Audio::Get().Play("click_off");
        currentState = GameState::MainMenu;

        return;
    }

    // kiểm tra click trái
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());
        if (btnHomeSprite.getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click_off");
            MusicManager::Get().Play("menu");
            currentState = GameState::MainMenu;
        }
    }
    // Phát âm thanh click
}

void GameManager::handleMainMenuEvent() {
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Enter)) {
        playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});
        playerManager.setVelocity(0, 0);
        playerManager.setIsOnGround(false);
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
            playerManager.setVelocity(0, 0);
            playerManager.setIsOnGround(false);
            Audio::Get().Play("click");
            MusicManager::Get().Stop();
            currentState = GameState::Playing;
        } else if (mainMenu.getBtnHighScoresSprite().getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click");
            MusicManager::Get().Stop();
            MusicManager::Get().Play("HighScores");

            currentState = GameState::HighScores;
        } else if (mainMenu.getBtnHelpSprite().getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click");
            MusicManager::Get().Stop();
            MusicManager::Get().Play("Help");
            currentState = GameState::Help;
        } else if (mainMenu.getBtnSettingsSprite().getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click");
            MusicManager::Get().Stop();
            // Có chưa có nhạc hehe
            // MusicManager::Get().Play("Settings");
            currentState = GameState::Settings;
        }
    }
}

// lúc chơi
void GameManager::handlePlayingEvent() {
    handleReturnToMenu();
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Space) && playerManager.getJump() > 0) {
        playerManager.jump(MAX_JUMPS);
    }
}

void GameManager::handleHighScoresEvent() {
    handleReturnToMenu();
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        if (highScoresUI.getIsNotFoundVisible()) {
            // Nếu đang hiện ảnh Not Found -> Click để tắt nó đi
            highScoresUI.setIsNotFoundVisible(false);
            Audio::Get().Play("click_off");
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
            Audio::Get().Play("switch_task");
        }
        // 2. Điểm tăng dần
        else if (highScoresUI.getBtnIncreasingScore().getGlobalBounds().contains(mousePos)) {
            increasingScore(currentList);
            highScoresUI.resetScroll();
            Audio::Get().Play("switch_task");
        }
        // 3. Thời gian giảm dần
        else if (highScoresUI.getBtnDecreasingTime().getGlobalBounds().contains(mousePos)) {
            decreasingTime(currentList);
            highScoresUI.resetScroll();
            Audio::Get().Play("switch_task");
        }
        // 4. Thời gian tăng dần
        else if (highScoresUI.getBtnIncreasingTime().getGlobalBounds().contains(mousePos)) {
            increasingTime(currentList);
            highScoresUI.resetScroll();
            Audio::Get().Play("switch_task");
        }
    }
}

void GameManager::handleSettingsEvent() {
    // Bấm Q quay về menu
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Q)) {
        Audio::Get().Play("click_off");
        MusicManager::Get().Play("menu");
        currentState = GameState::MainMenu;
        return;
    }

    // kiểm tra click trái
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());

        // --- BỔ SUNG: Xử lý nút Home ---
        if (settingsUI.getHomeButtonSprite().getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click_off");
            MusicManager::Get().Play("menu");
            currentState = GameState::MainMenu;
            return;
        }

        // --- Xử lý nút Mute/Unmute ---
        if (settingsUI.getMutedSprite().getGlobalBounds().contains(mousePos)) {
            bool currentMuteState = Audio::Get().IsMuted();
            bool newMuteState = !currentMuteState;

            Audio::Get().SetMute(newMuteState);

            if (!newMuteState) {
                Audio::Get().Play("click");
            } else {
                Audio::Get().Play("click_off");
            }
        }

        // --- Xử lý nút Music Mute/Unmute ---
        if (settingsUI.getMusicMutedSprite().getGlobalBounds().contains(mousePos)) {
            bool currentMusicMuteState = MusicManager::Get().IsMuted();
            bool newMusicMuteState = !currentMusicMuteState;

            MusicManager::Get().SetMute(newMusicMuteState);

            if (!newMusicMuteState) {
                Audio::Get().Play("click");
            } else {
                Audio::Get().Play("click_off");
            }
        }
    }
}

void GameManager::updatePlaying(float deltaTime) {
    updateScrollingBackground(deltaTime);

    // di chuyển người chơi =========================================================================================
    // không cần thay đổi nữa
    bool leftPressed =
        inputManager.IsKeyDown(sf::Keyboard::Scancode::Left) || inputManager.IsKeyDown(sf::Keyboard::Scancode::A);
    bool rightPressed =
        inputManager.IsKeyDown(sf::Keyboard::Scancode::Right) || inputManager.IsKeyDown(sf::Keyboard::Scancode::D);

    playerManager.Move(leftPressed, rightPressed, deltaTime, obstacles, MAX_JUMPS);

    //Cập nhật khung hình animation của người chơi
    playerManager.animation->Update(deltaTime);

    // Lấy vị trí người chơi để khủng long biết đường đuổi
    sf::Vector2f playerPos = playerManager.animation->getPosition();

    // Lặp qua tất cả khủng long trong danh sách
    for (auto &dino_ptr : dinosaurs) {
        dino_ptr->ChasePlayer(playerPos.x, playerPos.y);
        dino_ptr->animation->Update(deltaTime);
        PhysicsSystem::Update(*dino_ptr->animation, deltaTime, obstacles, *dino_ptr);

    }
    int currentHealth = playerManager.GetHealth(); //
    for (int i = 0; i < heartSprites.size(); ++i) {
        if (i < currentHealth) {
            heartSprites[i].setColor(sf::Color::White); // Hiện
        } else {
            heartSprites[i].setColor(sf::Color(255, 255, 255, 50)); // Mờ
        }
    }
    updateHealthBarUI();
    if (playerManager.IsAlive()) {

        // Duyệt qua tất cả khủng long trong danh sách 'dinosaurs' (được định nghĩa trong game.h)
        for (const auto &dino : dinosaurs) {

            // *dino vì 'dinosaurs' là vector chứa unique_ptr
            playerManager.HandleDinosaurCollision(*dino);
        }
    }
}
      
// ==============================================================================================================
void GameManager::updateScrollingBackground(float deltaTime) {

    // Di chuyển cả 2 mảng đất sang trái
    ground.move({-SCROLL_SPEED * deltaTime, 0.f});
    ground2.move({-SCROLL_SPEED * deltaTime, 0.f});

    // Lấy chiều rộng của mặt đất
    const float groundWidth = WINDOW_WIDTH;

    // Kiểm tra mảng đất 1 (ground)
    if (ground.getPosition().x + groundWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau mảng đất 2 (ground2)
        // Trừ 1.0f để tạo overlap, tránh bị hở
        ground.setPosition({ground2.getPosition().x + groundWidth - 1.0f, GROUND_Y});
    }

    // Kiểm tra mảng đất 2 (ground2)
    if (ground2.getPosition().x + groundWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau mảng đất 1 (ground)
        ground2.setPosition({ground.getPosition().x + groundWidth - 1.0f, GROUND_Y});
    }

    // dịch chuyển lập lại vật cản
    for (auto &obs : obstacles) {
        // Di chuyển bằng đúng tốc độ cuộn của nền
        obs.sprite->move({-SCROLL_SPEED * deltaTime, 0.f});
        const float obsWidth = obs.sprite->getGlobalBounds().size.x;
        if (obs.sprite->getPosition().x + obsWidth <= 0.f) {
            obs.sprite->move({static_cast<float>(WINDOW_WIDTH), 0.f});
        }
    }
    
}
void GameManager::updateHealthBarUI() {
    // 1. Lấy máu hiện tại của player
    int currentPlayerHealth = playerManager.GetHealth();
    // 2. Lặp qua từng trái tim trong vector 'heartSprites'
    for (int i = 0; i < heartSprites.size(); ++i) {

        // 'i' là chỉ số của trái tim (0, 1, 2)

        if (i < currentPlayerHealth) {
            // Nếu máu hiện tại là 2:
            // i = 0 (0 < 2) -> Tim đầy
            // i = 1 (1 < 2) -> Tim đầy
            // i = 2 (2 < 2) -> Sai -> Tim rỗng
            heartSprites[i].setTexture(healthTexture_full);
        } else {
            // Tim rỗng
            heartSprites[i].setTexture(healthTexture_empty);
        }
    }
}