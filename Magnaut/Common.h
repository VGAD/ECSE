//! \file Common.h Contains generic useful typedefs, functions and logging macros.

#pragma once

#include <boost/log/trivial.hpp>
#include <string>
#include <cstdlib>
#include <cmath>
#include "Logging.h"

const float pi = 3.14159265359f;    //!< The value of pi.

//! Pops up an OS-specific error message.
/*!
* \param message The error message.
*/
extern void errorMessage(std::string message);

//! Convert from radians to degrees.
/*!
* \param angle The angle to convert in radians.
* \return The angle in degrees.
*/
inline float radToDeg(float angle)
{
    return angle / pi * 180.f;
}

//! Convert from degrees to radians.
/*!
* \param angle The angle to convert in degrees.
* \return The angle in radians.
*/
inline float degToRad(float angle)
{
    return angle / 180.f * pi;
}

//! Smoothly interpolate between two values.
/*!
* You should probably only use floating-point types for this function.
*
* \param value1 The first value.
* \param value2 The second value.
* \param amount The interpolation amount.
* \return The interpolated value.
*/
template <typename T>
inline T lerp(T value1, T value2, float amount)
{
    return value1 + (value2 - value1) * amount;
}

//! Clamp a value between two other values.
/*!
* \param min The minimum value.
* \param max The maximum value.
* \param value The value to clamp.
* \return The clamped value.
*/
template <typename T>
inline T clamp(T min, T max, T value)
{
    return std::max(min, std::min(max, value));
}

//! Find the smallest difference between two values given that they "wrap" around a third value.
/*!
* E.g. from = 0.5, to = 2.5, wrap = 3.0 -> the shortest distance is -0.5.
*
* \param from The starting value.
* \param to The value to move toward.
* \param wrap The value to wrap around.
* \return The smallest difference between the values.
*/
template <typename T>
T wrapDifference(T from, T to, T wrap)
{
    from = from % wrap;
    to = to % wrap;

    T diffA = (to - from);
    T diffB = (abs(diffA) - wrap) * (diffA > T(0) ? T(-1) : T(1));

    return (abs(diffA) < abs(diffB)) ? diffA : diffB;
}