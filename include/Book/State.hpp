#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Player.hpp"
#include "ResourceHolder.hpp"

class StateStack;

namespace States
{  
    enum ID
    {
        NONE,
        Title,
        Menu,
        Game,
        StaticArray,
        DynamicArray,
        SinglyLinkedList,
        DoublyLinkedList,
        Pause
    };
} 

class State
{
    public:
        typedef std::unique_ptr<State> Ptr;
        struct Context 
        {
            Context(sf::RenderWindow& window,
                    TextureHolder& textures,
                    FontHolder& fonts,
                    Player& player);
            sf::RenderWindow*  window;
            TextureHolder*     textures;
            FontHolder*        fonts;
            Player*            player;
        };
    public:
                        State(StateStack& stack, Context context);
        virtual         ~State();
        virtual void    draw() = 0;
        virtual bool    update(sf::Time dt) = 0;
        virtual bool    handleEvent(const sf::Event& event) = 0;
        virtual bool    handleRealtimeInput() = 0;
    protected:
        void            requestStackPush(States::ID stateID);
        void            requestStackPop();
        void            requestStateClear();
        Context         getContext() const;
    private:
        StateStack*     mStack;
        Context         mContext;
};