#include <States/MenuState.hpp>

MenuState::MenuState(StateStack& stack, Context context) : 
State(stack, context),
mWindow(context.window),
bStaticArray(sf::Text("Static Array", (*context.fonts)[Fonts::Default]), sf::RectangleShape(sf::Vector2f(225, 75))),
bDynamicArray(sf::Text("Dynamic Array", (*context.fonts)[Fonts::Default]), sf::RectangleShape(sf::Vector2f(225, 75))),
bSinglyLinkedList(sf::Text("Singly Linked List", (*context.fonts)[Fonts::Default]), sf::RectangleShape(sf::Vector2f(225, 75))),
bDoublyLinkedList(sf::Text("Double Linked List", (*context.fonts)[Fonts::Default]), sf::RectangleShape(sf::Vector2f(225, 75)))
{
    bStaticArray.setPosition(100, 100);
    bDynamicArray.setPosition(100, 200);
    bSinglyLinkedList.setPosition(100, 300);
    bDoublyLinkedList.setPosition(100, 400);
}

void MenuState::draw()
{
    mWindow->draw(bStaticArray);
    mWindow->draw(bDynamicArray);
    mWindow->draw(bSinglyLinkedList);
    mWindow->draw(bDoublyLinkedList);
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
    bStaticArray.handleRealtimeInput(mWindow);
    bDynamicArray.handleRealtimeInput(mWindow);
    bSinglyLinkedList.handleRealtimeInput(mWindow);
    bDoublyLinkedList.handleRealtimeInput(mWindow);
    return true;
}