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
