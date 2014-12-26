//! \file Common.h Contains generic useful typedefs, functions and logging macros.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <boost/log/trivial.hpp>
#include <string>
#include "Logging.h"

//! A 2D SFML vector using doubles.
typedef sf::Vector2<double> Vector2d;

//! Pops up an OS-specific error message.
/*!
* \param message The error message.
*/
extern void errorMessage(std::string message);