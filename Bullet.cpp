// Made by Roy Quest

#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f position, sf::Vector2f direction, float speed) {
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(5.f, 5.f);
    shape.setPosition(position);

    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len != 0.f)
        direction /= len;

    velocity = direction * speed;
}

void Bullet::update(float dt) {
    shape.move(velocity * dt);
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}
