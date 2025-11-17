#include "Core/Audio/Audio.h"
#include "Core/Audio/MusicManager.h"
#include "Core/GameLoop/game.h"
#include "GamePlay/Avatar/player.h"
#include "GamePlay/Entity/Dinosaur.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/UI/StateUI.h"
#include "Utils/Entity.h"

// vòng lập
void GameManager::runGameLoop() {
    map.map1(window, menuFont, backgroundSprite, backgroundSprite2, sunSprite, treeSprite, ground, ground2, obstacles);
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
                event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter) {
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
        case GameState::GameOver:
            handlGameoverEvent();
            break;
        }
    }

    inputManager.Update();
}

void GameManager::update(float dt) {
    switch (currentState) {
    // CHỈ Playing gọi updatePlaying(dt)
    case GameState::Playing: 
        updatePlaying(dt);
        break;
    
    // MainMenu, HighScores, Help, Settings, GameOver KHÔNG cần update logic game
    case GameState::MainMenu:
    case GameState::HighScores:
    case GameState::Help:
    case GameState::Settings:
    case GameState::GameOver:

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
        window.draw(backgroundSprite);
        window.draw(backgroundSprite2);
        window.draw(sunSprite);
        //window.draw(ground);
        //window.draw(ground2);

        // Vẽ obstacles
        for (const auto &obs : obstacles) {
            window.draw(*obs.sprite);
        }

        for (auto &bullet : bullets) {
            if (!bullet->IsDestroyed()) {
                window.draw(*bullet->animation);
            }
        }
        // Vẽ player
        playerManager.Render(window);
        // (Tùy chọn: Bật dòng dưới để vẽ hitbox debug)
        for (auto &dino_ptr : dinosaurs) {
            dino_ptr->Render(window);
        }

        // vẽ máu
        for (const auto &heart : heartSprites) {
            window.draw(heart);
        }

        // vẽ màn hình game over
        // if (!playerManager.IsAlive()) {
        //     currentState = GameState::GameOver;
        //     // Có thể thêm hiệu ứng âm thanh hoặc pause music ở đây
        //     MusicManager::Get().Stop();
        //     // Audio::Get().Play("game_over");
        // }
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

    case GameState::GameOver:
        gameOverUI.Render(window, menuFont);
        break;
    }
    window.display();
}

// Trong file gameloop.cpp, sửa đổi hàm handlGameoverEvent() như sau:

void GameManager::handlGameoverEvent() {
    // Xử lý click chuột trái cho Game Over
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());

        // Nút Home: Quay về Main Menu
        if (gameOverUI.getHomeButtonSprite().getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click_off");
            MusicManager::Get().Play("menu");
            Audio::Get().Stop("dinosaur");
            ResetGame(); // Reset game trước khi về menu
            currentState = GameState::MainMenu;
            return;
        }

        // Nút Undo: Chơi lại (Reset game và quay về Playing)
        if (gameOverUI.getUndoButtonSprite().getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click");
            ResetGame();            // Reset game để chơi lại
            SpawnInitialEntities(); // Tái tạo entities (như dinosaurs)
            currentState = GameState::Playing;
            return;
        }
    }

    // Nút Q: Quay về Main Menu (backup)
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Q)) {
        Audio::Get().Play("click_off");
        MusicManager::Get().Play("menu");
        ResetGame();
        currentState = GameState::MainMenu;
    }
}

void GameManager::ResetGame() {
    std::cout << "RESET GAME\n";

    // 1. Reset trạng thái Player
    playerManager.setIsAlive(true);
    playerManager.SetHealth(playerManager.getMaxHealth()); // Reset máu về tối đa
    playerManager.setVelocity(0, 0);
    playerManager.setIsOnGround(false);

    // 2. ĐẶT LẠI VỊ TRÍ PLAYER (Sử dụng animation vì Entity.h có unique_ptr<Animation>)
    playerManager.animation->setPosition({PLAYER_START_X, PLAYER_START_Y});

    // 3. Xóa sạch Đạn (Khắc phục lỗi đạn không reset)
    bullets.clear();

    // KHÔNG cần dinosaurs.clear() ở đây, nó sẽ được gọi trong SpawnInitialEntities()
    // Nếu bạn muốn chắc chắn, bạn có thể gọi nó:
    dinosaurs.clear(); // Xóa tất cả Khủng long cũ
}

