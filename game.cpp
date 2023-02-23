#include "game.hpp"

Game::Game()
: mWindow(sf::VideoMode(640, 480), "Algorithm Visualizer"),
mTexture(),
mPlayer(),
mIsMovingUp(false),
mIsMovingDown(false),
mIsMovingLeft(false),
mIsMovingRight(false),
playerSpeed(200.f)
{
    mTexture.load(Textures::Flag, "Media/Textures/game_flag.bmp");
    
    mPlayer.setTexture(mTexture.get(Textures::Flag));
    mPlayer.setPosition(100.f, 100.f);

    mWindow.setFramerateLimit(60);
    mWindow.setVerticalSyncEnabled(true);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case sf::Event::Closed:
                mWindow.close();
                break;
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    sf::Vector2f movement(0.f, 0.f);
    if (mIsMovingUp)
        movement.y -= playerSpeed;
    if (mIsMovingDown)
        movement.y += playerSpeed;
    if (mIsMovingLeft)
        movement.x -= playerSpeed;
    if (mIsMovingRight)
        movement.x += playerSpeed;

    mPlayer.move(movement * deltaTime.asSeconds());
}

void Game::render()
{
    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::W)
        mIsMovingUp = isPressed;
    else if (key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
    else if (key == sf::Keyboard::A)
        mIsMovingLeft = isPressed;
    else if (key == sf::Keyboard::D)
        mIsMovingRight = isPressed;
}