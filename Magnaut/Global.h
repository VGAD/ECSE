#pragma once
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//! Contains global game-specific information.
namespace Magnaut
{
    // Data
    extern ResourceManager<sf::Texture> textureManager;    //!< Keeps track of loaded Textures.
    extern ResourceManager<sf::SoundBuffer> soundManager;  //!< Keeps track of loaded SoundBuffers.

    //! Cleanup all data. Must be called before exit to avoid SFML crash.
    void cleanup();
}