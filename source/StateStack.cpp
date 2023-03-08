#include <Book/StateStack.hpp>

StateStack::StateStack(State::Context context) :
mContext(context) {}

State::Ptr StateStack::createState(States::ID stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());
    return found->second();
}

void StateStack::handleEvent(const sf::Event& event)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->handleEvent(event))
            return;
    }
 
    applyPendingChanges();
}

void StateStack::update(sf::Time dt)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->update(dt))
            return;
    }
 
    applyPendingChanges();
}

void StateStack::draw()
{
    for (auto itr = mStack.begin(); itr != mStack.end(); ++itr)
    {
        (*itr)->draw();
    }
 
    applyPendingChanges();
}

void StateStack::applyPendingChanges()
{
    for(PendingChange change: mPendingList)
    {
        switch (change.action)
        {
            case Push:
                mStack.push_back(createState(change.stateID));
                break;
            case Pop:
                mStack.pop_back();
                break;
            case Clear:
                mStack.clear();
                break;
        }
    }
    mPendingList.clear();
}

void StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back({Push, stateID});
}

void StateStack::popState()
{
    mPendingList.push_back({Pop, States::NONE});
}

void StateStack::clearStates()
{
    mPendingList.push_back({Clear, States::NONE});
}

bool StateStack::isEmpty() const
{
    return mPendingList.size() == 0;
}
