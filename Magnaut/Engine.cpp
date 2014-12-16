#include "Engine.h"
#include "Spritemap.h"
#include <memory>

Engine::Engine()
{

}

Engine::~Engine()
{

}

void Engine::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Magnaut");

    // Run the main loop
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}