void GameManager::handleReturnToMenu() {
    // bấm Q quay về menu
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Q)) {
        Audio::Get().Play("click_off");

        if (currentState == GameState::Playing) {
            Audio::Get().Stop("dinosaur");
            MusicManager::Get().Play("menu");
            ResetGame();
        }

        currentState = GameState::MainMenu;

        return;
    }

    // kiểm tra click trái
    if (inputManager.IsMousePressed(sf::Mouse::Button::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());
        if (btnHomeSprite.getGlobalBounds().contains(mousePos)) {
            Audio::Get().Play("click_off");
            MusicManager::Get().Play("menu");
            if (currentState == GameState::Playing) {
                Audio::Get().Stop("dinosaur");
                ResetGame();
            }
            currentState = GameState::MainMenu;
        }
    }
    // Phát âm thanh click
}

void GameManager::handleMainMenuEvent() {
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Enter)) {
        ResetGame();
        SpawnInitialEntities();
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

            ResetGame();
            SpawnInitialEntities();

            playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});
            playerManager.setVelocity(0, 0);
            playerManager.setIsOnGround(false);
            Audio::Get().Play("click");
            MusicManager::Get().Stop();
            Audio::Get().PlayLoopVol("dinosaur", 5.0f);
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

// lúc chơi game
void GameManager::handlePlayingEvent() {
    handleReturnToMenu();
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::Space) && playerManager.getJump() > 0) {
        playerManager.jump(MAX_JUMPS);
        Audio::Get().Play("jump");
    }
    if (inputManager.IsKeyPressed(sf::Keyboard::Scancode::R)) {
        if (Gun *gun = playerManager.GetGun()) {
            gun->Reload();
        }
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

        // Deactivate nếu click không phải input box
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

    // ==============================================================================================================
    // Cập nhật khung hình animation của người chơi
    playerManager.animation->Update(deltaTime);
    // ==============================================================================================================
    sf::Vector2f mousePos = window.mapPixelToCoords(inputManager.GetMousePosition());

    // Lấy tâm của player (dùng globalBounds cho chính xác)
    sf::FloatRect playerBounds = playerManager.animation->getGlobalBounds();
    sf::Vector2f playerCenter(playerBounds.position.x + playerBounds.size.x / 2.f,
                              playerBounds.position.y + playerBounds.size.y / 2.f);

    if (Gun *gun = playerManager.GetGun()) {
        // 1. Cập nhật vị trí và góc quay của súng
        gun->UpdateGun(playerCenter, mousePos);

        // 2. Xử lý bắn (dùng IsMouseDown để giữ chuột)
        if (inputManager.IsMouseDown(sf::Mouse::Button::Left)) {
            if (gun->Shoot()) {
                // Nếu bắn thành công (gun->Shoot() == true)
                // Lấy vị trí đầu nòng
                sf::Vector2f muzzlePos = gun->GetMuzzlePosition();

                // Lấy hướng bắn (từ đầu nòng tới chuột)
                sf::Vector2f fireDirection = mousePos - muzzlePos;

                // Tạo đạn
                CreateBullet(muzzlePos.x, muzzlePos.y, 1, fireDirection, 1500.f); // Tốc độ đạn 1500
            }
        }
    }

    // === CẬP NHẬT ĐẠN VÀ KIỂM TRA VA CHẠM ===
    for (auto &bullet : bullets) {
        // 1. Bỏ qua nếu đạn đã bị đánh dấu xóa
        if (bullet->IsDestroyed())
            continue;

        // 2. Cập nhật vị trí bay của đạn
        bullet->UpdateBullet(deltaTime);

        // 3. Lặp qua tất cả khủng long để kiểm tra va chạm
        for (auto &dino_ptr : dinosaurs) {

            // Lấy hitbox (hình chữ nhật) của đạn và khủng long
            sf::FloatRect bulletBounds = bullet->animation->getGlobalBounds();
            sf::FloatRect dinoBounds = dino_ptr->animation->getGlobalBounds();

            // === LOGIC VA CHẠM AABB (CÚ PHÁP SFML 3) ===

            // 1. Kiểm tra va chạm trục X
            bool collisionX = bulletBounds.position.x + bulletBounds.size.x >= dinoBounds.position.x &&
                              dinoBounds.position.x + dinoBounds.size.x >= bulletBounds.position.x;

            // 2. Kiểm tra va chạm trục Y
            bool collisionY = bulletBounds.position.y + bulletBounds.size.y >= dinoBounds.position.y &&
                              dinoBounds.position.y + dinoBounds.size.y >= bulletBounds.position.y;

            // 3. Nếu va chạm trên cả hai trục
            if (collisionX && collisionY) {

                // TRÚNG MỤC TIÊU!
                dino_ptr->TakeDamage(bullet->GetDamage()); // Khủng long mất máu

                bullet->Destroy(); // Đánh dấu đạn này để xóa

                // Đạn đã trúng 1 con, không cần check con khác
                break;
            }
        }
    }

    // === XÓA ĐẠN (HẾT HẠN HOẶC ĐÃ TRÚNG) ===
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](const auto &b) { return b->IsExpired() || b->IsDestroyed(); }),
                  bullets.end());

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

    if (playerManager.IsAlive()) {
        // Duyệt qua tất cả khủng long trong danh sách 'dinosaurs' (được định nghĩa trong game.h)
        for (const auto &dino : dinosaurs) {

            // *dino vì 'dinosaurs' là vector chứa unique_ptr
            playerManager.HandleDinosaurCollision(*dino);
        }
    }

    if (!playerManager.IsAlive()) {
        currentState = GameState::GameOver;
        // Có thể thêm hiệu ứng âm thanh hoặc pause music ở đây
        MusicManager::Get().Stop();
        Audio::Get().Play("gameOver");
    }
}

