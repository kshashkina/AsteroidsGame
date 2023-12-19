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


class Asteroid {
private:
    float meteorSpawnInterval;
    sf::Texture meteorTexture1;
    sf::Texture meteorTexture2;
    sf::Texture meteorTexture3;

public:
    Asteroid() : meteorSpawnInterval(3.5f), currentMeteorSpawnTime(0.0f) {
        if (!meteorTexture1.loadFromFile("C:\\KSE IT\\oop_game\\meteor1.png") ||
            !meteorTexture2.loadFromFile("C:\\KSE IT\\oop_game\\meteor2.png") ||
            !meteorTexture3.loadFromFile("C:\\KSE IT\\oop_game\\meteor3.png")) {
            throw std::runtime_error("Failed to load texture");
        }
    }

    const sf::Texture& getMeteorTexture1() const {
        return meteorTexture1;
    }

    const sf::Texture& getMeteorTexture2() const {
        return meteorTexture2;
    }

    const sf::Texture& getMeteorTexture3() const {
        return meteorTexture3;
    }


    void generate(const sf::RenderWindow& window) {
        currentMeteorSpawnTime += 1.0f / 60;

        if (currentMeteorSpawnTime >= meteorSpawnInterval) {
            sf::Sprite meteor;
            int randomMeteorType = std::rand() % 3 + 1;
            switch (randomMeteorType) {
                case 1:
                    meteor.setTexture(meteorTexture1);
                    break;
                case 2:
                    meteor.setTexture(meteorTexture2);
                    break;
                case 3:
                    meteor.setTexture(meteorTexture3);
                    break;
            }

            float spawnX, spawnY;
            int side = std::rand() % 4;
            switch (side) {
                case 0:
                    spawnX = static_cast<float>(std::rand() % window.getSize().x);
                    spawnY = -static_cast<float>(meteor.getGlobalBounds().height);
                    break;
                case 1:
                    spawnX = static_cast<float>(window.getSize().x);
                    spawnY = static_cast<float>(std::rand() % window.getSize().y);
                    break;
                case 2:
                    spawnX = static_cast<float>(std::rand() % window.getSize().x);
                    spawnY = static_cast<float>(window.getSize().y);
                    break;
                case 3:
                    spawnX = -static_cast<float>(meteor.getGlobalBounds().width);
                    spawnY = static_cast<float>(std::rand() % window.getSize().y);
                    break;
            }

            meteor.setPosition(spawnX, spawnY);

            float directionX = static_cast<float>(std::rand() % 200 - 100);
            float directionY = static_cast<float>(std::rand() % 200 - 100);

            float length = std::sqrt(directionX * directionX + directionY * directionY);
            if (length != 0) {
                directionX /= length;
                directionY /= length;
            }

            float meteorSpeed = static_cast<float>(std::rand() % 1 + 1) / 5.0f;
            meteor.setScale(0.6f, 0.6f);

            meteors.push_back(meteor);
            meteorDirections.push_back(sf::Vector2f(directionX, directionY));
            meteorSpeeds.push_back(meteorSpeed);

            currentMeteorSpawnTime = 0.0f;
        }
    }

    void update(const sf::RenderWindow& window) {
        for (size_t i = 0; i < meteors.size(); ++i) {
            float meteorSpeed = meteorSpeeds[i];
            sf::Vector2f direction = meteorDirections[i];
            meteors[i].move(direction.x * meteorSpeed, direction.y * meteorSpeed);

            if (meteors[i].getPosition().x < -meteors[i].getGlobalBounds().width ||
                meteors[i].getPosition().y < -meteors[i].getGlobalBounds().height ||
                meteors[i].getPosition().x > window.getSize().x ||
                meteors[i].getPosition().y > window.getSize().y) {
                meteors.erase(meteors.begin() + i);
                meteorDirections.erase(meteorDirections.begin() + i);
                meteorSpeeds.erase(meteorSpeeds.begin() + i);
                --i;
            }
        }
    }

    void draw(sf::RenderWindow& window) const {
        for (const auto& meteor : meteors) {
            window.draw(meteor);
        }
    }

    const std::vector<sf::Sprite>& getMeteors() const {
        return meteors;
    }

    void removeMeteor(size_t index) {
        if (index < meteors.size()) {
            meteors.erase(meteors.begin() + index);
            meteorDirections.erase(meteorDirections.begin() + index);
            meteorSpeeds.erase(meteorSpeeds.begin() + index);
        }
    }

    std::vector<sf::Sprite> meteors;
    std::vector<sf::Vector2f> meteorDirections;
    std::vector<float> meteorSpeeds;
    float currentMeteorSpawnTime;
};

class GameManager {
    sf::Clock textureTimer;
public:
    void checkCollisions(Spaceship& spaceship, Asteroid& asteroid, ScoreManager& scoreManager) {
        sf::Sprite shipSprite = spaceship.getSprite();
        sf::FloatRect shipBounds = shipSprite.getGlobalBounds();

        for (size_t i = 0; i < asteroid.getMeteors().size(); ++i) {
            sf::Sprite meteor = asteroid.getMeteors()[i];
            sf::FloatRect meteorBounds = meteor.getGlobalBounds();

            if (shipBounds.intersects(meteorBounds)) {
                spaceship.decreaseLives();
                if (spaceship.getLives() <= 0){
                    gameOver = true;
                    break;
                }
                spaceship.blinkTextureForDuration();
                textureTimer.restart();
                asteroid.removeMeteor(i);
            }
        }
    }

    void setTextureBack(Spaceship& spaceship){
        if (textureTimer.getElapsedTime().asSeconds() >= 0.1f) {
            spaceship.blinkReversed();
        }
    }

    bool getGameStatus(){
        return gameOver;
    }

    void checkProjectileCollisions(Spaceship& spaceship, Asteroid& asteroid, ScoreManager& scoreManager) {
        for (size_t i = 0; i < spaceship.projectiles.size(); ) {
            const sf::Sprite& projectile = spaceship.projectiles[i];
            sf::FloatRect projectileBounds = projectile.getGlobalBounds();

            bool hit = false;
            for (size_t j = 0; j < asteroid.getMeteors().size() && !hit; ++j) {
                sf::Sprite& meteor = const_cast<sf::Sprite &>(asteroid.getMeteors()[j]);
                sf::FloatRect meteorBounds = meteor.getGlobalBounds();

                if (projectileBounds.intersects(meteorBounds)) {
                    const sf::Texture* tex = meteor.getTexture();
                    if (tex == &asteroid.getMeteorTexture1()) {
                        meteor.setTexture(asteroid.getMeteorTexture2(), true);
                        scoreManager.setScore(3);
                    } else if (tex == &asteroid.getMeteorTexture2()) {
                        meteor.setTexture(asteroid.getMeteorTexture3(), true);
                        scoreManager.setScore(2);
                    } else if (tex == &asteroid.getMeteorTexture3()) {
                        asteroid.removeMeteor(j);
                        scoreManager.setScore(1);
                        hit = true;
                    }

                    spaceship.projectiles.erase(spaceship.projectiles.begin() + i);
                }
            }

            if (!hit) {
                ++i;
            }
        }
    }

    bool gameOver = false;
};
