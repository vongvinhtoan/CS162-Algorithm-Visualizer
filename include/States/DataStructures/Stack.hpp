#pragma once
#include <Book/State.hpp>
#include <Book/CommandQueue.hpp>
#include <GUI/Button.hpp>
#include <array>
#include <DSEntity/SLLNode.hpp>

class Stack : public State
{
    public:
        Stack(StateStack& stack, Context context);
        ~Stack();
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();
        void                push(std::string val);

    private:
        void                buildScenes();
        void                deleteNodes(SLLNode* node);

    private:
        enum Layers 
        {
            Nodes,
            LayerCount
        };

    private:
        sf::RenderWindow*                   mWindow;
        SceneNode                           mSceneGraph;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        CommandQueue                        mCommandQueue;
        SLLNode*                            mHead;
        
};
