#include <Book/Game.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    try
    {
        Game game;
        game.run();
    }
    catch (std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }
}
