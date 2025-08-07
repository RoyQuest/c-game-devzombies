// Made by sebastian Rodriguez

#include "Asteroid.h"

Asteroid::Asteroid(sf::Vector2f position, float speed)
    : speed(speed) {
    shape.setSize(sf::Vector2f(20.f, 40.f));
    shape.setFillColor(sf::Color(100, 100, 100)); 
    shape.setOrigin(shape.getSize() / 4.f);
    shape.setPosition(position);
}

void Asteroid::update(float dt) {
    shape.move(0.f, speed * dt);
}

void Asteroid::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Asteroid::getBounds() const {
    return shape.getGlobalBounds();
}
