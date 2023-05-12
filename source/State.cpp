#include <Book/State.hpp>
#include <Book/StateStack.hpp>
#include <iostream>

State::Context::Context
(
    sf::RenderWindow& window,
    TextureHolder& textures,
    FontHolder& fonts,
    Json::Value& data,
    DialogOpener& dialogOpener,
    Randomizer& randomizer
) :
window(&window),
textures(&textures),
fonts(&fonts),
data(&data),
dialogOpener(&dialogOpener),
randomizer(&randomizer)
{
}


State::State(StateStack& stack, Context context) : 
mContext(context),
mStack(&stack) 
{
    auto dBackground = (*getContext().data)["background"];
    mBackground.setSize((sf::Vector2f) getContext().window->getSize());
    mBackground.setFillColor(dBackground["default_color"].asColor());
}

State::~State()
{
    
}

void State::requestStackPush(States::ID stateID)
{
    mStack->pushState(stateID);
}

void State::requestStackPop()
{
    mStack->popState();
}

void State::requestStateClear()
{
    mStack->clearStates();
}

State::Context State::getContext() const
{
    return mContext;
}

void State::drawBackground()
{
    getContext().window->draw(mBackground);
}