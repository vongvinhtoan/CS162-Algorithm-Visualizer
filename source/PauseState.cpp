#include <States/PauseState.hpp>
#include <iostream>

PauseState::PauseState(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window),
mPausedText("Paused", (*context.fonts)[Fonts::Default], 70),
mData((*context.data)["PauseState"])
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
    auto textRect = mPausedText.getLocalBounds();
    mPausedText.setOrigin(  
        textRect.left + textRect.width/2.f,
        textRect.top  + textRect.height/2.f
    );
    mPausedText.setPosition(mWindow->getView().getSize().x / 2.f, mData["tPause"]["top"].asFloat());

    mSceneGraph.update(dt);
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

        if(button->getCategory() == Button::Category::Quit) {
            mWindow->close();
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
    mPausedText.setCharacterSize(mData["tPause"]["charSize"].asUInt());

    // Set the buttons
    auto dResume = mData["bResume"];
    std::unique_ptr<Button> bResume(new Button(
        Button::Category::Resume,
        sf::Text("Resume", (*getContext().fonts)[Fonts::Default], dResume["charSize"].asUInt()),
        sf::RectangleShape(dResume["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bResume->setOrigin(bResume->getLocalBounds().width / 2.f, bResume->getLocalBounds().height / 2.f);
    bResume->setPosition(mWindow->getView().getSize().x / 2.f, dResume["top"].asFloat());
    bResume->setCharacterSize(dResume["charSize"].asUInt());
    mButtons.push_back(bResume.get());
    mSceneLayers[Buttons]->attachChild(std::move(bResume));

    auto dMenu = mData["bMenu"];
    std::unique_ptr<Button> bMenu(new Button(
        Button::Category::Menu,
        sf::Text("Menu", (*getContext().fonts)[Fonts::Default], dMenu["charSize"].asUInt()),
        sf::RectangleShape(dMenu["size"].asVector2f()),
        (*getContext().data)["buttons"]
    )); 
    bMenu->setOrigin(bMenu->getLocalBounds().width / 2.f, bMenu->getLocalBounds().height / 2.f);
    bMenu->setPosition(mWindow->getView().getSize().x / 2.f, dMenu["top"].asFloat());
    bMenu->setCharacterSize(dMenu["charSize"].asUInt());
    mButtons.push_back(bMenu.get());
    mSceneLayers[Buttons]->attachChild(std::move(bMenu));

    // Quit button
    auto dQuit = mData["bQuit"];
    std::unique_ptr<Button> bQuit(new Button(
        Button::Category::Quit,
        sf::Text("Quit", (*getContext().fonts)[Fonts::Default], dQuit["charSize"].asUInt()),
        sf::RectangleShape(dQuit["size"].asVector2f()),
        (*getContext().data)["buttons"]
    ));
    bQuit->setOrigin(bQuit->getLocalBounds().width / 2.f, bQuit->getLocalBounds().height / 2.f);
    bQuit->setPosition(mWindow->getView().getSize().x / 2.f, dQuit["top"].asFloat());
    bQuit->setCharacterSize(dQuit["charSize"].asUInt());
    mButtons.push_back(bQuit.get());
    mSceneLayers[Buttons]->attachChild(std::move(bQuit));
}