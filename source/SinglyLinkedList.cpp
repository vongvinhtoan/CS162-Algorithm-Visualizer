#include <States/DataStructures/SinglyLinkedList.hpp>

SinglyLinkedList::SinglyLinkedList(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void SinglyLinkedList::draw()
{
    mWindow->draw(*mHead);
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
    mHead = new SLLNode("Head", sf::Vector2f(100, 100), 25.f, (*getContext().fonts)[Fonts::Default]);
    mHead->setNext(new SLLNode("Next", sf::Vector2f(100, 0), 25.f, (*getContext().fonts)[Fonts::Default]));
}

void SinglyLinkedList::deleteNodes(SLLNode* node)
{
    if(node->getNext())
        deleteNodes(node->getNext());
    delete node;
}

SinglyLinkedList::~SinglyLinkedList()
{
    deleteNodes(mHead);
}