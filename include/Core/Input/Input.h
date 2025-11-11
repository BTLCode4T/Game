
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp> 
#include <unordered_map>

class InputManager {
  public:
    // Khởi tạo
    InputManager();

    // input
    void ProcessEvent(const sf::Event &event);

    // Reset
    void Update();

    // phím
    bool IsKeyDown(sf::Keyboard::Scancode key) const;     // Giữ phím
    bool IsKeyPressed(sf::Keyboard::Scancode key) const;  // Nhấn 1 lần
    bool IsKeyReleased(sf::Keyboard::Scancode key) const; // Nhả ra

    // chuột
    bool IsMouseDown(sf::Mouse::Button btn) const;
    bool IsMousePressed(sf::Mouse::Button btn) const;
    bool IsMouseReleased(sf::Mouse::Button btn) const;

    sf::Vector2i GetMousePosition() const;
    // kiểm tra trái phải
    bool IsLeftMouse() const;
    bool IsLeftMousePressed() const;
    bool IsLeftMouseReleased() const;
    bool IsRightMouse() const;
    bool IsRightMousePressed() const;
    bool IsRightMouseReleased() const;

  private:
    std::unordered_map<sf::Keyboard::Scancode, bool> keyDown;
    std::unordered_map<sf::Keyboard::Scancode, bool> keyPressed;
    std::unordered_map<sf::Keyboard::Scancode, bool> keyReleased;

    std::unordered_map<sf::Mouse::Button, bool> mouseDown;
    std::unordered_map<sf::Mouse::Button, bool> mousePressed;
    std::unordered_map<sf::Mouse::Button, bool> mouseReleased;

    sf::Vector2i mousePos;
};
