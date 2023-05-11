#pragma once
#include <Book/State.hpp>
#include <Book/CommandQueue.hpp>
#include <GUI/Button.hpp>
#include <GUI/Textbox.hpp>
#include <array>
#include <DSEntity/ArrayBlock.hpp>
#include <memory>
#include <windows.h>
#include <fstream>

class DynamicArray : public State
{
    public:
        DynamicArray(StateStack& stack, Context context);
        virtual void        draw();
        virtual bool        update(sf::Time dt);
        virtual bool        handleEvent(const sf::Event& event);
        virtual bool        handleRealtimeInput();

    private:
        void                allocate(int size);
        void                insert(int id, std::string val);
        void                delete_(int id);
        void                update(int id, std::string val);
        void                access(int id);
        void                search(std::string val);
        void                init(std::stringstream &ss);
        void                initRandom();

    private:
        void                buildScenes();

    private:
        void filterTextboxesAndButtons(Button* button, std::vector<Textbox*> &textboxes, std::vector<Button*> &buttons);

        void handleEventButtonAllocate(Button *button, const sf::Event& event);
            void ButtonAllocateCreate(Button *button);
            void handleEventButtonAllocate_Go(Button *button, const sf::Event& event);

        void handleEventButtonInit(Button *button, const sf::Event& event);
            void ButtonInitCreate(Button *button);
            bool handleEventButtonInit_Manual(Button *button, const sf::Event& event);
                void ButtonInitManualCreate(Button* button);
                void handleEventButtonInit_Manual_Go(Button *button, const sf::Event& event);
            bool handleEventButtonInit_Random(Button *button, const sf::Event& event);
            bool handleEventButtonInit_File(Button *button, const sf::Event& event);

        void handleEventButtonInsert(Button *button, const sf::Event& event);
            void ButtonInsertCreate(Button *button);
            bool handleEventButtonInsert_First(Button *button, const sf::Event& event);
                void ButtonInsertFirstCreate(Button *button);
                void handleEventButtonInsert_First_Go(Button *button, const sf::Event& event);
            bool handleEventButtonInsert_Last(Button *button, const sf::Event& event);
                void ButtonInsertLastCreate(Button *button);
                void handleEventButtonInsert_Last_Go(Button *button, const sf::Event& event);
            bool handleEventButtonInsert_Middle(Button *button, const sf::Event& event);
                void ButtonInsertMiddleCreate(Button *button);
                void handleEventButtonInsert_Middle_Go(Button *button, const sf::Event& event);

        void handleEventButtonDelete(Button *button, const sf::Event& event);
            void ButtonDeleteCreate(Button *button);
            bool handleEventButtonDelete_First(Button *button, const sf::Event& event);
            bool handleEventButtonDelete_Last(Button *button, const sf::Event& event);
            bool handleEventButtonDelete_Middle(Button *button, const sf::Event& event);
                void ButtonDeleteMiddleCreate(Button *button);
                void handleEventButtonDelete_Middle_Go(Button *button, const sf::Event& event);

        void handleEventButtonUpdate(Button *button, const sf::Event& event);
            void ButtonUpdateCreate(Button *button);
            void handleEventButtonUpdate_Go(Button *button, const sf::Event& event);

        void handleEventButtonAccess(Button *button, const sf::Event& event);
            void ButtonAccessCreate(Button *button);
            void handleEventButtonAccess_Go(Button *button, const sf::Event& event);

        void handleEventButtonSearch(Button *button, const sf::Event& event);
            void ButtonSearchCreate(Button *button);
            void handleEventButtonSearch_Go(Button *button, const sf::Event& event);

    private:
        enum Layers 
        {
            Blocks,
            Buttons,
            LayerCount
        };

    private:
        sf::RenderWindow*                   mWindow;
        SceneNode                           mSceneGraph;
        std::array<SceneNode*, LayerCount>  mSceneLayers;
        ArrayBlock*                         mHead;
        int                                 mAllocatedSize;
        int                                 mSize;
        std::vector<Button*>                mButtons;
        Json::Value                         &mData;
        DialogOpener*                       mDialogOpener;
        Randomizer*                         mRandomizer;
};
