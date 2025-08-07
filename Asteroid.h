#ifndef ASTEROID_H
#define ASTEROID_H

#include <SFML/Graphics.hpp>

class Asteroid {
public:
    Asteroid(sf::Vector2f position, float speed);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    float speed;
};

#endif
