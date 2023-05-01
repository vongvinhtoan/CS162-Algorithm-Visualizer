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
    // If press escape, trigger the pause screen
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        requestStackPush(States::ID::Pause);
        return true;
    }
    
    return true;
}

bool CircularLinkedList::handleRealtimeInput()
{
    
    return true;
}

void CircularLinkedList::buildScenes()
{
    
}
