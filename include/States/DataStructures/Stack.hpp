#pragma once
#include <Book/State.hpp>
#include <Book/CommandQueue.hpp>
#include <GUI/Button.hpp>
#include <GUI/Textbox.hpp>
#include <array>
#include <DSEntity/SLLNode.hpp>
#include <memory>
#include <windows.h>
#include <Book/DialogOpener.hpp>

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
        void filterTextboxesAndButtons(Button* button, std::vector<Textbox*> &textboxes, std::vector<Button*> &buttons);

        void handleEventButtonInit(Button *button, const sf::Event& event);
        void ButtonInitCreate(Button *button);
        void ButtonInitManualCreate(Button* button);
        void handleEventButtonInit_Go(Button *button, const sf::Event& event);
        void handleEventButtonInit_Manual(Button *button, const sf::Event& event);
        void handleEventButtonInit_Random(Button *button, const sf::Event& event);
        void handleEventButtonInit_File(Button *button, const sf::Event& event);

        void handleEventButtonPush(Button *button, const sf::Event& event);
        void ButtonPushCreate(Button *button);
        void handleEventButtonPush_Go(Button *button, const sf::Event& event, std::vector<Textbox*> &textboxes);

        void handleEventButtonPop(Button *button, const sf::Event& event);

        void handleEventButtonClear(Button *button, const sf::Event& event);

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
        Json::Value                         mData;
        DialogOpener                        mDialogOpener;
};
