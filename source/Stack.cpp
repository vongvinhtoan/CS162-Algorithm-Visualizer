#include <States/DataStructures/Stack.hpp>

Stack::Stack(StateStack& stack, Context context) :
State(stack, context),
mWindow(context.window)
{
    buildScenes();
}

void Stack::draw()
{
    
}

bool Stack::update(sf::Time dt)
{
    return true;
}

bool Stack::handleEvent(const sf::Event& event)
{
    
    return true;
}

bool Stack::handleRealtimeInput()
{
    
    return true;
}

void Stack::buildScenes()
{
    for (int i = 0; i < LayerCount; ++i)
    {
        SceneNode::Ptr layer(new SceneNode());
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }
}
