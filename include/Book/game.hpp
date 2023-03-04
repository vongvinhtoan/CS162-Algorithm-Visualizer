#pragma once

#include "ResourceHolder.hpp"
#include "Textures.hpp"
#include "World.hpp"
#include <SFML/Graphics.hpp>
#include "Player.hpp"

class Game
{
public:
    Game();
    void run();

private:
    void processInput();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
    sf::RenderWindow    mWindow;
    World               mWorld;
    bool                mIsPaused;
    Player              mPlayer;

private:
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};
