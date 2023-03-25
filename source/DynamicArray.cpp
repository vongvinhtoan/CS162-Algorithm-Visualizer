#include <States/DataStructures/DynamicArray.hpp>

DynamicArray::DynamicArray(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void DynamicArray::draw()
{
    
}

bool DynamicArray::update(sf::Time dt)
{
    return true;
}

bool DynamicArray::handleEvent(const sf::Event& event)
{
    
    return true;
}

bool DynamicArray::handleRealtimeInput()
{
    
    return true;
}

void DynamicArray::buildScenes()
{
    
}