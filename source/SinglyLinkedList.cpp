#include <States/DataStructures/SinglyLinkedList.hpp>

SinglyLinkedList::SinglyLinkedList(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void SinglyLinkedList::draw()
{
    
}

bool SinglyLinkedList::update(sf::Time dt)
{
    return true;
}

bool SinglyLinkedList::handleEvent(const sf::Event& event)
{
    
    return true;
}

bool SinglyLinkedList::handleRealtimeInput()
{
    
    return true;
}

void SinglyLinkedList::buildScenes()
{
    
}
