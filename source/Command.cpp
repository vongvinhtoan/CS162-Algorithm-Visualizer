#include <Book/Command.hpp>

Command::Command() : 
category(0)
{

}

Command::Command(std::function<void(SceneNode&, sf::Time)> action, unsigned int category) :
action{action},
category{category}
{

}