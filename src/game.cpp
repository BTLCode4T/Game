/* --- 1. BAO GỒM THƯ VIỆN --- */
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "Core/GameLoop/game.h"
#include "GamePlay/UI/UI.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include "Core/Constants.h"

/* ---------------------. HẰNG SỐ GAME --- -------------------------------/
//  Khai báo tại Core/Constants.h
// -------------------------------------------------------------------------------//


/* ------------------------- 3. HÀM TIỆN ÍCH GIAO DIỆN ------------------------- */
//  Khai báo tại UI.h
// -------------------------------------------------------------------------------//

/* --- 4. TRẠNG THÁI GAME (ENUM) --- */
// Enum để quản lý các màn hình/trạng thái của game
enum class GameState
{
    MainMenu,   // Màn hình menu chính
    Playing,    // Đang trong game
    HighScores, // Màn hình bảng xếp hạng
    Help,
    Settings
};

// Quay lại menu
void handleReturnToMenu(sf::RenderWindow &window,
                        const std::optional<sf::Event> &event,
                        GameState &currentState, const sf::Sprite &btnHomeSprite)
{
    if (!event.has_value())
    {
        return;
    }

    // --- 1. Xử lý Phím Escape ---
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            currentState = GameState::MainMenu; // Quay lại Menu
            return;                             // Thoát ngay sau khi chuyển trạng thái
        }
    }

    // --- 2. Xử lý Click chuột (Nút Home) ---
    if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButton->button == sf::Mouse::Button::Left)
        {
            // Lấy tọa độ chuột
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

            // Kiểm tra va chạm với nút Home
            if (btnHomeSprite.getGlobalBounds().contains(mousePos))
            {
                currentState = GameState::MainMenu; // Quay lại Menu
            }
        }
    }
}

