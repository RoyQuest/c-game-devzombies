//We made it together in class

#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

Player::Player(std::string texturePath, float speed)
    : speed(speed), direction(1), currentFrame(0), timeSinceLastFrame(0.f), timePerFrame(0.1f), timeSinceLastShot(0.f)
{
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Error loading texture from: " << texturePath << std::endl;
    }
    else {
        std::cout << "Texture loaded: " << texture.getSize().x << "x" << texture.getSize().y << std::endl;
        texture.setSmooth(false);
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(400.f, 300.f);
    sprite.setScale(1.5f, 1.5f);

    healthBarBack.setSize(sf::Vector2f(100.f, 12.f));
    healthBarBack.setFillColor(sf::Color(60, 60, 60));
    healthBarBack.setOrigin(50.f, 40.f);

    healthBarFront.setSize(sf::Vector2f(100.f, 12.f));
    healthBarFront.setFillColor(sf::Color::Green);
    healthBarFront.setOrigin(50.f, 40.f);

    hitbox.setSize(sf::Vector2f(20.f, 50.f));
    hitbox.setOrigin(10.f, 25.f);
    hitbox.setFillColor(sf::Color::Transparent);
}

sf::Vector2f Player::getPosition() {
    return sprite.getPosition();
}

sf::FloatRect Player::getGlobalBounds() {
    return sprite.getGlobalBounds();
}

sf::FloatRect Player::getHitboxBounds() const {
    return hitbox.getGlobalBounds();
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(healthBarBack);
    window.draw(healthBarFront);
    window.draw(hitbox);
}

void Player::update(float dt, sf::Vector2f mousePos) {
    timeSinceLastShot += dt;
    bool isMoving = handleInput(dt);
    if (isMoving)
        handleAnimation(dt);
    else
        currentFrame = 0;

    updateHealthBar();
    hitbox.setPosition(sprite.getPosition());
}

bool Player::handleInput(float dt) {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction = -1;
        movement.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction = 1;
        movement.x += 1.f;
    }

    float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length > 0.f) movement /= length;

    sprite.move(movement * speed * dt);
    return (movement.x != 0.f || movement.y != 0.f);
}

void Player::handleAnimation(float dt) {
    timeSinceLastFrame += dt;

    if (timeSinceLastFrame >= timePerFrame) {
        currentFrame = (currentFrame + 1) % frameCount;
        int textureX = currentFrame * frameWidth;
        if (textureX + frameWidth > texture.getSize().x) {
            currentFrame = 0;
            textureX = 0;
        }
        sprite.setTextureRect(sf::IntRect(textureX, 0, frameWidth, frameHeight));
        sprite.setScale(1.5f * direction, 1.5f);
        timeSinceLastFrame = 0.f;
    }
}

bool Player::shoot(sf::Vector2f mousePos, std::vector<Bullet>& bullets) {
    if (timeSinceLastShot >= shootCooldown) {
        sf::Vector2f direction = mousePos - getPosition();
        bullets.emplace_back(getPosition(), direction, 600.f);
        timeSinceLastShot = 0.f;
        return true;
    }
    return false;
}

void Player::updateHealthBar() {
    float healthPercent = static_cast<float>(health) / maxHealth;
    healthBarFront.setSize(sf::Vector2f(100.f * healthPercent, 12.f));

    sf::Vector2f barPos = sprite.getPosition();
    barPos.y -= 60.f;
    healthBarBack.setPosition(barPos);
    healthBarFront.setPosition(barPos);
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    updateHealthBar();
}

int Player::getHealth() const {
    return health;
}
