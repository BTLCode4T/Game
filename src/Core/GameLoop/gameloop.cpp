#include "Core/GameLoop/Game.h"

void Game::run()
{
    cout << "Da chay Game::run()" << endl;

    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - circle.getRadius(), WINDOW_HEIGHT / 2.f - circle.getRadius()));
    while (window.isOpen())
    {
        while (auto eventOpt = window.pollEvent())
        {
            const auto &event = *eventOpt;

            if (event.getIf<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::White);
        window.draw(circle);
        window.display();
    }
}