// UI.cpp

#include "GamePlay/UI/UI.h"

// --- Định nghĩa Struct Obstacle (đã tách ra khỏi game.cpp) ---
// Obstacle::Obstacle(const std::string &filePath, float width, float height, float x, float y) {
//     texture = std::make_unique<sf::Texture>();
//     if (!texture->loadFromFile(filePath)) {
//         std::cerr << "Loi: Khong the tai '" << filePath << "'" << std::endl;
//     }
//     sprite = std::make_unique<sf::Sprite>(*texture);
//     sf::FloatRect bounds = sprite->getLocalBounds();
//     float scaleX = width / bounds.size.x;
//     float scaleY = height / bounds.size.y;
//     sprite->setScale(sf::Vector2f(scaleX, scaleY));
//     sprite->setPosition({x, y});
// }

void drawSpriteBounds(sf::RenderWindow &window, const sf::Sprite &sprite) {
    sf::FloatRect bounds = sprite.getGlobalBounds();

    sf::RectangleShape rect;
    rect.setPosition(bounds.position);
    rect.setSize({bounds.size.x, bounds.size.y});
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(2.f); // độ dày viền
    

    window.draw(rect);
}

// --- Định nghĩa Hàm createSprite ---
sf::Sprite createSprite(sf::Texture &textureRef, const std::string &filePath, float desiredWidth, float desiredHeight,
                        float x, float y) {
    if (!textureRef.loadFromFile(filePath)) {
        std::cerr << "Loi: Khong the tai '" << filePath << "'" << std::endl;
        // Trả về sprite với texture đã tải (nếu có lỗi, texture sẽ rỗng)
        return sf::Sprite(textureRef); 
    }
    sf::Sprite sprite(textureRef);
    sf::FloatRect bounds = sprite.getLocalBounds();
    float scaleX = desiredWidth / bounds.size.x;
    float scaleY = desiredHeight / bounds.size.y;
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition({x, y});

    return sprite;
}

// --- Định nghĩa Hàm createText ---
sf::Text createText(const sf::Font &fontRef, const std::wstring &content, unsigned int size, const sf::Color &color,
                    float x, float y, bool centerAnchor) {
    sf::Text text(fontRef, content, size);
    text.setFillColor(color);
    if (centerAnchor) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
    }
    text.setPosition(sf::Vector2f(x, y));
    return text;
}