void GameManager::updateScrollingBackground(float deltaTime) {

    // --- PHẦN DI CHUYỂN BACKGROUND (PARALLAX) ---
    // Tạo 1 tốc độ di chuyển chậm hơn cho background (ví dụ: 20% tốc độ của mặt đất)
    const float PARALLAX_SPEED = SCROLL_SPEED * 0.2f;

    // Di chuyển cả 2 background sang trái
    backgroundSprite.move({-PARALLAX_SPEED * deltaTime, 0.f});
    backgroundSprite2.move({-PARALLAX_SPEED * deltaTime, 0.f});

    // Lấy chiều rộng của background (chính là chiều rộng cửa sổ)
    const float bgWidth = WINDOW_WIDTH;

    // Kiểm tra background 1
    if (backgroundSprite.getPosition().x + bgWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau background 2
        backgroundSprite.setPosition({backgroundSprite2.getPosition().x + bgWidth - 0.0f, 0.f});
    }

    // Kiểm tra background 2
    if (backgroundSprite2.getPosition().x + bgWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau background 1
        backgroundSprite2.setPosition({backgroundSprite.getPosition().x + bgWidth - 0.0f, 0.f});
    }

    // --- PHẦN DI CHUYỂN MẶT ĐẤT (Đã có) ---
    // Di chuyển cả 2 mảng đất sang trái
    ground.move({-SCROLL_SPEED * deltaTime, 0.f});
    ground2.move({-SCROLL_SPEED * deltaTime, 0.f});

    // Lấy chiều rộng của mặt đất
    const float groundWidth = WINDOW_WIDTH;

    // Kiểm tra mảng đất 1 (ground)
    if (ground.getPosition().x + groundWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau mảng đất 2 (ground2)
        ground.setPosition({ground2.getPosition().x + groundWidth - 0.0f, GROUND_Y});
    }

    // Kiểm tra mảng đất 2 (ground2)
    if (ground2.getPosition().x + groundWidth <= 0.f) {
        // Dịch chuyển nó ra phía sau mảng đất 1 (ground)
        ground2.setPosition({ground.getPosition().x + groundWidth - 0.0f, GROUND_Y});
    }

    // --- PHẦN DI CHUYỂN VẬT CẢN (Đã có) ---
    // dịch chuyển lập lại vật cản
    for (auto &obs : obstacles) {
        // Di chuyển bằng đúng tốc độ cuộn của nền
        obs.sprite->move({-SCROLL_SPEED * deltaTime, 0.f});
        const float obsWidth = obs.sprite->getGlobalBounds().size.x;
        if (obs.sprite->getPosition().x + obsWidth <= 0.f) {
            obs.sprite->move({static_cast<float>(WINDOW_WIDTH) + 300.0f, 0.f});
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

void GameManager::SpawnInitialEntities() {
    // Đảm bảo danh sách Khủng long trống trước khi tạo lại
    dinosaurs.clear();

    // Logic tạo Khủng long được lấy từ constructor của GameManager
    dinosaurs.emplace_back(std::make_unique<Dinosaur>("Rex",
                                                      0.0f, // Vị trí X
                                                      WINDOW_HEIGHT / 2.f,
                                                      100,                             // Máu
                                                      15.0f,                           // Tốc độ
                                                      "assets/Images/raptor-runn.png", // ĐƯỜNG DẪN ẢNH
                                                      250.0f,                          // Rộng
                                                      350.0f, sf::Vector2i(6, 1),      // <-- CHỈNH SỐ FRAME Ở ĐÂY
                                                      0.1f));
    // Nếu có nhiều Khủng long hơn, hãy thêm chúng vào đây.
}