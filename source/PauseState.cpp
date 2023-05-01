#include <States/PauseState.hpp>
#include <iostream>

PauseState::PauseState(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window),
mPausedText("Paused", (*context.fonts)[Fonts::Default], 70)
{
    buildScenes();
}

void PauseState::draw()
{
    mWindow->setView(mWindow->getDefaultView());
    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(sf::Vector2f(mWindow->getSize()));

    mWindow->draw(backgroundShape);
    mWindow->draw(mPausedText);
    mWindow->draw(mSceneGraph);
}

bool PauseState::update(sf::Time dt)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
    // Escape pressed, remove itself to return to the game
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        requestStackPop();
        return false;
    }

    // Handle events for all buttons
    for(auto &button: mButtons) {
        button->handleEvent(event, mWindow);
    }

    for(auto &button: mButtons)
    {
        if(!button->isClicked()) continue;

        if(button->getCategory() == Button::Category::Menu) {
            requestStateClear();
            requestStackPush(States::ID::Menu);
            return false;
        }

        if(button->getCategory() == Button::Category::Resume) {
            requestStackPop();
            return false;
        }
    }
    return false;
}

bool PauseState::handleRealtimeInput()
{
    for(auto &button: mButtons) {
        button->handleRealtimeInput(mWindow);
    }
    return false;
}

void PauseState::buildScenes()
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    // Set the paused text
    auto textRect = mPausedText.getLocalBounds();
    mPausedText.setOrigin(  
        textRect.left + textRect.width/2.f,
        textRect.top  + textRect.height/2.f
    );
    mPausedText.setPosition(mWindow->getView().getSize() / 2.f);

    // Set the buttons
    std::unique_ptr<Button> bResume(new Button(
        Button::Category::Resume,
        sf::Text("Resume", (*getContext().fonts)[Fonts::Default]),
        sf::RectangleShape(sf::Vector2f(200.f, 50.f))
    ));
    bResume->setPosition(mWindow->getView().getSize() / 2.f);
    mButtons.push_back(bResume.get());
    mSceneLayers[Buttons]->attachChild(std::move(bResume));

    std::unique_ptr<Button> bMenu(new Button(
        Button::Category::Menu,
        sf::Text("Menu", (*getContext().fonts)[Fonts::Default]),
        sf::RectangleShape(sf::Vector2f(200.f, 50.f))
    )); 
    bMenu->setPosition(mWindow->getView().getSize() / 2.f + sf::Vector2f(0.f, 100.f));
    mButtons.push_back(bMenu.get());
    mSceneLayers[Buttons]->attachChild(std::move(bMenu));
}