#pragma once
#include <Book/State.hpp>
#include <Book/CommandQueue.hpp>
#include <GUI/Button.hpp>
#include <array>
#include <DSEntity/SLLNode.hpp>
#include <DSEntity/Arrow.hpp>

class SinglyLinkedList : public State
{
    public:
        SinglyLinkedList(StateStack& stack, Context context);
        ~SinglyLinkedList();
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
        SLLNode*                            mHead;
};
