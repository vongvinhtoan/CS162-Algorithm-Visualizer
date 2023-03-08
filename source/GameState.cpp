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
    getContext().player->handleEvent(event, commandQueue);
    return true;
}

bool GameState::handleRealtimeInput()
{
    CommandQueue& commandQueue = mWorld.getCommandQueue();
    getContext().player->handleRealtimeInput(commandQueue);
    return true;
}