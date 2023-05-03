#include <Book/Application.hpp>

Application::Application() : 
mStateStack(State::Context(mWindow, mTextures, mFonts, mData)),
mWindow(sf::VideoMode(800, 600), "Algorithm Visualizer", sf::Style::Close),
mIsPaused(false),
mStatisticsNumFrames(0),
mStatisticsUpdateTime(),
mIsShowingStatistics(false)
{
    readJSON();
    mFonts.load(Fonts::Default, "Media/Fonts/Sansation.ttf");
    
    registerStates();
    mStateStack.pushState(States::Menu);
    mStateStack.draw();

	mStatisticsText.setFont(mFonts[Fonts::Default]);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}

void Application::readJSON()
{
    std::ifstream ifs("Media/JSON/data.json");
    Json::Reader reader;
    reader.parse(ifs, mData);
}

void Application::registerStates()
{
    mStateStack.registerState<TitleState>           (States::Title);
    mStateStack.registerState<MenuState>            (States::Menu);
    mStateStack.registerState<StaticArray>          (States::StaticArray);
    mStateStack.registerState<DynamicArray>         (States::DynamicArray);
    mStateStack.registerState<SinglyLinkedList>     (States::SinglyLinkedList);
    mStateStack.registerState<DoublyLinkedList>     (States::DoublyLinkedList);
    mStateStack.registerState<CircularLinkedList>   (States::CircularLinkedList);
    mStateStack.registerState<Stack>                (States::Stack);
    mStateStack.registerState<Queue>                (States::Queue);
    mStateStack.registerState<PauseState>           (States::Pause);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time elapsed             = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        if(mStateStack.isEmpty()) mWindow.close();

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
        draw();
    }
}

void Application::processInput()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);
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
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::F5) mIsShowingStatistics ^= true;
                break;
        }
    }
    mStateStack.handleRealtimeInput();
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::draw()
{
    mWindow.clear();
    mStateStack.draw();
    
    mWindow.setView(mWindow.getDefaultView());
    if(mIsShowingStatistics) mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Application::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"FPS = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us"
        );					 
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}