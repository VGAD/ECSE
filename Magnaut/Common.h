#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>

typedef sf::Vector2<double> Vector2d;

//! Pops up an OS-specific error message.
/*!
  \param message The error message.
*/
extern void errorMessage(std::string message);