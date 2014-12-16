#pragma once

#include <SFML/System/Vector2.hpp>
#include <boost/log/trivial.hpp>
#include <string>

#define LOG BOOST_LOG_TRIVIAL

typedef sf::Vector2<double> Vector2d;

//! Pops up an OS-specific error message.
/*!
  \param message The error message.
*/
extern void errorMessage(std::string message);