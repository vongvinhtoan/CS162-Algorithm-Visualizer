#pragma once
#include <Book/State.hpp>

class TitleState : public State
{
    public:
        TitleState(StateStack& stack, Context context);
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();
        
    private:
        sf::Text            mText;
        bool                mShowText;
        sf::Time            mTextEffectTime;
        sf::RenderWindow*   mWindow;
};
