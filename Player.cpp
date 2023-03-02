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
        output.category = Category::PlayerFlag | Category::AlliedFlag;
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
    float playerSpeed = 100.f;
    
    Command moveLeft;
    moveLeft.category = Category::PlayerFlag;
    moveLeft.action = derivedAction<Flag>(FlagMover(-playerSpeed, 0.f));
    Command moveRight;
    moveRight.category = Category::PlayerFlag;
    moveRight.action = derivedAction<Flag>(FlagMover(playerSpeed, 0.f));
    Command moveUp;
    moveUp.category = Category::PlayerFlag;
    moveUp.action = derivedAction<Flag>(FlagMover(0.f, -playerSpeed));
    Command moveDown;
    moveDown.category = Category::PlayerFlag;
    moveDown.action = derivedAction<Flag>(FlagMover(0.f, playerSpeed));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        commands.push(moveLeft);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        commands.push(moveRight);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        commands.push(moveUp);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        commands.push(moveDown);
}