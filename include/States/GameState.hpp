#pragma once
#include <Book/State.hpp>
#include <Book/World.hpp>

class GameState : public State
{
    public:
        GameState(StateStack& stack, Context context);
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();
    private:
        World               mWorld;
};
