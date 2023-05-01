#include <States/DataStructures/DoublyLinkedList.hpp>

DoublyLinkedList::DoublyLinkedList(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void DoublyLinkedList::draw()
{
    
}

bool DoublyLinkedList::update(sf::Time dt)
{
    return true;
}

bool DoublyLinkedList::handleEvent(const sf::Event& event)
{
    // If press escape, trigger the pause screen
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        requestStackPush(States::ID::Pause);
        return true;
    }
    
    return true;
}

bool DoublyLinkedList::handleRealtimeInput()
{
    
    return true;
}

void DoublyLinkedList::buildScenes()
{
    
}
