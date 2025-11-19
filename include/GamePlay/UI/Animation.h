#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
class Animation : public sf::Sprite {
  private:
    sf::Vector2i m_frameNum;     // so frame
    sf::Vector2i m_frameCurrent; // frame hien tai

    float m_frameTime;        // tgian giua 2frame
    float m_frameTimeCurrent; // tgian hien tai

    sf::Vector2i m_rectUV;   // toa do tren texture
    sf::Vector2i m_rectSize; // chieu dai,rong cua 1 frame
    sf::IntRect m_rect;      // <-- FIX: Use sf::IntRect instead of sf::Vector2i
  public:
    Animation(sf::Texture &texture, sf::Vector2i frameNum, float frameTime);
    void calculateRectSize(); //= chieu dai,rong chia cho frameNum(dai,rong)
    void calculateRectUV();   // dua vao frameCurrent nhan voi rectSize
    void ApplyRect();         // tinh xong roi chuyen qua frame tiep theo
    void Update(float deltaTime);
    void Reset();
    sf::Vector2i getFrameSize() const {
        return m_rectSize;
    }
};