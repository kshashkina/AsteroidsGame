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

class GameRendering{
private:
    sf::Font font;
public:
    void setFont() {
        if (!font.loadFromFile("C:\\KSE IT\\oop_game\\zh-cn.ttf")) {
            throw std::runtime_error("Failed to load font");
        }
    }
    void addCursor(sf::RenderWindow& window) {
        sf::Image cursorImage;
        if (!cursorImage.loadFromFile("C:\\KSE IT\\sfml\\cursor.png")) {
            throw std::runtime_error("Failed to load texture");
        }

        sf::Cursor cursor;
        if (cursor.loadFromPixels(cursorImage.getPixelsPtr(), cursorImage.getSize(), sf::Vector2u(0, 0))) {
            window.setMouseCursor(cursor);
        } else {
            throw std::runtime_error("Failed to load cursor");
        }
    }

    void scoreText(sf::RenderWindow& window, ScoreManager& scoreManager) {
        int score = scoreManager.getCurrentScore();
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Your score: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);
    }

    void livesText(sf::RenderWindow& window, Spaceship& spaceship) {
        int lives = spaceship.getLives();
        sf::Text livesText;
        livesText.setFont(font);
        livesText.setString("Lives: " + std::to_string(lives));
        livesText.setCharacterSize(24);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(1800, 10);
        window.draw(livesText);
    }
    void renderGame(sf::RenderWindow& window) {
        GameManager gameInterface;
        Spaceship spaceship;
        Asteroid asteroid;
        ScoreManager scoreManager;
        GameRendering manager;

        sf::Texture mainMenuBackgroundTexture, buttonTexture, gameBackgroundTexture;
        if (!mainMenuBackgroundTexture.loadFromFile("C:\\KSE IT\\sfml\\mainpage.jpg") ||
            !buttonTexture.loadFromFile("C:\\KSE IT\\sfml\\startgame.png") ||
            !gameBackgroundTexture.loadFromFile("C:\\KSE IT\\oop_game\\background.jpg")){
            throw std::runtime_error("Failed to load texture");
        }
        sf::Texture gameOverTexture;
        sf::Sprite gameOverSprite;
        if (!gameOverTexture.loadFromFile("C:\\KSE IT\\sfml\\gameover.jpg")) {
            throw std::runtime_error("Failed to load texture");
        }

        sf::RectangleShape playAgainButton;
        playAgainButton.setSize(sf::Vector2f(115.0f, 75.0f)); // размер кнопки
        playAgainButton.setFillColor(sf::Color::Transparent); // цвет кнопки
        playAgainButton.setPosition(815.0f, 840.0f); // позиция кнопки

        sf::RectangleShape exitButton;
        exitButton.setSize(sf::Vector2f(115.0f, 75.0f));
        exitButton.setFillColor(sf::Color::Transparent);
        exitButton.setPosition(1000.0f, 840.0f);

        gameOverSprite.setTexture(gameOverTexture);
        gameOverSprite.setColor(sf::Color(255, 255, 255, 0)); // Изначально спрайт полностью прозрачен
        gameOverSprite.setPosition(0, 0);

        sf::Sprite mainMenuBackground(mainMenuBackgroundTexture);
        sf::Sprite background(gameBackgroundTexture);
        sf::Sprite button(buttonTexture);
        button.setPosition(780.0f, 405.0f);
        sf::Vector2u buttonTextureSize = buttonTexture.getSize();
        float scaleX = 370.0f / buttonTextureSize.x;
        float scaleY = 140.0f / buttonTextureSize.y;
        button.setScale(scaleX, scaleY);

        int highScore = scoreManager.loadHighScore();
        sf::Text highestScore;
        highestScore.setFont(font);
        std::string hexColor = "#25335a";
        unsigned int rgb = std::stoul(hexColor.erase(0, 1), nullptr, 16); // Convert hex to integer
        sf::Color customColor((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
        highestScore.setString("Highest score\n         " + std::to_string(highScore));
        highestScore.setCharacterSize(28);
        highestScore.setFillColor(customColor);
        highestScore.setPosition(865, 735);

        bool gameStarted = false;
        bool inTransition = false;
        float transitionTime = 3.0f;
        float currentTransitionTime = 0.0f;
        float gameOverFadeInTime = 3.0f;
        float gameOverFadeInTimer = 0.0f;
        setFont();
        auto restartGame = [&]() {
            scoreManager.resetScore();
            spaceship.setLives(3);
            asteroid.meteors.clear();
            spaceship.projectiles.clear();
            asteroid.meteorDirections.clear();
            asteroid.meteorSpeeds.clear();
            gameStarted = true;
            gameInterface.gameOver = false;
            asteroid.currentMeteorSpawnTime = 0.0f;
            gameOverFadeInTime = 2.0f;
            gameOverFadeInTimer = 0.0f;
        };

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (!gameStarted && !inTransition && event.type == sf::Event::MouseButtonPressed) {
                    if (button.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                        inTransition = true;
                    }
                }
            }
            if (inTransition) {
                currentTransitionTime += 1.0f / 60;
                if (currentTransitionTime >= transitionTime) {
                    inTransition = false;
                    gameStarted = true;
                    currentTransitionTime = transitionTime;
                }
            }
            float alpha = currentTransitionTime / transitionTime;
            mainMenuBackground.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * (1 - alpha))));
            background.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alpha)));

            window.clear();
            if (!gameStarted || inTransition) {
                window.draw(mainMenuBackground);
                window.draw(highestScore);
                if (!inTransition) {
                    window.draw(button);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                spaceship.shootProjectile();
            }
            if (gameStarted || inTransition){
                spaceship.move(window);
                asteroid.update(window);
                asteroid.generate(window);
                gameInterface.checkCollisions(spaceship, asteroid, scoreManager);
                gameInterface.setTextureBack(spaceship);
                gameInterface.checkProjectileCollisions(spaceship, asteroid, scoreManager);
                spaceship.updateProjectiles(window);

                window.clear();
                window.draw(background);
                scoreText(window, scoreManager);
                livesText(window, spaceship);
                window.draw(spaceship.getSprite());
                asteroid.draw(window);

                for (const auto& projectile : spaceship.projectiles) {
                    window.draw(projectile);
                }
                if (gameInterface.getGameStatus()){
                    gameOverFadeInTimer += 0.5f / 60.0f;
                    float alpha = (gameOverFadeInTimer / gameOverFadeInTime) * 255.0f;
                    if (alpha > 255.0f) alpha = 255.0f;
                    gameOverSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
                    window.clear();
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                        if (playAgainButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            restartGame();
                        }
                        if (exitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            window.close();
                        }
                    }
                    if (scoreManager.getCurrentScore() > highScore) {
                        scoreManager.saveHighScore();
                        highScore = scoreManager.getCurrentScore();
                    }
                }
            }
            if (gameInterface.getGameStatus()) {
                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setString(std::to_string(scoreManager.getCurrentScore()));
                scoreText.setCharacterSize(24);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition(880, 650);

                sf::Text highestScore;
                highestScore.setFont(font);
                highestScore.setString(std::to_string(highScore));
                highestScore.setCharacterSize(24);
                highestScore.setFillColor(sf::Color::White);
                highestScore.setPosition(1025, 650);

                sf::Text newRec;
                newRec.setFont(font);
                newRec.setString("New highest score!");
                newRec.setCharacterSize(24);
                newRec.setFillColor(sf::Color::White);
                newRec.setPosition(850, 680);



                window.draw(playAgainButton);
                window.draw(exitButton);
                window.draw(gameOverSprite);
                window.draw(scoreText);
                window.draw(highestScore);
                if (scoreManager.getCurrentScore() == highScore) {
                    window.draw(newRec);
                }
            }
            window.display();
        }
    }

};

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Asteroid");
    GameRendering gameManager;
    gameManager.addCursor(window);
    gameManager.renderGame(window);
    return 0;
}
