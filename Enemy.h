#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include <vector>
#include <memory>

class Enemy {
public:
    Enemy(const sf::Texture& texture, sf::Vector2f position, float speed, int health);
    virtual ~Enemy() = default;

    virtual void update(float dt, sf::Vector2f playerPos);
    virtual void draw(sf::RenderWindow& window);

    bool checkCollision(const Bullet& bullet);
    bool isDead() const;
    void takeDamage(int amount);
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    sf::FloatRect getHitboxBounds() const;

    void setSpeed(float newSpeed);
    virtual int getDamage() const = 0;
    virtual int getPoints() const = 0;

protected:
    sf::Sprite sprite;
    float speed;
    int health;
    int maxHealth;

    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
    sf::RectangleShape hitbox;

    void updateHealthBar();
};

class EnemyType1 : public Enemy {
public:
    EnemyType1(const sf::Texture& texture, sf::Vector2f position);
    int getDamage() const override { return 2; }
    int getPoints() const override { return 10; }
};

class EnemyType2 : public Enemy {
public:
    EnemyType2(const sf::Texture& texture, sf::Vector2f position);
    int getDamage() const override { return 1; }
    int getPoints() const override { return 25; }
};

class EnemyManager {
public:
    void loadTextures();
    void spawnEnemy(sf::Vector2f position);
    int update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& bullets, bool level3);
    void draw(sf::RenderWindow& window);
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;

private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    sf::Texture texture1;
    sf::Texture texture2;
};

#endif
