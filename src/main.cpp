/* --- 1. BAO GỒM THƯ VIỆN --- */
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>

/* --- 2. HẰNG SỐ GAME --- */
const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 600;

const float PLAYER_SIZE = 50.0f;
const float PLAYER_START_X = 100.0f;
const float PLAYER_START_Y = 500.0f; // Sẽ được điều chỉnh khi va chạm đất

const float GROUND_HEIGHT = 50.0f;
const float GROUND_Y = 450.0f;

const float GRAVITY = 3200.0f;
const float JUMP_VELOCITY = -1000.0f;
const float MOVE_SPEED = 200.0f;

/* --- 3. HÀM TIỆN ÍCH VẼ ẢNH (ĐÃ BỊ XÓA VÌ GÂY LAG) --- */
// void paintImg(...) { ... } // <- XÓA HÀM NÀY

/* --- 4. TRẠNG THÁI GAME (ENUM) --- */
// Enum để quản lý các màn hình/trạng thái của game
enum class GameState
{
    MainMenu,  // Màn hình menu chính
    Playing,   // Đang trong game
    HighScores // Màn hình bảng xếp hạng
};

/* --- 5. HÀM CHÍNH --- */
int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                            "Dino Game Setup NT - SFML 3.0.0",
                            sf::Style::Titlebar | sf::Style::Close,
                            sf::State::Windowed);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    

    // --- TẢI TÀI NGUYÊN (LOAD RESOURCES) ---

    // 1. Tải hình nền (Background)
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/bg.png"))
    {
        std::cerr << "Lỗi: Không thể tải 'assets/bg.png'" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
    backgroundSprite.setScale(
        sf::Vector2f(
            (float)WINDOW_WIDTH / bgBounds.size.x,
            (float)WINDOW_HEIGHT / bgBounds.size.y));
    backgroundSprite.setPosition({0.f, 0.f});

    // 2. Tải mặt trời (Sun)
    sf::Texture sunTexture;
    if (!sunTexture.loadFromFile("assets/blend/x2sun9.png"))
    {
        std::cerr << "Lỗi: Không thể tải 'assets/blend/x2sun9.png'" << std::endl;
        return -1;
    }
    sf::Sprite sunSprite(sunTexture);
    sf::FloatRect sunBounds = sunSprite.getLocalBounds();
    sunSprite.setScale(
        sf::Vector2f(
            50.0f / sunBounds.size.x, // Kích thước 50x50 từ code cũ
            50.0f / sunBounds.size.y));
    sunSprite.setPosition({200.f, 100.f}); // Vị trí 200, 100 từ code cũ

    // 3. Tải nhân vật (Player)
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/a.png"))
    {
        std::cerr << "Lỗi: Không thể tải 'assets/a.png'" << std::endl;
        return -1;
    }
    sf::Sprite playerSprite(playerTexture);

    sf::FloatRect bounds = playerSprite.getLocalBounds();
    float scaleX = PLAYER_SIZE / bounds.size.x;
    float scaleY = PLAYER_SIZE / bounds.size.y;
    playerSprite.setScale(sf::Vector2f(scaleX, scaleY));
    playerSprite.setPosition({PLAYER_START_X, PLAYER_START_Y});

    // 4. Tải nut new.png
    sf::Texture btnNew;
    if (!btnNew.loadFromFile("assets/new.png"))
    {
        std::cerr << "Lỗi: Không thể tải 'assets/new.png'" << std::endl;
        return -1;
    }
    sf::Sprite btnNewSprite(btnNew);
    sf::FloatRect btnNewBounds = btnNewSprite.getLocalBounds();
    btnNewSprite.setScale(
        sf::Vector2f(
            (55.6f / sunBounds.size.x) * 0.4,
            (35.7f / sunBounds.size.y) * 0.4));
    // Vị trí giữa màn hình, cao 1/3 màn
    btnNewSprite.setPosition({333.2f, 163.9f});

    // tải nút HighScores
    sf::Texture btnHighScores;
    if (!btnHighScores.loadFromFile("assets/HighScores.png"))
    {
        std::cerr << "Lỗi: Không thể tải 'assets/HighScores.png'" << std::endl;
        return -1;
    }
    sf::Sprite btnHighScoresSprite(btnHighScores);
    sf::FloatRect btnHighScoresBounds = btnHighScoresSprite.getLocalBounds();
    btnHighScoresSprite.setScale(
        sf::Vector2f(
            (55.6f / sunBounds.size.x) * 0.25,
            (35.7f / sunBounds.size.y) * 0.25));
    // Vị trí giữa màn hình, dươi new một khoảng
    btnHighScoresSprite.setPosition(
        {333.2f, 250.0f});



    //. Tạo mặt đất (Ground)
    sf::RectangleShape ground(sf::Vector2f(WINDOW_WIDTH, GROUND_HEIGHT));
    ground.setFillColor(sf::Color(139, 69, 19));
    ground.setPosition({0.f, GROUND_Y});

    /* --- 5. TÀI NGUYÊN CHO MENU (USER TỰ CODE) --- */
    // Ví dụ: Tải Font chữ cho menu
    // sf::Font menuFont;
    // if (!menuFont.loadFromFile("assets/fonts/your_font.ttf"))
    // {
    //     std::cerr << "Lỗi: Không thể tải font" << std::endl;
    //     return -1;
    // }
    // sf::Text titleText("DINO GAME", menuFont, 50);
    // sf::Text startText("Nhan Enter de Bat dau", menuFont, 30);
    // ...

    // --- BIẾN TRẠNG THÁI GAME ---
    GameState currentState = GameState::MainMenu; // Bắt đầu từ Menu chính
    sf::Vector2f velocity(0.f, 0.f);
    bool isOnGround = false;
    sf::Clock clock;

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
                /* --- XỬ LÝ INPUT MENU (USER TỰ CODE) --- */
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
                        // *** SỬA LỖI ***
                        // Trong SFML 3, 'x' và 'y' được thay thế bằng 'position' (một sf::Vector2i)
                        // Code cũ: sf::Vector2f mousePos = window.mapPixelToCoords({mouseButton->x, mouseButton->y});
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseButton->position);

                        // Lấy vùng chữ nhật bao quanh sprite của nút
                        sf::FloatRect buttonBounds = btnNewSprite.getGlobalBounds();

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
                    }
                }
                break;
            }

            case GameState::Playing:
            {
                // Code xử lý input gốc của bạn
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        // Thay vì đóng cửa sổ, quay lại Menu
                        currentState = GameState::MainMenu;
                    }
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::Space && isOnGround)
                    {
                        velocity.y = JUMP_VELOCITY;
                        isOnGround = false;
                    }
                }
                break;
            }

            case GameState::HighScores:
            {
                /* --- XỬ LÝ INPUT BẢNG XẾP HẠNG (USER TỰ CODE) --- */
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    // Ví dụ:
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    {
                        currentState = GameState::MainMenu; // Quay lại Menu
                    }
                }
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
            // Code logic update gốc của bạn

            // Di chuyển
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
                playerSprite.move({-MOVE_SPEED * deltaTime, 0.f});

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
                playerSprite.move({MOVE_SPEED * deltaTime, 0.f});

            // Trọng lực
            if (!isOnGround)
                velocity.y += GRAVITY * deltaTime;

            playerSprite.move({0.f, velocity.y * deltaTime});

            // Va chạm mặt đất
            const sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
            if (playerBounds.position.y + playerBounds.size.y >= GROUND_Y)
            {
                playerSprite.setPosition({playerSprite.getPosition().x, GROUND_Y - playerBounds.size.y});
                velocity.y = 0;
                isOnGround = true;
            }

            // Giới hạn trong cửa sổ
            const sf::Vector2f pos = playerSprite.getPosition();
            if (pos.x < 0.f)
                playerSprite.setPosition({0.f, pos.y});
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
            window.draw(btnNewSprite);
            window.draw(btnHighScoresSprite);
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
            window.draw(playerSprite);     // nhân vật
            break;
        }

        case GameState::HighScores:
        {
            /* --- VẼ BẢNG XẾP HẠNG (USER TỰ CODE) --- */
            // 1. Vẽ nền
            window.draw(backgroundSprite);
            // 2. Vẽ Tiêu đề "Bảng xếp hạng"
            // 3. Vẽ danh sách điểm
            // 4. Vẽ hướng dẫn "Nhấn Escape để quay lại"
            break;
        }
        } // kết thúc switch(currentState)

        window.display();
    } // kết thúc while(window.isOpen)
    system("pause");
    return 0;
}
