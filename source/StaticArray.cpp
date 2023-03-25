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
    
    return true;
}

bool StaticArray::handleRealtimeInput()
{
    
    return true;
}

void StaticArray::buildScenes()
{
    
}
