//! \file Random.h Contains functions for randomly generating numbers.
/*!
* If only functions/engines from this file are used, then determinism is guaranteed,
* so it's highly recommended not to roll your own random functions.
*/

#pragma once

#include <random>
#include <SFML/System.hpp>

namespace ECSE
{

//! The random engine that should be used for generating any random numbers in ECSE.
extern std::mt19937 randomEngine;

//! Generate a random vector in an angular spread.
/*!
* \param midAngle The midpoint of the possible angles.
* \param angleSpread The spread of the possible angles (in radians).
* \param minMag The minimum magnitude.
* \param maxMag The maximum magnitude.
* \return A reference v1.
*/
sf::Vector2f randomSpreadVector(float midAngle, float angleSpread, float minMag, float maxMag);

//! Generate a random integer.
/*!
* \param min The minimum value.
* \param max The maximum value.
* \return An int in the range [min, max].
*/
inline int randomInt(int min, int max)
{
    auto dist = std::uniform_int_distribution<>(min, max);
    return dist(randomEngine);
}

//! Generate a random float.
/*!
* \param min The minimum value.
* \param max The maximum value.
* \return A real number in the range [min, max].
*/
inline float randomFloat(float min, float max)
{
    auto dist = std::uniform_real_distribution<float>(min, max);
    return dist(randomEngine);
}

}