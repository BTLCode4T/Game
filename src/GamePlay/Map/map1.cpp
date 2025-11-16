#include "GamePlay/Map/map.h"

void Map::map1(sf::RenderWindow &window, sf::Font &menuFont, sf::Sprite &backgroundSprite, sf::Sprite &sunSprite,
               sf::Sprite &treeSprite, sf::RectangleShape &ground, sf::RectangleShape &ground2) {
    if (!backgroundTexture.loadFromFile("assets/Images/bg_game/bgmap1.jpg")) {
        std::cerr << "Lỗi: Không thể tải 'assets/Images/bg_game/bgmap1.jpg'\n";
        return;
    } // ảnh 3500 * 2023 thi chỉnh làm sao thấy được phần muốn thấy bằng cách chỉnh tọa độ

    backgroundSprite.setTexture(backgroundTexture);
    sf::FloatRect bgBounds = backgroundSprite.getLocalBounds();
    // Hiển thị phần giữa ảnh
    int viewLeft = 0;
    int viewTop = 0;
    sf::IntRect viewRect(sf::Vector2i(viewLeft, viewTop),
                         sf::Vector2i(static_cast<int>(WINDOW_WIDTH)*3.6, static_cast<int>(WINDOW_HEIGHT)*4));
    backgroundSprite.setTextureRect(viewRect);

    sf::Vector2u windowSize = window.getSize();

    int setScaleInt = 1;

    backgroundSprite.setScale(sf::Vector2f((float)windowSize.x * setScaleInt / viewRect.size.x,
                                           (float)windowSize.y * setScaleInt / viewRect.size.y));

    backgroundSprite.setPosition({0.f, 0.f});

    

    ground.setFillColor(sf::Color(0, 0, 0, 0));
    ground2.setFillColor(sf::Color(0, 0, 0, 0));
}
