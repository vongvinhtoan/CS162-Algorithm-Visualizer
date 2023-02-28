#pragma once

#include "ResourceHolder.hpp"
#include "Textures.hpp"
#include "World.hpp"
#include <SFML/Graphics.hpp>

class Game
{
public:
    Game();
    void run();
    
private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
    sf::RenderWindow    mWindow;
    World               mWorld;

private:
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};
