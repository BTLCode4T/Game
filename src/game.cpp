#include "Core/GameLoop/Game.h"



int main()
{
    Game game;
    game.run();
    return 0;
}
Game::Game() : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "SFML 3.x - Window with Circle", sf::Style::Default)
{
    cout<<"Da khoi tao Game::Game()"<<endl;
    // SFML 3.x: chỉ dùng VideoMode và Style

    window.setFramerateLimit(60);

    window.setMinimumSize(sf::Vector2u(1000, 600));

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.size.x;
    unsigned int screenHeight = desktop.size.y;

    window.setMaximumSize(sf::Vector2u(screenWidth, screenHeight));


    return;
}
