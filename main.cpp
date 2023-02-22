#include <SFML/Graphics.hpp>

void renderingThread(sf::RenderWindow* window);

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "Algorithm Visualizer");

    window.setActive(false);

    sf::Thread thread(&renderingThread, &window);
    thread.launch();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) 
                window.close(); 
        }
    }

    return 0;
}

void renderingThread(sf::RenderWindow* window)
{
    window->setActive(true);

    while(window->isOpen())
    {
        window->clear(sf::Color::White);
        
        // Draw here

        window->display();
    }
}

