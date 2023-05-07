#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceHolder.hpp"
#include "StateStack.hpp"
#include <States/TitleState.hpp>
#include <States/MenuState.hpp>
#include <States/PauseState.hpp>
#include <States/DataStructures/StaticArray.hpp>
#include <States/DataStructures/DynamicArray.hpp>
#include <States/DataStructures/SinglyLinkedList.hpp>
#include <States/DataStructures/DoublyLinkedList.hpp>
#include <States/DataStructures/CircularLinkedList.hpp>
#include <States/DataStructures/Stack.hpp>
#include <States/DataStructures/Queue.hpp>
#include <Book/DialogOpener.hpp>
#include <Book/Randomizer.hpp>
#include <json/json.h>
#include <fstream>
#include <iostream>

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
    void readJSON();

private:
    sf::RenderWindow                mWindow;
    TextureHolder                   mTextures;
    FontHolder                      mFonts;
    bool                            mIsPaused;
    bool                            mIsShowingStatistics;
    sf::Text                        mStatisticsText;
    sf::Time                        mStatisticsUpdateTime;
	int                             mStatisticsNumFrames;
    StateStack                      mStateStack;
    Json::Value                     mData;
    std::unique_ptr<DialogOpener>   mDialogOpener;
    std::unique_ptr<Randomizer>     mRandomizer;

private:
    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
};
