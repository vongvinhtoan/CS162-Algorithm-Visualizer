#include "game.h"

Game::Game()
: mWindow(sf::VideoMode(500, 500), "Algorithm Visualizer")
{

}

void Game::run()
{
    while (mWindow.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Game::update()
{

}

void Game::render()
{
    mWindow.clear();
    mWindow.display();
}