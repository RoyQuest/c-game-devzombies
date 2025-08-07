//We made it together in class

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Bullet.h"

class Player {
public:
    Player(std::string texturePath, float speed);
    void update(float dt, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    sf::FloatRect getGlobalBounds();
    bool shoot(sf::Vector2f mousePos, std::vector<Bullet>& bullets);

    void takeDamage(int amount);        // 🩸 recibe daño
    int getHealth() const;              // ❤️ consultar vida
    sf::FloatRect getHitboxBounds() const; // 📦 hitbox personalizado

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;

    int direction;
    int currentFrame;
    float timeSinceLastFrame;
    float timePerFrame;

    float timeSinceLastShot = 0.f;
    float shootCooldown = 0.25f;

    static const int frameWidth = 20;
    static const int frameHeight = 64;
    static const int frameCount = 6;

    int health = 50;
    int maxHealth = 50;

    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
    sf::RectangleShape hitbox;

    bool handleInput(float dt);
    void handleAnimation(float dt);
    void updateHealthBar();
};

#endif
