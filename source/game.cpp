#include <Book/Game.hpp>
#include <iostream>

Game::Game() : 
mWindow(sf::VideoMode(640, 480), "Algorithm Visualizer", sf::Style::Close),
mWorld(mWindow),
mIsPaused(false),
mStatisticsNumFrames(0),
mStatisticsUpdateTime()
{
    mFonts.load(Fonts::Default, "Media/Fonts/Sansation.ttf");

	mStatisticsText.setFont(mFonts[Fonts::Default]);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time elapsed = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        processInput();
        elapsed = clock.restart();
        timeSinceLastUpdate += elapsed;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            if(!mIsPaused) update(TimePerFrame);
        }
        updateStatistics(elapsed);
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
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"FPS = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");
							 
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}