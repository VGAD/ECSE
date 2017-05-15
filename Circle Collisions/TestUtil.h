//! \file TestUtil.h Contains utility functions for test states.

#pragma once

#include "ECSE/World.h"

//! Create a debug circle and add it to the world.
/*!
* \param world The world to create it in.
* \param start The circle's start position.
* \param end The circle's next position.
* \param radius The radius of the circle.
* \param discrete Whether this should be a discrete jump.
* \return The Entity ID of the created circle.
*/
ECSE::Entity::ID createCircle(ECSE::World& world, sf::Vector2f start, sf::Vector2f end,
                              float radius, bool discrete = false);

//! Create a debug line and add it to the world.
/*!
* \param world The world to create it in.
* \param start The line's start position.
* \param end The line's end position.
* \param offset The line's offset.
* \param discrete Whether this should be a discrete jump.
* \return The Entity ID of the created circle.
*/
ECSE::Entity::ID createLine(ECSE::World& world, sf::Vector2f start, sf::Vector2f end,
                            sf::Vector2f offset, bool discrete = false);