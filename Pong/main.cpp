#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "properties.h"
#include <thread>
#include <sstream>

int score = 0;
int lives = -1;


class Platform
{
public:
    Platform(int x, int y)
    {
        platPosition.x = x;
        platPosition.y = y;

        platfObject.setSize(sf::Vector2f(widthPlatform, heightPlatform));
        platfObject.setPosition(platPosition);
    }

    sf::FloatRect getPlatFloatRect()
    {
        return platfObject.getGlobalBounds();
    }

    sf::Vector2f getPlatPosition()
    {
        return platPosition;
    }

    sf::RectangleShape getPlatObject()
    {
        return platfObject;
    }

    void update()
    {
        platfObject.setPosition(platPosition);
    }
    
    void movePlatform(int direction)
    {
        if (direction == 1) {
            platPosition.x += platSpeed;
            if (platPosition.x + widthPlatform > WIDTH) platPosition.x = WIDTH - widthPlatform;
        }
        else if (direction == -1) {
            platPosition.x -= platSpeed;
            if (platPosition.x < 0) platPosition.x = 0;
        }
    }
    
private:
    sf::RectangleShape platfObject;
    sf::Vector2f platPosition;
    int platSpeed = 4;
};

class Ball
{
public:
    Ball(int x, int y)
    {
        ballPosition.x = x;
        ballPosition.y = y;
        
        //ballObject.setSize(sf::Vector2f(40, 40));
        ballObject.setRadius(RADIUS);
        ballObject.setPosition(ballPosition);
        ballObject.setFillColor(sf::Color::Green);
    }

    sf::CircleShape getBallObject() 
    {
        return ballObject;
    }

    sf::FloatRect getBallFloatRect()
    {
        return ballObject.getGlobalBounds();
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(ballObject);
    }

    void rebound()
    { 
        ballPosition.y -= (ballVelocityY * 1);
        ballVelocityY *= -1;
        ballVelocityY += -0.25; // Up the velocity
        ballVelocityX *= rand() % 2 == 0 ? 1 : -1;
        // std::cout << ballVelocityX << std::endl;
        // std::cout << ballVelocityY << std::endl;
        score++;
    }

    void update()
    {
        ballPosition.x += ballVelocityX;
        ballPosition.y += ballVelocityY;
            
        ballObject.setPosition(ballPosition);
    }

    void stop()
    {
        ballVelocityX = 0;
        ballVelocityY = 0;
    }

    void start()
    {
        ballVelocityX = 3;
        ballVelocityY = 3;
    }

    void logic()
    {
        // Ball collision with horizontal walls
        if (ballPosition.x + 2 * RADIUS >= WIDTH || ballPosition.x <= 0) {
            ballVelocityX *= -1;
        }
        if (ballPosition.y + 2 * RADIUS >= HEIGHT || ballPosition.y <= 0) {
            ballVelocityY *= -1;
        }
        // std::cout << ballPosition.y << std::endl;
        // Ball collision with down wall
        if (ballPosition.y + 2*RADIUS >= HEIGHT)
        {
            end = true;
            ballPosition.x = WIDTH / 2;
            ballPosition.y = HEIGHT / 2;

            if (rand() % 2 == 1) ballVelocityY *= -1;
            if (rand() % 2 == 2) ballVelocityX *= -1;
            lives--;
        }
    }
private:
    //sf::RectangleShape ballObject;
    sf::CircleShape ballObject;
    sf::Vector2f ballPosition;
    double ballVelocityX = 0;
    double ballVelocityY = 0;
    bool end = false;
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ponk");
    
    // creating objects
    Ball ball(100, 100);
    Platform platform(300, 540);
    int direction = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                direction = -1;
                // platform.moveLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                direction = 1;
                // platform.moveRight();
            }
        }
        if (ball.getBallFloatRect().intersects(platform.getPlatFloatRect()))
            ball.rebound();

        // Load the sprite to display
        sf::Texture texture;
        if (!texture.loadFromFile("Texture.jpg"))
            return EXIT_FAILURE;
        sf::Sprite sprite(texture);

        //! Score message
        std::stringstream ss;
        ss << "Score: " << score;
        sf::Text scoreMessage;
        sf::Font fontScore;
        fontScore.loadFromFile("Nexa Light.otf");
        scoreMessage.setFont(fontScore);
        scoreMessage.setCharacterSize(30);
        scoreMessage.setFillColor(sf::Color::White);
        scoreMessage.setString(ss.str());

        // Start message
        std::stringstream sm;
        sm << "           Welcome to the game.\n Press the button to start the game.";
        sf::Text startMessage;
        sf::Font fontStart;
        fontStart.loadFromFile("Nexa Light.otf");
        startMessage.setFont(fontStart);
        startMessage.setCharacterSize(30);
        startMessage.setFillColor(sf::Color::White);
        startMessage.setPosition(WIDTH / 2 - 230, HEIGHT / 2 - 55);
        startMessage.setString(sm.str());

        // End message
        std::stringstream pm;
        pm << "Your score is " << score;
        sf::Text endMessage;
        sf::Font fontp;
        fontp.loadFromFile("Nexa Light.otf");
        endMessage.setFont(fontp);
        endMessage.setCharacterSize(30);
        endMessage.setFillColor(sf::Color::White);
        endMessage.setPosition(WIDTH / 2 - 110, HEIGHT / 2 - 50);
        endMessage.setString(pm.str());

        ball.logic();
        ball.update();

        platform.movePlatform(direction);
        platform.update();

        window.clear();
        
        if (lives == -1) {
            ball.stop();
            window.draw(sprite);
            window.draw(startMessage);
            if (event.type == event.KeyPressed) {
                lives = 1;
                score = 0;
                ball.start();
                window.clear();
                window.draw(sprite);
            }
        }
        else if (lives == 0) {
            window.clear();
            window.draw(sprite);
            window.draw(endMessage);
            ball.stop();
            if (event.type == event.KeyPressed) {
                lives = 1;
                score = 0;
                ball.start();
            }
        }
        else {
            window.draw(sprite);
            window.draw(ball.getBallObject());
            window.draw(platform.getPlatObject());
            window.draw(scoreMessage);
        }
        window.display();
    }
    return 0;
}