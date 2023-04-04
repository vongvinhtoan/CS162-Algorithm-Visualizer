#include <States/DataStructures/CircularLinkedList.hpp>

CircularLinkedList::CircularLinkedList(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void CircularLinkedList::draw()
{
    
}

bool CircularLinkedList::update(sf::Time dt)
{
    return true;
}

bool CircularLinkedList::handleEvent(const sf::Event& event)
{
    
    return true;
}

bool CircularLinkedList::handleRealtimeInput()
{
    
    return true;
}

void CircularLinkedList::buildScenes()
{
    
}
