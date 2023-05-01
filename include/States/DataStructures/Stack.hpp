#pragma once
#include <Book/State.hpp>
#include <Book/CommandQueue.hpp>
#include <GUI/Button.hpp>
#include <GUI/Textbox.hpp>
#include <array>
#include <DSEntity/SLLNode.hpp>
#include <memory>

class Stack : public State
{
    public:
        Stack(StateStack& stack, Context context);
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();
        void                push(std::string val);
        void                pop();
        void                clear();

    private:
        void                buildScenes();

    private:
        enum Layers 
        {
            Nodes,
            Buttons,
            LayerCount
        };

    private:
        sf::RenderWindow*                   mWindow;
        SceneNode                           mSceneGraph;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        SLLNode*                            mHead;
        std::vector<Button*>                mButtons;
};
