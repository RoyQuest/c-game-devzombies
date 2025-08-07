#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Map.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

const float VIEW_WIDTH = 800.f;
const float VIEW_HEIGHT = 600.f;

struct Asteroid {
    sf::CircleShape shape;
    float speed;
};

int main() {
    sf::RenderWindow window(sf::VideoMode((unsigned int)VIEW_WIDTH, (unsigned int)VIEW_HEIGHT), "Zombie Game");
    window.setFramerateLimit(60);

    Player player("Assets/Sprites/player5.png", 200.f);
    Map map("Assets/Sprites/floor3.png", 0, 0);
    std::vector<Bullet> bullets;
    EnemyManager enemyManager;
    enemyManager.loadTextures();

    std::vector<Asteroid> asteroids;
    bool asteroidsEnabled = false;
    bool showAsteroidMessage = false;
    sf::Clock asteroidMessageClock;

    float asteroidTimer = 0.f;
    float asteroidInterval = 1.5f;

    int score = 0;
    bool inTutorial = true;
    bool isPaused = false;
    bool level3 = false;
    bool showLevel3Msg = false;
    sf::Clock level3MsgClock;

    // === Font and Text ===
    sf::Font font;
    if (!font.loadFromFile("Assets/Fonts/arial.ttf")) {
        std::cerr << "❌ Error loading font!\n";
    }

    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    sf::Text asteroidWarning("ASTEROIDS INCOMING!", font, 36);
    asteroidWarning.setFillColor(sf::Color::Red);
    asteroidWarning.setStyle(sf::Text::Bold);
    asteroidWarning.setOrigin(asteroidWarning.getLocalBounds().width / 2.f, asteroidWarning.getLocalBounds().height / 2.f);
    asteroidWarning.setPosition(VIEW_WIDTH / 2.f, 60.f);

    sf::Text level3Text("The enemies are faster now!!", font, 28);
    level3Text.setFillColor(sf::Color::Magenta);
    level3Text.setStyle(sf::Text::Bold);
    level3Text.setOrigin(level3Text.getLocalBounds().width / 2.f, level3Text.getLocalBounds().height / 2.f);
    level3Text.setPosition(VIEW_WIDTH / 2.f, 100.f);

    sf::Text pausedText("PAUSED", font, 40);
    pausedText.setFillColor(sf::Color::Yellow);
    pausedText.setStyle(sf::Text::Bold);
    pausedText.setOrigin(pausedText.getLocalBounds().width / 2.f, pausedText.getLocalBounds().height / 2.f);
    pausedText.setPosition(VIEW_WIDTH / 2.f, VIEW_HEIGHT / 2.f);

    // === Music ===
    sf::Music music;
    if (!music.openFromFile("Assets/Audio/soundtrack.ogg")) {
        std::cerr << "❌ Error loading soundtrack.ogg\n";
    }
    else {
        music.setLoop(true);
        music.setVolume(60.f);
        music.play();
    }

    // === Tutorial Dialog ===
    sf::Text npcDialog("Hello, survivor. I'm Commander B.\n\nUse WASD to move.\nLeft click to shoot.\nPress P to pause.\nAvoid zombies and asteroids.\n\nPress ENTER to begin.", font, 22);
    npcDialog.setFillColor(sf::Color::White);
    npcDialog.setPosition(80.f, 140.f);

    sf::RectangleShape dialogBox(sf::Vector2f(VIEW_WIDTH - 160.f, 220.f));
    dialogBox.setPosition(70.f, 130.f);
    dialogBox.setFillColor(sf::Color(30, 30, 30, 220));
    dialogBox.setOutlineColor(sf::Color::White);
    dialogBox.setOutlineThickness(2.f);

    sf::Text npcName("NPC: Commander B", font, 20);
    npcName.setFillColor(sf::Color::Cyan);
    npcName.setPosition(80.f, 100.f);

    sf::Text continueText("Press ENTER to continue...", font, 18);
    continueText.setFillColor(sf::Color(150, 150, 150));
    continueText.setPosition(VIEW_WIDTH / 2.f - 140.f, 380.f);

    sf::Texture npcTexture;
    sf::Sprite npcSprite;
    if (npcTexture.loadFromFile("Assets/Sprites/brim.png")) {
        npcSprite.setTexture(npcTexture);
        npcSprite.setScale(0.25f, 0.25f);
        npcSprite.setPosition(dialogBox.getPosition().x + dialogBox.getSize().x - 140.f, dialogBox.getPosition().y + 10.f);
    }

    // === Tutorial Loop ===
    while (inTutorial && window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
                inTutorial = false;
        }

        window.clear(sf::Color::Black);
        window.draw(dialogBox);
        window.draw(npcName);
        window.draw(npcDialog);
        window.draw(continueText);
        window.draw(npcSprite);
        window.display();
    }

    // === Game Over Setup ===
    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("Assets/Sprites/gameover.png")) {
        std::cerr << "❌ Error loading Game Over image!\n";
    }
    sf::Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setOrigin(gameOverTexture.getSize().x / 2.f, gameOverTexture.getSize().y / 2.f);
    gameOverSprite.setPosition(VIEW_WIDTH / 2.f, VIEW_HEIGHT / 2.f);

    sf::View view(sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT));
    sf::Clock clock;
    float spawnTimer = 0.f;
    const float spawnInterval = 2.0f;

    // === Main Game Loop ===
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
                isPaused = !isPaused;
        }

        if (player.getHealth() <= 0) {
            music.stop();
            sf::View staticView(sf::FloatRect(0, 0, VIEW_WIDTH, VIEW_HEIGHT));
            staticView.setCenter(VIEW_WIDTH / 2.f, VIEW_HEIGHT / 2.f);
            window.setView(staticView);
            window.clear(sf::Color::Black);
            window.draw(gameOverSprite);
            window.display();

            while (window.isOpen()) {
                sf::Event e;
                while (window.pollEvent(e)) {
                    if (e.type == sf::Event::Closed ||
                        (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape))
                        window.close();
                }
            }
            break;
        }

        // === Updates ===
        if (!isPaused) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            player.update(dt, mousePos);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                player.shoot(mousePos, bullets);

            spawnTimer += dt;
            if (spawnTimer >= spawnInterval) {
                enemyManager.spawnEnemy(player.getPosition());
                spawnTimer = 0.f;
            }

            for (auto& bullet : bullets)
                bullet.update(dt);

            if (!level3 && score >= 200) {
                level3 = true;
                showLevel3Msg = true;
                level3MsgClock.restart();
            }

            int kills = enemyManager.update(dt, player.getPosition(), bullets, level3);
            score += kills;

            if (!asteroidsEnabled && score >= 100 && !level3) {
                asteroidsEnabled = true;
                showAsteroidMessage = true;
                asteroidMessageClock.restart();
            }

            if (asteroidsEnabled) {
                asteroidTimer += dt;
                if (asteroidTimer >= asteroidInterval) {
                    Asteroid a;
                    a.shape.setRadius(12.f);
                    a.shape.setFillColor(sf::Color(150, 150, 150));
                    float x = static_cast<float>(std::rand() % static_cast<int>(VIEW_WIDTH));
                    a.shape.setPosition(player.getPosition().x - VIEW_WIDTH / 2.f + x, player.getPosition().y - VIEW_HEIGHT / 2.f - 30.f);
                    a.speed = 150.f + std::rand() % 100;
                    asteroids.push_back(a);
                    asteroidTimer = 0.f;
                }

                for (auto& a : asteroids)
                    a.shape.move(0.f, a.speed * dt);

                for (auto& a : asteroids)
                    if (a.shape.getGlobalBounds().intersects(player.getGlobalBounds()))
                        player.takeDamage(10);

                asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), [&](const Asteroid& a) {
                    return a.shape.getPosition().y > player.getPosition().y + VIEW_HEIGHT / 2.f + 50.f;
                    }), asteroids.end());
            }

            for (const auto& enemy : enemyManager.getEnemies())
                if (enemy->getHitboxBounds().intersects(player.getGlobalBounds()))
                    player.takeDamage(enemy->getDamage());
        }

        // === Draw ===
        view.setCenter(player.getPosition());
        window.setView(view);
        window.clear();
        map.draw(window, player.getPosition());
        player.draw(window);
        for (auto& bullet : bullets) bullet.draw(window);
        enemyManager.draw(window);
        for (auto& a : asteroids) window.draw(a.shape);

        // HUD
        window.setView(window.getDefaultView());
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        if (showAsteroidMessage && asteroidMessageClock.getElapsedTime().asSeconds() < 3.f)
            window.draw(asteroidWarning);

        if (showLevel3Msg && level3MsgClock.getElapsedTime().asSeconds() < 3.f)
            window.draw(level3Text);

        if (isPaused)
            window.draw(pausedText);

        window.display();
    }

    return 0;
}
