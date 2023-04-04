#include <States/DataStructures/Queue.hpp>

Queue::Queue(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void Queue::draw()
{
    
}

bool Queue::update(sf::Time dt)
{
    return true;
}

bool Queue::handleEvent(const sf::Event& event)
{
    
    return true;
}

bool Queue::handleRealtimeInput()
{
    
    return true;
}

void Queue::buildScenes()
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
}
