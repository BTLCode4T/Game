#include "GamePlay/Map/map.h"
#include "GamePlay/UI/UI.h"

void Map::map1(sf::RenderWindow &window, sf::Font &menuFont, sf::Sprite &backgroundSprite,
               sf::Sprite &backgroundSprite2, sf::Sprite &sunSprite, sf::Sprite &treeSprite, sf::RectangleShape &ground,
               sf::RectangleShape &ground2, std::vector<Obstacle> &obstacles) {
    if (!backgroundTexture.loadFromFile("assets/Images/bg_game/Previewx3.png")) {
        std::cerr << "Lỗi: Không thể tải 'assets/Images/bg_game/bgmap1.jpg'\n";
        return;
    } // ảnh 3500 * 2023 thi chỉnh làm sao thấy được phần muốn thấy bằng cách chỉnh tọa độ

    backgroundSprite.setTexture(backgroundTexture);
    sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
    // Hiển thị phần giữa ảnh
    int viewLeft = 0;
    int viewTop = 0;
    sf::IntRect viewRect(sf::Vector2i(viewLeft, viewTop),
                         sf::Vector2i(static_cast<int>(WINDOW_WIDTH)*1.0, static_cast<int>(WINDOW_HEIGHT)*1.0));
    backgroundSprite.setTextureRect(viewRect);

    sf::Vector2u windowSize = window.getSize();

    int setScaleInt = 1;
    // 1. Định nghĩa rõ ràng scaleX và scaleY
    float scaleX = (float)windowSize.x * setScaleInt / viewRect.size.x;
    float scaleY = (float)windowSize.y * setScaleInt / viewRect.size.y;
    backgroundSprite.setScale(sf::Vector2f((float)windowSize.x * setScaleInt / viewRect.size.x,
                                           (float)windowSize.y * setScaleInt / viewRect.size.y));

    backgroundSprite.setPosition({0.f, 0.f});

    backgroundSprite2.setTexture(backgroundTexture);
    backgroundSprite2.setTextureRect(viewRect);
    backgroundSprite2.setScale(sf::Vector2f(scaleX, scaleY));
    // Đặt vị trí của sprite 2 ngay bên phải sprite 1
    backgroundSprite2.setPosition({(float)windowSize.x, 0.f});
    obstacles.emplace_back("assets/Images/dat1.png", 60.0f, 60.0f, 250.0f, 450.0f - 60.0f);
    obstacles.emplace_back("assets/Images/dat2.png", 60.0f, 60.0f, 550.0f, 390.0f);
    obstacles.emplace_back("assets/Images/dat3.png", 60.0f, 60.0f, 800.0f, 390.0f);
    obstacles.emplace_back("assets/Images/blend/may.png", 160.0f, 38.0f, 0.0f, 450.0f - 138.0f);
    ground.setFillColor(sf::Color(0, 0, 0, 0));
    ground2.setFillColor(sf::Color(0, 0, 0, 0));
}
