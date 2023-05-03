#pragma once
#include <Book/State.hpp>
#include <GUI/Button.hpp>
#include <Book/CommandQueue.hpp>
#include <array>
#include <json/json.h>

class PauseState : public State
{
    public:
        PauseState(StateStack& stack, Context context);
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();

    private:
        void                buildScenes();

    private:
        enum Layers 
        {
            Buttons,
            LayerCount
        };

    private:
        sf::RenderWindow*                   mWindow;
        SceneNode                           mSceneGraph;
        sf::Text                            mPausedText;
        std::vector<Button*>                mButtons;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        Json::Value                         mData;
};
