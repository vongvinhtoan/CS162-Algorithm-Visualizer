#include <States/DataStructures/StaticArray.hpp>

StaticArray::StaticArray(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void StaticArray::draw()
{
    
}

bool StaticArray::update(sf::Time dt)
{
    return true;
}

bool StaticArray::handleEvent(const sf::Event& event)
{
    // If press escape, trigger the pause screen
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        requestStackPush(States::ID::Pause);
        return true;
    }
    
    return true;
}

bool StaticArray::handleRealtimeInput()
{
    
    return true;
}

void StaticArray::buildScenes()
{
    
}
