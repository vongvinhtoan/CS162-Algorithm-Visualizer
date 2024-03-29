#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <functional>
#include <cassert>
#include "ResourceHolder.hpp"
#include "State.hpp"

class StateStack : private sf::NonCopyable
{
    public:
        enum Action
        {
            Push,
            Pop,
            Clear
        };
    public:        
        explicit            StateStack(State::Context context);
        template <typename T>
        void                registerState(States::ID stateID);
        void                update(sf::Time dt);
        void                draw();
        void                handleEvent(const sf::Event& event);
        void                handleRealtimeInput();
        void                pushState(States::ID stateID);
        void                popState();
        void                clearStates();
        bool                isEmpty() const;
        unsigned int        size() const;
        void                setDialogOpener(DialogOpener* dialogOpener);
        void                setRandomizer(Randomizer* randomizer);

    private:
        State::Ptr          createState(States::ID stateID);
        void                applyPendingChanges();
    private:
        struct PendingChange
        {
            Action              action;
            States::ID          stateID;
        };
    private:
        std::vector<State::Ptr>             mStack;
        std::vector<PendingChange>          mPendingList;
        State::Context                      mContext;
        std::map<States::ID, std::function<State::Ptr()>>    mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
    mFactories[stateID] = [this] ()
    {
        return State::Ptr(new T(*this, mContext));
    };
}