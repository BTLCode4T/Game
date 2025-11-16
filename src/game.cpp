#include "Core/GameLoop/game.h"

#include "Utils/Constants.h"
#include "Core/GameLoop/json.h"
#include "Core/Input/Input.h"
#include "GamePlay/Physics/PhysicsSystem.h"
#include "GamePlay/UI/StateUI.h"
#include "GamePlay/UI/UI.h"
#include "Utils/GameSate.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>


int main() {

    // ⚠️⚠️⚠️ PHẦN CHO HỆ ĐIỀU HÀNH: KHÔNG ĐỘNG VÀO ⚠️⚠️⚠️
    //============================================================================================================
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Dino Game Setup NT - SFML 3.0.0", sf::Style::Default);
    std::cout << "Da khoi tao window" << std::endl;
    window.setFramerateLimit(60);
    window.setMinimumSize(sf::Vector2u(1000, 600));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.size.x;
    unsigned int screenHeight = desktop.size.y;
    window.setMaximumSize(sf::Vector2u(screenWidth, screenHeight));
    window.setVerticalSyncEnabled(true); // Đồng bộ hóa với tần số quét
    sf::Image iconImage;                 // Tải icon game
    if (iconImage.loadFromFile("assets/icon.png")) {
        unsigned int iconWidth = iconImage.getSize().x;
        unsigned int iconHeight = iconImage.getSize().y;
        const std::uint8_t *iconPixels = iconImage.getPixelsPtr();
        std::size_t iconSize = iconWidth * iconHeight * 4;
        window.setIcon({iconWidth, iconHeight}, iconPixels);
    } else {
        std::cerr << "Loi: Khong the tai 'assets/Images/icon.png' de lam icon." << std::endl;
    }
    //============================================================================================================

    //===================================== Phần tải ảnh trước cho game ==========================================
    sf::Texture backgroundTexture; // nền
    if (!backgroundTexture.loadFromFile("assets/Images/bg.png")) {
        std::cerr << "Loi: Khong the tai 'assets/Images/bg.png'" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
    backgroundSprite.setScale(
        sf::Vector2f((float)WINDOW_WIDTH / bgBounds.size.x, (float)WINDOW_HEIGHT / bgBounds.size.y));
    backgroundSprite.setPosition({0.f, 0.f});

    sf::Texture sunTexture; // mặt trời
    sf::Sprite sunSprite = createSprite(sunTexture, "assets/Images/blend/x2sun9.png", 50.0f, 50.0f, 200.0f, 100.0f);

    sf::Texture treeTexture; // cây
    sf::Sprite treeSprite = createSprite(treeTexture, "assets/Images/blend/cay.png", 154.0f, 272.0f, 50.0f, 100.0f);

    sf::Texture playerTexture; // nhân vật
    sf::Sprite playerSprite = createSprite(playerTexture, "assets/Images/a.png",
                                       PLAYER_SIZE, PLAYER_SIZE, PLAYER_START_X, PLAYER_START_Y);


    sf::RectangleShape ground(sf::Vector2f(WINDOW_WIDTH, GROUND_HEIGHT)); // mặt đất
    ground.setFillColor(sf::Color(139, 69, 19));
    ground.setPosition({0.f, GROUND_Y});

    std::vector<Obstacle> obstacles; // chướng ngại vật
    obstacles.emplace_back("assets/Images/dat1.png", 60.0f, 60.0f, 250.0f, 450.0f - 60.0f);
    obstacles.emplace_back("assets/Images/dat2.png", 60.0f, 60.0f, 550.0f, 390.0f);
    obstacles.emplace_back("assets/Images/dat3.png", 60.0f, 60.0f, 800.0f, 390.0f);
    obstacles.emplace_back("assets/Images/blend/may.png", 160.0f, 38.0f, 0.0f, 450.0f - 138.0f);

    sf::Texture btnHome; // nút home
    sf::Sprite btnHomeSprite = createSprite(btnHome, "assets/Images/Home.png", 50.0f, 50.0f, 15.0f, 15.0f);

    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("assets/Images/heart.png")) { // Đảm bảo bạn có file này
        std::cerr << "Loi: Khong the tai 'assets/Images/heart.png'" << std::endl;
        return -1;
    }

    std::vector<sf::Sprite> heartSprites;
    float heartSize = 40.0f; // Kích thước (rộng/cao) của mỗi trái tim
    float padding = 10.0f;   // Khoảng cách từ mép phải và giữa các trái tim

    // Vị trí X của trái tim ngoài cùng bên phải
    float startX = WINDOW_WIDTH - heartSize - padding;

    for (int i = 0; i < 3; ++i) { // Tạo 3 trái tim
        sf::Sprite heartSprite(heartTexture);

        // Tính toán scale để đạt kích thước mong muốn
        sf::FloatRect bounds = heartSprite.getLocalBounds();
        heartSprite.setScale(sf::Vector2f(heartSize / bounds.size.x, heartSize / bounds.size.y));

        // Đặt vị trí: trái tim thứ 2, 3 sẽ cách trái tim trước đó (bên phải)
        heartSprite.setPosition({startX - i * (heartSize + padding), padding});

        heartSprites.push_back(heartSprite);
    }

    sf::Font menuFont; // phong chữ
    if (!menuFont.openFromFile("assets/Images/font.ttf")) {
        std::cerr << "Loi: Khong the tai 'assets/Images/font.ttf'" << std::endl;
        return -1;
    }
   
    //============================================================================================================

    //============================================= Vào game =====================================================

    GameManager manager(window, menuFont, playerSprite, backgroundSprite, sunSprite, treeSprite, ground, btnHomeSprite,
                        obstacles); // truyền trước vào
   
    manager.runGameLoop(); // Bắt đầu vòng lặp game

    return 0;
}