/* --- 5. HÀM CHÍNH --- */
int main()
{

    // ⚠️⚠️⚠️ ========== PHẦN CHO HỆ ĐIỀU HÀNH ( VUI LÒNG KHÔNG ĐỘNG VÀO ) ======== ⚠️⚠️⚠️

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Dino Game Setup NT - SFML 3.0.0",
                            sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    sf::Image iconImage;

    if (iconImage.loadFromFile("assets/icon.png"))
    {
        unsigned int iconWidth = iconImage.getSize().x;
        unsigned int iconHeight = iconImage.getSize().y;
        const std::uint8_t *iconPixels = iconImage.getPixelsPtr();
        std::size_t iconSize = iconWidth * iconHeight * 4;
        window.setIcon({iconWidth, iconHeight}, iconPixels);
    }
    else
    {
        std::cerr << "Loi: Khong the tai 'assets/Images/icon.png' de lam icon." << std::endl;
    }
    //                     ⚠️⚠️⚠️ ========== KẾT THÚC PHẦN CHO HĐH ======== ⚠️⚠️⚠️



    // ------------------------------------ TẢI TÀI NGUYÊN (LOAD RESOURCES) -------------------------------------------
    // ---------------------------- Tải các phần background ---------------------------------//

    // 1. Tải hình nền (Background)
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/Images/bg.png"))
    {
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

    //  Cái cây
    sf::Texture treeTexture;
    sf::Sprite treeSprite = createSprite(treeTexture, "assets/Images/blend/cay.png", 154.0f, 272.0f, 50.0f, 100.0f);

    // ---------------------------- Kết thúc tải các phần background ---------------------------------//

    // Tải nhân vật
    sf::Texture playerTexture;
    sf::Sprite playerSprite =
        createSprite(playerTexture, "assets/Images/a.png", PLAYER_SIZE, PLAYER_SIZE, PLAYER_START_X, PLAYER_START_Y);
    sf::FloatRect bounds = playerSprite.getLocalBounds();

    //. Tạo mặt đất (Ground)
    sf::RectangleShape ground(sf::Vector2f(WINDOW_WIDTH, GROUND_HEIGHT));
    ground.setFillColor(sf::Color(139, 69, 19));
    ground.setPosition({0.f, GROUND_Y});

    // 1. Tải Texture cho CNV

    // --- SỬ DỤNG VECTOR OBSTACLE VÀ EMPLACE_BACK (Không đổi) ---
    std::vector<Obstacle> obstacles;
    obstacles.emplace_back("assets/Images/dat1.png", 60.0f, 60.0f, 250.0f, 450.0f - 60.0f);
    obstacles.emplace_back("assets/Images/dat2.png", 60.0f, 60.0f, 550.0f, 390.0f); // 450.0f - 60.0f
    obstacles.emplace_back("assets/Images/dat3.png", 60.0f, 60.0f, 800.0f, 390.0f); // 450.0f - 60.0f
    // assets/Images/blend/may.png
    obstacles.emplace_back("assets/Images/blend/may.png", 160.0f, 38.0f, 0.0f, 450.0f - 138.0f);

    // ----------------------------------------------------------------------------------------//

    // 4. ----------------------------------- Tải các nút cho Menu --------------------------------------

    // Tải nút new
    sf::Texture btnNew;
    sf::Sprite btnNewSprite = createSprite(btnNew, "assets/Images/new.png", 300.0f, 70.0f, 333.2f, 150.0f);

    // Tải nút HighScores
    sf::Texture btnHighScores;
    sf::Sprite btnHighScoresSprite =
        createSprite(btnHighScores, "assets/Images/HighScores.png", 300.0f, 70.0f, 333.2f, 250.0f);

    // Tải nút Settings
    sf::Texture btnSettings;
    sf::Sprite btnSettingsSprite =
        createSprite(btnSettings, "assets/Images/Settings.png", 300.0f, 70.0f, 333.2f, 350.0f);

    // Tải nút help
    sf::Texture btnHelp;
    sf::Sprite btnHelpSprite = createSprite(btnHelp, "assets/Images/Help.png", 300.0f, 70.0f, 333.2f, 450.0f);

    // Tải nút Home
    sf::Texture btnHome;
    sf::Sprite btnHomeSprite = createSprite(btnHome, "assets/Images/Home.png", 50.0f, 50.0f, 15.0f, 15.0f);

    /*------------------------------------ Kết thúc tải ảnh cho Menu ----------------------------*/

    /* ----------------------------------- 5. TÀI NGUYÊN CHO TEXT ------------------------------*/

    sf::Font menuFont;
    if (!menuFont.openFromFile("assets/Images/font.ttf"))
    {
        std::cerr << "Loi: Khong the tai 'assets/Images/font.ttf'" << std::endl;
        return -1;
    }

    // Khu vực tạo biến viết chữ

    // Tạo Text cho tiêu đề Bảng Xếp Hạng
    sf::Text highScoresTitleText =
        createText(menuFont, L"Bảng xếp hạng", 40, sf::Color::White, WINDOW_WIDTH / 2.0f, 100.f, true);

    // TEST text nhân vật
    sf::Text IDChacrater =
        createText(menuFont, L"Ngọc Tiên - 1 tỷ sức mạnh", 20, sf::Color::White, WINDOW_WIDTH / 2.0f, 300.f, true);

    // Text: Sử dụng các phím mũi tên <, > và nút space để điều khiển nhân vật
    // Né tránh các chướng ngại vật, tiêu diệt khủng long để giành chiến thắng
    sf::Text helpText = createText(menuFont,
                                   L"Sử dụng các phím mũi tên <, > và nút space để điều khiển nhân vật\n Né tránh các "
                                   L"chướng ngại vật, tiêu diệt khủng long để giành chiến thắng",
                                   20, sf::Color::White, WINDOW_WIDTH / 2.0f, 300.f, true);

    // Text: Thực hiện bởi: Nhóm CODE 4T
    sf::Text aboutText =
        createText(menuFont, L"Thực hiện bởi: Nhóm CODE 4T", 25, sf::Color::Yellow, WINDOW_WIDTH / 2.0f, 550.f, true);

    // TextMenu: https://github.com/BTLCode4T
    // v0.0.1
    sf::Text InfoText = createText(menuFont, L"https://github.com/BTLCode4T\n\t\t\t\t\t\t\t\t\t  v0.0.3", 15,
                                   sf::Color::White, 1000u - 245.0f, 15.0f, false);

    // -------------------------------- Kết thúc tạo text -----------------------------------------//

    // ------------------------------- BIẾN TRẠNG THÁI GAME --------------------------------------//

    GameState currentState = GameState::MainMenu; // Bắt đầu từ Menu chính
    sf::Vector2f velocity(0.f, 0.f);
    bool isOnGround = false;
    sf::Clock clock;
    const int MAX_JUMPS = 2; // Cho phép 2 lần nhảy (1 nhảy đất, 1 nhảy giữa không)
    int jumpsRemaining = MAX_JUMPS;

    // --- VÒNG LẶP GAME CHÍNH ---
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // --- XỬ LÝ SỰ KIỆN (INPUT) ---
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // Sự kiện đóng cửa sổ (luôn xử lý)
            if (event->is<sf::Event::Closed>())
                window.close();

            // Xử lý input dựa trên trạng thái game
            switch (currentState)
            {
            case GameState::MainMenu:
            {
                /* --- XỬ LÝ INPUT MENU  --- */
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    // Ví dụ:
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
                    {
                        // Đặt lại vị trí người chơi khi bắt đầu game
                        playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});
                        velocity = {0.f, 0.f};
                        isOnGround = false;
                        currentState = GameState::Playing; // Chuyển sang màn hình chơi
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::H)
                    {
                        currentState = GameState::HighScores; // Chuyển sang bảng xếp hạng
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        window.close(); // Thoát game từ menu
                    }
                }

                // 2. Xử lý click chuột cho nút btnNew
                if (const auto *mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    // Chỉ xử lý khi nhấn chuột trái
                    if (mouseButton->button == sf::Mouse::Button::Left)
                    {
                        // Chuyển tọa độ pixel của chuột sang tọa độ thế giới (view)
                        // quan trọng khi view bị di chuyển, nhưng ở đây vẫn nên dùng
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

                        // Lấy vùng chữ nhật bao quanh sprite của nút
                        sf::FloatRect buttonBounds = btnNewSprite.getGlobalBounds();
                        sf::FloatRect highScoresButtonBounds = btnHighScoresSprite.getGlobalBounds();
                        sf::FloatRect settingsButtonBounds = btnSettingsSprite.getGlobalBounds();

                        // Kiểm tra xem vị trí chuột có nằm trong vùng của nút không
                        if (buttonBounds.contains(mousePos))
                        {
                            // Nếu có, thực hiện hành động chuyển sang màn hình Playing
                            // (Giống hệt như khi nhấn phím Enter)
                            playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});
                            velocity = {0.f, 0.f};
                            isOnGround = false;
                            currentState = GameState::Playing; // Chuyển sang màn hình chơi
                        }
                        else if (highScoresButtonBounds.contains(mousePos))
                        {
                            // Nếu có, thực hiện hành động chuyển sang màn hình HighScores
                            currentState = GameState::HighScores;
                        }
                        else if (btnHelpSprite.getGlobalBounds().contains(mousePos))
                        {
                            // Nếu có, thực hiện hành động chuyển sang màn hình Help
                            currentState = GameState::Help;
                        }
                        else if (settingsButtonBounds.contains(mousePos))
                        {
                            // Nếu có, thực hiện hành động chuyển sang màn hình Settings
                            currentState = GameState::Settings;
                        }
                    }
                }
                break;
            }
            case GameState::Playing:
            {
                /* --- XỬ LÝ INPUT GAME --- */
                handleReturnToMenu(window, event, currentState, btnHomeSprite);

                // LOGIC NHẢY
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    // ⚠️ THAY ĐỔI LOGIC JUMP ⚠️
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Space && jumpsRemaining > 0)
                    {
                        velocity.y = JUMP_VELOCITY;
                        jumpsRemaining--;   // Giảm số lần nhảy còn lại
                        isOnGround = false; // Luôn reset, dù đang ở trên không hay không
                    }
                }
                break;
            }
            case GameState::HighScores:
            {
                /* --- XỬ LÝ INPUT BẢNG XẾP HẠNG (USER TỰ CODE) --- */
                handleReturnToMenu(window, event, currentState, btnHomeSprite);
                break;
            }
            case GameState::Help:
            {
                handleReturnToMenu(window, event, currentState, btnHomeSprite);
                break;
            }

            case GameState::Settings:
            {
                /* --- XỬ LÝ INPUT MÀN HÌNH SETTINGS (USER TỰ CODE) --- */
                handleReturnToMenu(window, event, currentState, btnHomeSprite);
                break;
            }
            } // kết thúc switch(currentState)
        } // kết thúc while(pollEvent)

        // --- CẬP NHẬT LOGIC GAME (UPDATE) ---
        switch (currentState)
        {
        case GameState::MainMenu:
        {
            /* --- CẬP NHẬT LOGIC MENU (USER TỰ CODE) --- */
            // (ví dụ: hiệu ứng hover nút, ...)
            break;
        }

        case GameState::Playing:
        {
            sf::Vector2f oldPos = playerSprite.getPosition();
            // 1. Cập nhật vận tốc X từ input (VẪN GIỮ NGUYÊN)
            velocity.x = 0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
                velocity.x = -MOVE_SPEED;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
                velocity.x = MOVE_SPEED;

            // 2. GỌI HỆ THỐNG VẬT LÝ MỚI
            PhysicsSystem::Update(playerSprite, velocity, deltaTime, obstacles, isOnGround);

            if (isOnGround)
            {
                jumpsRemaining = MAX_JUMPS;
            }

            // 3. Giới hạn trong cửa sổ (Giữ lại logic này trong GameLoop)
            const sf::Vector2f pos = playerSprite.getPosition();
            if (pos.x < 0.f)
                playerSprite.setPosition({0.f, pos.y});

            sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
            if (pos.x + playerBounds.size.x > WINDOW_WIDTH)
                playerSprite.setPosition({WINDOW_WIDTH - playerBounds.size.x, pos.y});

            break;
        }
        case GameState::HighScores:
        {
            /* --- CẬP NHẬT LOGIC BẢNG XẾP HẠNG (USER TỰ CODE) --- */
            // (thường là không cần)
            break;
        }
        case GameState::Help:
        {
            /* --- CẬP NHẬT LOGIC MÀN HÌNH HELP (USER TỰ CODE) --- */
            // (thường là không cần)
            break;
        }
        case GameState::Settings:
        {
            /* --- CẬP NHẬT LOGIC MÀN HÌNH SETTINGS (USER TỰ CODE) --- */
            // (thường là không cần)
            break;
        }
        } // kết thúc switch(currentState)

        // --- VẼ (RENDER) ---
        window.clear(sf::Color::Black);

        switch (currentState)
        {
        case GameState::MainMenu:
        {
            /* --- VẼ MENU CHÍNH (USER TỰ CODE) --- */
            // 1. Vẽ nền
            window.draw(backgroundSprite);
            window.draw(sunSprite); // Có thể vẽ mặt trời ở menu
            window.draw(treeSprite);
            window.draw(InfoText);
            window.draw(btnNewSprite);
            window.draw(btnHighScoresSprite);
            window.draw(btnSettingsSprite);
            window.draw(btnHelpSprite);
            //
            // 2. Vẽ Tiêu đề game (ví dụ: window.draw(titleText))
            // 3. Vẽ Nút "Bắt đầu" (ví dụ: window.draw(startText))
            // 4. Vẽ Nút "Bảng xếp hạng"
            // 5. Vẽ Nút "Thoát"
            break;
        }
        case GameState::Playing:
        {
            // Code vẽ gốc của bạn
            window.draw(backgroundSprite); // nền
            window.draw(sunSprite);        // mặt trời
            window.draw(ground);           // mặt đất

            // --- THAY ĐỔI: VẼ 3 CHƯỚNG NGẠI VẬT MỚI ---
            // --- VẼ CÁC CHƯỚNG NGẠI VẬT BẰNG VÒNG LẶP ---
            for (const auto &obs : obstacles)
            {
                // Giải tham chiếu con trỏ để có thể truyền đối tượng sf::Sprite
                window.draw(*obs.sprite);
            }
            // -------------------------------------------
            // --- KẾT THÚC THAY ĐỔI ---

            window.draw(playerSprite); // nhân vật
            break;
        }
        case GameState::HighScores:
        {
            /* --- VẼ BẢNG XẾP HẠNG (USER TỰ CODE) --- */
            // 1. Vẽ nền
            window.draw(backgroundSprite);
            window.draw(btnHomeSprite);
            // 2. Vẽ Tiêu đề "Bảng xếp hạng"

            window.draw(highScoresTitleText);
            window.draw(IDChacrater);
            // 3. Vẽ danh sách điểm
            // 4. Vẽ hướng dẫn "Nhấn Escape để quay lại"
            break;
        }
        case GameState::Help:
        {
            /* --- VẼ MÀN HÌNH HELP (USER TỰ CODE) --- */
            // 1. Vẽ nền
            window.draw(backgroundSprite);
            window.draw(btnHomeSprite);
            // 2. Vẽ nội dung hướng dẫn
            window.draw(helpText);
            window.draw(aboutText);
            // 3. Vẽ hướng dẫn "Nhấn Escape để quay lại"
            break;
        }
        case GameState::Settings:
        {
            /* --- VẼ MÀN HÌNH SETTINGS (USER TỰ CODE) --- */
            // 1. Vẽ nền
            window.draw(backgroundSprite);
            window.draw(btnHomeSprite);
            // 2. Vẽ các tùy chọn cài đặt
            // 3. Vẽ hướng dẫn "Nhấn Escape để quay lại"
            break;
        }
        } // kết thúc switch(currentState)

        window.display();
    } // kết thúc while(window.isOpen)

    // system("pause");
    return 0;
}