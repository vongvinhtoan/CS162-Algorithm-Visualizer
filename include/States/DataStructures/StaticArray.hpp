#pragma once
#include <Book/State.hpp>
#include <Book/CommandQueue.hpp>
#include <Book/Button.hpp>
#include <array>

class StaticArray : public State
{
    public:
        StaticArray(StateStack& stack, Context context);
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();

    private:
        void                buildScenes();

    private:
        enum Layers 
        {
            LayerCount
        };

    private:
        sf::RenderWindow*                   mWindow;
        SceneNode                           mSceneGraph;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        CommandQueue                        mCommandQueue;
};
