#include <Book/Game.hpp>

Game::Game() : 
mWindow(sf::VideoMode(640, 480), "Algorithm Visualizer", sf::Style::Close),
mWorld(mWindow),
mIsPaused(false)
{
    
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processInput();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processInput();
            if(!mIsPaused) update(TimePerFrame);
        }
        render();
    }
}

void Game::processInput()
{
    CommandQueue& commandQueue = mWorld.getCommandQueue();

    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        mPlayer.handleEvent(event, commandQueue);
        switch (event.type)
        {
            case sf::Event::Closed:
                mWindow.close();
                break;
            case sf::Event::GainedFocus:
                mIsPaused = false;
                break;
            case sf::Event::LostFocus:
                mIsPaused = true;
                break;
        }
    }

    mPlayer.handleRealtimeInput(commandQueue);
}

void Game::update(sf::Time dt)
{
    mWorld.update(dt);
}

void Game::render()
{
    mWindow.clear();
    mWorld.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    
}