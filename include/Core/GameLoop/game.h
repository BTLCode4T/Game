#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

using namespace std;
const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 600;
class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::CircleShape circle;
};

