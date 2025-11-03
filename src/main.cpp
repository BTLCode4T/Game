#include <SFML/Graphics.hpp>
#include <optional>

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 600;

int main()
{
    // SFML 3.x: chỉ dùng VideoMode và Style
  sf::RenderWindow window(
    sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)),
    "SFML 3.x - Window with Circle",
    sf::Style::Titlebar | sf::Style::Close
);

    window.setFramerateLimit(60);

    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(sf::Vector2f(
        WINDOW_WIDTH / 2.f - circle.getRadius(),
        WINDOW_HEIGHT / 2.f - circle.getRadius()
    ));

    while (window.isOpen())
    {
        while (auto eventOpt = window.pollEvent())
        {
            const auto& event = *eventOpt;

            if (event.getIf<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(circle);
        window.display();
    }

    return 0;
}
