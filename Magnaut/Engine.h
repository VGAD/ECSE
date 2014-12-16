#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourceManager.h"

//! Contains the base game information and procedures, such as the game loop and resource management.
class Engine
{
public:
    //! Initializes the engine.
    Engine();

    //! Deallocates resources and stops the game.
    ~Engine();

    //! Runs the game loop.
    void run();

private:

};