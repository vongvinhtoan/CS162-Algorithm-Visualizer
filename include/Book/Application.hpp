#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceHolder.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include <States/TitleState.hpp>
#include <States/MenuState.hpp>
#include <States/GameState.hpp>
#include <States/PauseState.hpp>
#include <States/DataStructures/StaticArray.hpp>
#include <States/DataStructures/DynamicArray.hpp>
#include <States/DataStructures/SinglyLinkedList.hpp>
#include <States/DataStructures/DoublyLinkedList.hpp>

class Application
{
public:
    Application();
    void run();

private:
    void registerStates();
    void processInput();
    void update(sf::Time dt);
    void updateStatistics(sf::Time elapsedTime);
    void draw();

private:
    sf::RenderWindow    mWindow;
    TextureHolder       mTextures;
    FontHolder          mFonts;
    bool                mIsPaused;
    sf::Text            mStatisticsText;
    sf::Time            mStatisticsUpdateTime;
	int                 mStatisticsNumFrames;
    StateStack          mStateStack;
    Player              mPlayer;

private:
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};
