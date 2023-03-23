#include <States/MenuState.hpp>

MenuState::MenuState(StateStack& stack, Context context) : 
State(stack, context),
mWindow(context.window)
{
    
}

void MenuState::draw()
{
    
}

bool MenuState::update(sf::Time dt)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    return true;
}

bool MenuState::handleRealtimeInput()
{
    return true;
}