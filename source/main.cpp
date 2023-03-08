#include <Book/Application.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    try
    {
        Application application;
        application.run();
    }
    catch (std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }
}
