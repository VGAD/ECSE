#include "Engine.h"
#include "Spritemap.h"
#include <memory>

namespace Engine
{
    //! Private data.
    namespace
    {
        //! Cleans up Engine data before closing.
        void cleanup()
        {
            // These need to be called explicitly to avoid SFML getting angry about destroying
            // resources without a context
            textureManager.clear();
            soundManager.clear();
        }
    }

    ResourceManager<sf::Texture> textureManager;
    ResourceManager<sf::SoundBuffer> soundManager;

    void run()
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

        // Clean everything up
        cleanup();
    }

}