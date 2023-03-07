#pragma once

#include "ResourceHolder.hpp"
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
    void updateStatistics(sf::Time elapsedTime);
    void render();

private:
    sf::RenderWindow    mWindow;
    World               mWorld;
    FontHolder          mFonts;
    bool                mIsPaused;
    Player              mPlayer;
    sf::Text            mStatisticsText;
    sf::Time            mStatisticsUpdateTime;
	int                 mStatisticsNumFrames;

private:
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};
