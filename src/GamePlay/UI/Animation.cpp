#include "GamePlay/UI/Animation.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>

Animation::Animation(sf::Texture &texture, sf::Vector2i frameNum, float frameTime)
    : sf::Sprite(texture) // Call sf::Sprite's constructor with texture
{
    m_frameNum = frameNum;
    m_frameTime = frameTime;
    m_frameCurrent = sf::Vector2i(0, 0);
    m_frameTimeCurrent = 0.0f;
    calculateRectSize();
    calculateRectUV();
    ApplyRect();
}
void Animation::calculateRectSize() {
    m_rectSize =
        sf::Vector2i(this->getTexture().getSize().x / m_frameNum.x, this->getTexture().getSize().y / m_frameNum.y);
}

void Animation::calculateRectUV() {
    m_rectUV.x = m_frameCurrent.x * m_rectSize.x;
    m_rectUV.y = m_frameCurrent.y * m_rectSize.y;
}

void Animation::ApplyRect() {
    m_rect = sf::IntRect{m_rectUV, m_rectSize};
    this->setTextureRect(m_rect);
}

void Animation::Update(float deltaTime) {
    m_frameTimeCurrent += deltaTime;
    if (m_frameTimeCurrent >= m_frameTime) {
        m_frameCurrent.x++;
        if (m_frameCurrent.x == m_frameNum.x) {
            m_frameCurrent.x = 0;
        }
        calculateRectUV();
        ApplyRect();
        m_frameTimeCurrent -= m_frameTime;
    }
}
