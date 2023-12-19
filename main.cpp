#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

class Spaceship {
private:
    int lives;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Texture hittedTexture;
    sf::Texture projectileTexture;
    bool textureChanged;
    sf::Clock textureTimer;
    sf::Clock projectileClock;
    float currentRotation;
    const float rotationSmoothing;
    sf::Vector2i lastMousePosition;

public:
    Spaceship() : lives(3), currentRotation(0.0f), rotationSmoothing(0.05f), textureChanged(false) {
        if (!texture.loadFromFile("C:\\KSE IT\\oop_game\\spacecraft.png")) {
            throw std::runtime_error("Failed to load texture");
        }
        if (!hittedTexture.loadFromFile("C:\\KSE IT\\oop_game\\spacecraftHitted.png")) {
            throw std::runtime_error("Failed to load hitted texture");
        }
        if (!projectileTexture.loadFromFile("C:\\KSE IT\\oop_game\\ball.png")){
            throw std::runtime_error("Failed to load projectile texture");
        }
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        sprite.setScale(0.10f, 0.10f);
        lastMousePosition = sf::Mouse::getPosition();
    }

    void move(const sf::RenderWindow& window) {
        sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
        sprite.setPosition(static_cast<float>(currentMousePosition.x), static_cast<float>(currentMousePosition.y));

        sf::Vector2f direction = static_cast<sf::Vector2f>(currentMousePosition - lastMousePosition);
        if (direction.x != 0 || direction.y != 0) {
            float targetAngle = std::atan2(direction.y, direction.x) * 180 / 3.14159f + 90;
            float angleDifference = targetAngle - currentRotation;
            angleDifference -= std::floor((angleDifference + 180) / 360) * 360;
            currentRotation += angleDifference * rotationSmoothing;
            sprite.setRotation(currentRotation);
        }
        lastMousePosition = currentMousePosition;
    }

    void blinkTextureForDuration() {
        if (!textureChanged) {
            textureChanged = true;
            sprite.setTexture(hittedTexture);
        }
    }

    void blinkReversed(){
        if (textureChanged){
            sprite.setTexture(texture);
            textureChanged = false;
        }
    }

    std::vector<sf::Sprite> projectiles;

    void shootProjectile() {
        if (projectileClock.getElapsedTime().asSeconds() >= 0.3f) {
            sf::Sprite projectile(projectileTexture);

            projectile.setScale(0.08f, 0.08f);

            float scaleFactor = 0.1f;
            sf::Vector2f noseOffset(std::sin(currentRotation * 3.14159f / 180) * sprite.getGlobalBounds().height * scaleFactor,
                                    -std::cos(currentRotation * 3.14159f / 180) * sprite.getGlobalBounds().height * scaleFactor);
            projectile.setPosition(sprite.getPosition() + noseOffset);
            projectile.setRotation(currentRotation);

            projectiles.push_back(projectile);
            projectileClock.restart();
        }
    }

    void updateProjectiles(const sf::RenderWindow& window) {
        for (auto& projectile : projectiles) {
            float angle = projectile.getRotation();
            float speed = 2.0f;
            projectile.move(std::sin(angle * 3.14159f / 180) * speed, -std::cos(angle * 3.14159f / 180) * speed);
        }

        projectiles.erase(
                std::remove_if(
                        projectiles.begin(),
                        projectiles.end(),
                        [&window](const sf::Sprite& projectile) {
                            return projectile.getPosition().y < -projectile.getGlobalBounds().height ||
                                   projectile.getPosition().y > window.getSize().y ||
                                   projectile.getPosition().x < -projectile.getGlobalBounds().width ||
                                   projectile.getPosition().x > window.getSize().x;
                        }
                ),
                projectiles.end()
        );
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    int getLives() const{
        return lives;
    }

    void setLives(int value){
        lives = value;
    }

    void decreaseLives() {
        --lives;
    }
};


class ScoreManager {
private:
    int currentScore;
    std::string filePath;
    int highScore;

public:
    ScoreManager() : currentScore(0), filePath("C:\\KSE IT\\oop_game\\high_score.txt") {}

    void saveHighScore() {
        std::ofstream file(filePath);
        if (file.is_open()) {
            file << currentScore;
            file.close();
        }
    }

    int loadHighScore() {
        std::ifstream file(filePath);
        int loadedHighScore = 0;
        if (file.is_open()) {
            file >> loadedHighScore;
            file.close();
        }
        return loadedHighScore;
    }

    void setScore(int value) {
        currentScore += value;
    }

    void resetScore(){
        currentScore = 0;
    }

    int getCurrentScore(){
        return currentScore;
    }
};