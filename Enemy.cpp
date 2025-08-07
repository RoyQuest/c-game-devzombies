#include "Enemy.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Enemy::Enemy(const sf::Texture& texture, sf::Vector2f position, float speed, int health)
    : speed(speed), health(health), maxHealth(health) {
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 48, 80));
    sprite.setPosition(position);
    sprite.setOrigin(24.f, 24.f);
    sprite.setScale(1.5f, 1.5f);

    healthBarBack.setSize(sf::Vector2f(48.f, 6.f));
    healthBarBack.setFillColor(sf::Color(100, 100, 100));
    healthBarBack.setOrigin(24.f, 60.f);

    healthBarFront.setSize(sf::Vector2f(48.f, 6.f));
    healthBarFront.setFillColor(sf::Color::Red);
    healthBarFront.setOrigin(24.f, 60.f);

    hitbox.setSize(sf::Vector2f(30.f, 70.f));
    hitbox.setOrigin(15.f, 35.f);
    hitbox.setFillColor(sf::Color::Transparent);

    updateHealthBar();
}

void Enemy::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f direction = playerPos - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.f) direction /= length;
    sprite.move(direction * speed * dt);
    updateHealthBar();
    hitbox.setPosition(sprite.getPosition());
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(healthBarBack);
    window.draw(healthBarFront);
}

bool Enemy::checkCollision(const Bullet& bullet) {
    return hitbox.getGlobalBounds().intersects(bullet.getBounds());
}

bool Enemy::isDead() const {
    return health <= 0;
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    updateHealthBar();
}

sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::FloatRect Enemy::getHitboxBounds() const {
    return hitbox.getGlobalBounds();
}

void Enemy::updateHealthBar() {
    float healthPercent = static_cast<float>(health) / maxHealth;
    healthBarFront.setSize(sf::Vector2f(48.f * healthPercent, 6.f));
    healthBarBack.setPosition(sprite.getPosition());
    healthBarFront.setPosition(sprite.getPosition());
}

void Enemy::setSpeed(float newSpeed) {
    speed = newSpeed;
}

EnemyType1::EnemyType1(const sf::Texture& texture, sf::Vector2f position)
    : Enemy(texture, position, 50.f, 10) {
    sprite.setColor(sf::Color(200, 255, 200));
}

EnemyType2::EnemyType2(const sf::Texture& texture, sf::Vector2f position)
    : Enemy(texture, position, 200.f, 5) {
    sprite.setColor(sf::Color(255, 180, 180));
}

void EnemyManager::loadTextures() {
    if (!texture1.loadFromFile("Assets/Sprites/zombie3.png"))
        std::cerr << "Failed to load zombie1 texture\n";
    if (!texture2.loadFromFile("Assets/Sprites/zombie3.png"))
        std::cerr << "Failed to load zombie2 texture\n";
}

void EnemyManager::spawnEnemy(sf::Vector2f playerPos) {
    const float MIN_DISTANCE_TO_PLAYER = 250.f;
    sf::Vector2f spawnPos;
    bool valid = false;

    for (int attempt = 0; attempt < 100 && !valid; ++attempt) {
        float x = static_cast<float>(std::rand() % 1600 - 800);
        float y = static_cast<float>(std::rand() % 1600 - 800);
        spawnPos = playerPos + sf::Vector2f(x, y);
        float dist = std::sqrt(x * x + y * y);
        if (dist >= MIN_DISTANCE_TO_PLAYER) valid = true;
    }

    if (!valid)
        spawnPos = playerPos + sf::Vector2f(300.f, 0.f);

    if (std::rand() % 2 == 0)
        enemies.push_back(std::make_unique<EnemyType1>(texture1, spawnPos));
    else
        enemies.push_back(std::make_unique<EnemyType2>(texture2, spawnPos));
}

int EnemyManager::update(float dt, sf::Vector2f playerPos, std::vector<Bullet>& bullets, bool level3) {
    int totalPoints = 0;

    float multiplier = level3 ? 2.f : 1.f;

    for (auto& enemy : enemies) {
        if (enemy->getDamage() == 2)
            enemy->setSpeed(50.f * multiplier); // EnemyType1
        else
            enemy->setSpeed(200.f * multiplier); // EnemyType2

        enemy->update(dt, playerPos);

        for (auto& bullet : bullets) {
            if (enemy->checkCollision(bullet)) {
                enemy->takeDamage(1);
            }
        }
    }

    auto it = enemies.begin();
    while (it != enemies.end()) {
        if ((*it)->isDead()) {
            totalPoints += (*it)->getPoints();
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }

    return totalPoints;
}

const std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemies() const {
    return enemies;
}

void EnemyManager::draw(sf::RenderWindow& window) {
    for (auto& enemy : enemies)
        enemy->draw(window);
}
