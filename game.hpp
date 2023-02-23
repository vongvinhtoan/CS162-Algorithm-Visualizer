#pragma once

#include "ResourceHolder.hpp"
#include<SFML/Graphics.hpp>

namespace Textures
{
    enum ID{Flag};  
} 

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
    sf::RenderWindow                            mWindow;
    ResourceHolder<sf::Texture, Textures::ID>   mTexture;
    sf::Sprite                                  mPlayer;
    bool    mIsMovingUp;
    bool    mIsMovingDown;
    bool    mIsMovingLeft;
    bool    mIsMovingRight;
    float   playerSpeed;
private:
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};
