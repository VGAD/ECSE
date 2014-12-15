#include "Engine.h"

namespace Engine
{
	//! Private data.
	namespace
	{
		//! The game's window.
		sf::RenderWindow window;

		//! Processes events in each update.
		/*! Quit if Escape is pressed or Closed event is registered. */
		void processEvents()
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
		}
	}

	ResourceManager<sf::Texture> textureManager;
	ResourceManager<sf::SoundBuffer> soundManager;

	void init()
	{
		window.create(sf::VideoMode(800, 600), "Magnaut");
	}

	void loop()
	{
		// Run the main loop
		while (window.isOpen())
		{
			processEvents();
			window.clear();
			window.display();
		}
	}

	void cleanup()
	{
		// textureManager and soundManager automatically free their contents
	}
}