#include "Player.hpp"

Player::Player()
{

}

#include <iostream>

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed
     && event.key.code == sf::Keyboard::P)
    {
        Command output;
        output.category = Category::PlayerFlag;
        output.action = [] (SceneNode& s, sf::Time)
        {
            std::cout << s.getPosition().x << "," 
                      << s.getPosition().y << "\n";
        };
        commands.push(output);
    }
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
    
    float playerSpeed = 30.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        Command moveLeft;
        moveLeft.category = Category::PlayerFlag;
        moveLeft.action = derivedAction<Flag>(FlagMover(-playerSpeed, 0.f));
        commands.push(moveLeft);
    }
}