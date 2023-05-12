#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <Book/ResourceHolder.hpp>
#include <Book/DialogOpener.hpp>
#include <Book/Randomizer.hpp>
#include <json/json.h>

class StateStack;

namespace States
{  
    enum ID
    {
        NONE,
        Title,
        Menu,
        StaticArray,
        DynamicArray,
        SinglyLinkedList,
        DoublyLinkedList,
        CircularLinkedList,
        Stack,
        Queue,
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
                    Json::Value& data,
                    DialogOpener& dialogOpener,
                    Randomizer& randomizer);
            sf::RenderWindow*  window;
            TextureHolder*     textures;
            FontHolder*        fonts;
            Json::Value*       data;
            DialogOpener*      dialogOpener;
            Randomizer*        randomizer;
        };
    public:
                        State(StateStack& stack, Context context);
        virtual         ~State();
        virtual void    draw() = 0;
        virtual bool    update(sf::Time dt) = 0;
        virtual bool    handleEvent(const sf::Event& event) = 0;
        virtual bool    handleRealtimeInput() = 0;
        void            drawBackground();
    protected:
        void            requestStackPush(States::ID stateID);
        void            requestStackPop();
        void            requestStateClear();
        Context         getContext() const;
    private:
        StateStack*         mStack;
        Context             mContext;
        sf::RectangleShape  mBackground;
};