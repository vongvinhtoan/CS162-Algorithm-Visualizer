#pragma once
#include <Book/State.hpp>
#include <Book/Button.hpp>

class MenuState : public State
{
    public:
        MenuState(StateStack& stack, Context context);
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();

    private:
        sf::RenderWindow*   mWindow;
        Button              bStaticArray;
        Button              bDynamicArray;
        Button              bSinglyLinkedList;
        Button              bDoublyLinkedList;
};
