#include<SFML/Graphics.hpp>

class Game
{
public:
    Game();
    void run();
private:
    void processEvents();
    void update();
    void render();
private:
    sf::RenderWindow  mWindow;
};

