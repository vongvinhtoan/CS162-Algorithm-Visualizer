#include <States/GameState.hpp>

GameState::GameState(StateStack& stack, Context context) : 
State(stack, context),
mWorld(*context.window) {}

void GameState::draw()
{
    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
    CommandQueue& commandQueue = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commandQueue);
    mPlayer.handleRealtimeInput(commandQueue);
    return true;
}