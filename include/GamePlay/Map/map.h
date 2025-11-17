#pragma once
#include "Utils/Constants.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include "GamePlay/Physics/PhysicsSystem.h"

class Map {
private:
    sf::Texture backgroundTexture;
  public:
    void map1(sf::RenderWindow &window, sf::Font &menuFont, sf::Sprite &backgroundSprite, sf::Sprite &backgroundSprite2,
              sf::Sprite &sunSprite, sf::Sprite &treeSprite, sf::RectangleShape &ground, sf::RectangleShape &ground2,
              std::vector<Obstacle> &obstacles);
};