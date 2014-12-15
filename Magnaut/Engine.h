#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourceManager.h"

//! Includes functions and data which are needed globally in the game.
namespace Engine
{
    //! Keeps track of loaded Textures.
    extern ResourceManager<sf::Texture> textureManager;

    //! Keeps track of loaded SoundBuffers.
    extern ResourceManager<sf::SoundBuffer> soundManager;

    //! Runs the game init, loop and cleanup.
    void run();
}