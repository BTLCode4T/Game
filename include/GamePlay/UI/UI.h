#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
// #include <optional> không cần

// THÊM INCLUDE NÀY nếu các hàm UI cần dùng Obstacle
// hoặc nếu Obstacle là một kiểu dữ liệu được trả về/nhận vào.
// Tuy nhiên, vì các hàm dưới đây không dùng Obstacle,
// ta có thể KHÔNG CẦN include nếu Obstacle đã được định nghĩa ở nơi khác (game.h).

/* --- CÁC HÀM TIỆN ÍCH UI --- */

/* Tạo ảnh:
sf::Texture &textureRef :                         biến Texture cần khai báo trước
const std::string &filePath :                     đường dẫn đến ảnh
float desiredWidth, float desiredHeight :         Kích thước dạng Vector2f
float x, float y :                                vị trí ảnh dạng Vector2f
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

void drawSpriteBounds(sf::RenderWindow &window, const sf::Sprite &sprite);

#endif // UI_H