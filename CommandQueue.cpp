#include "CommandQueue.hpp"

void CommandQueue::push(const Command& command)
{
    mQueue.push(command);
}

Command CommandQueue::pop()
{
    if(isEmpty())
        throw std::runtime_error("Command Queue is empty, can't pop()");

    Command command = mQueue.front();
    mQueue.pop();
    return command;
}

bool CommandQueue::isEmpty() const
{
    return mQueue.empty();
}