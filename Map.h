// === Map.h ===
#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Map {
public:
    Map(std::string tileSheet, int row, int column);
    Map();
    void draw(sf::RenderWindow& window, sf::Vector2f playerPos);

private:
    sf::Texture texture;
    int row;
    int column;
};

#endif
