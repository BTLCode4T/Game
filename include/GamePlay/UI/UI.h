
#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
// Có thể cần thêm <optional> nếu struct Obstacle dùng nó, nhưng ở đây không cần

// Khai báo struct Obstacle để có thể sử dụng trong các hàm tiện ích
// Đặt định nghĩa struct ở đây nếu nó là một phần của giao diện công khai
// Tạo chướng ngại vật



/*
const std::string &filePath:        đường dẫn ảnh
float width, float height:          kích thước CNV dạng Vector2f
float x, float y:                   vị trí dạng Vector2f
*/
struct Obstacle {
    std::unique_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
    Obstacle(const std::string &filePath, float width, float height, float x, float y);
};


/* Tạo ảnh:
sf::Texture &textureRef :                        biến Texture cần khai báo trước
const std::string &filePath :                    đường dẫn đến ảnh
float desiredWidth, float desiredHeight :        Kích thước dạng Vector2f
float x, float y :                               vị trí ảnh dạng Vector2f
*/
sf::Sprite createSprite(sf::Texture &textureRef, const std::string &filePath, float desiredWidth, float desiredHeight,
                        float x, float y);

/* Tạo text
const sf::Font &fontRef :                   biến font
const std::wstring &content:                text dạng : L"<text cần viết>"
unsigned int size:                          Kích thước (int)
const sf::Color &color:                     Màu sắc
float x, float y:                           vị trí
bool centerAnchor = true                    true: căn gữa / false: căn trái
*/
sf::Text createText(const sf::Font &fontRef, const std::wstring &content, unsigned int size, const sf::Color &color,
                    float x, float y, bool centerAnchor = true);

#endif // UI_H