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
    sf::Vector2i size(800, 600);
    sf::RenderWindow window(sf::VideoMode(size.x, size.y), "Magnaut");

    LOG(INFO) << "Initialized window at size " << size.x << "x" << size.y;

    // Run the main loop
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                LOG(TRACE) << "Close event detected";
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}