#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "properties.h"
#include <thread>
#include <sstream>

int score = 0;


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

    void moveLeft() 
    {
        platPosition.x -= platSpeed;
        if (platPosition.x < 0) platPosition.x = 0;
    }

    void moveRight() 
    {
        platPosition.x += platSpeed;
        if (platPosition.x + widthPlatform > WIDTH) platPosition.x = WIDTH - widthPlatform;
    }

    void moveUp()
    {
        platPosition.y -= platSpeed;
        if (platPosition.y < 0) platPosition.y = 0;
    }

    void moveDown()
    {
        platPosition.y += 4;
        if (platPosition.y + heightPlatform > HEIGHT) platPosition.y = HEIGHT - heightPlatform;
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

        ballObject.setSize(sf::Vector2f(40, 40));
        ballObject.setPosition(ballPosition);
        ballObject.setFillColor(sf::Color::Green);
    }
    
    bool endTheGame()
    {
        return end;
    }

    sf::RectangleShape getBallObject() {
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
    }

    void update()
    {
        ballPosition.x += ballVelocityX;
        ballPosition.y += ballVelocityY;
            
        ballObject.setPosition(ballPosition);
    }

    void logic()
    {
        // Ball collision with horizontal walls
        if (ballPosition.x + 2 * RADIUS == WIDTH || ballPosition.x == 0)
            ballVelocityX *= -1;
        if (ballPosition.y + 2 * RADIUS == HEIGHT || ballPosition.y == 0)
            ballVelocityY *= -1;
        std::cout << ballPosition.y << std::endl;
        // Ball collision with down wall
        if (ballPosition.y + 2*RADIUS >= HEIGHT)
        {
            end = true;
            ballPosition.x = WIDTH / 2;
            ballPosition.y = HEIGHT / 2;
            score++;
            if (rand() % 2 == 1) ballVelocityY *= -1;
            if (rand() % 2 == 2) ballVelocityX *= -1;
        }
    }
private:
    sf::RectangleShape ballObject;
    sf::Vector2f ballPosition;
    int ballVelocityX = 2;
    int ballVelocityY = 2;
    bool end = false;
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ponk");
    
    // creating objects
    Ball ball(100, 100);
    Platform platform(300, 540);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                platform.moveLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                platform.moveRight();
            }
            /*
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                platform.moveUp();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                platform.moveDown();
            }
            */

        }
        if (ball.getBallFloatRect().intersects(platform.getPlatFloatRect()))
            ball.rebound();

        // Load a sprite to display
        sf::Texture texture;
        if (!texture.loadFromFile("Texture.jpg"))
            return EXIT_FAILURE;
        sf::Sprite sprite(texture);
        
        //! Display
        ball.logic();
        ball.update();

        platform.update();

        window.clear();
        window.draw(sprite);
        
        if (ball.endTheGame())
            system("pause");

        window.draw(ball.getBallObject());
        window.draw(platform.getPlatObject());
        //std::this_thread::sleep_for(std::chrono::microseconds(1));
        window.display();
    }
    return 0;
}