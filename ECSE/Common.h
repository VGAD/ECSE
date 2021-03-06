//! \file Common.h Contains generic useful typedefs, functions and logging macros.

#pragma once

#include <cmath>
#include <algorithm>
#include <string>

namespace ECSE
{

const float pi = 3.14159265358979323846f;   //!< The value of pi.
const float twoPi = pi * 2.f;               //!< The value of pi * 2.
const float halfPi = pi * 0.5f;             //!< The value of pi * 0.5.
const float quarterPi = pi * 0.25f;         //!< The value of pi * 0.25.

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

//! Performs modulus on a floating-point number, keeping it within the range of [0..y) or (y..0]
/*!
* Shamelessly stolen from: http://stackoverflow.com/a/4635752/858878
*
* \param x The dividend.
* \param y The divisor.
* \return (x % y) within the range [0..y], or (y..0] if y <= 0.
*/
template<typename T>
T intervalMod(T x, T y)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");

    if (0. == y)
        return x;

    T m = x - y * floor(x/y);

    // handle boundary cases resulted from floating-point cut off:

    if (y > 0)              // modulo range: [0..y)
    {
        if (m>=y)           // Mod(-1e-16             , 360.    ): m= 360.
            return 0;

        if (m<0)
        {
            if (y+m == y)
                return 0; // just in case...
            else
                return y+m; // Mod(106.81415022205296 , _TWO_PI ): m= -1.421e-14
        }
    }
    else                    // modulo range: (y..0]
    {
        if (m<=y)           // Mod(1e-16              , -360.   ): m= -360.
            return 0;

        if (m>0)
        {
            if (y+m == y)
                return 0; // just in case...
            else
                return y+m; // Mod(-106.81415022205296, -_TWO_PI): m= 1.421e-14
        }
    }

    return m;
}

//! Smoothly interpolate between two values.
/*!
* \param from The first value.
* \param to The second value.
* \param amount The interpolation amount.
* \return The interpolated value.
*/
template <typename T>
inline T lerp(T from, T to, float amount)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");
    return from + (to - from) * amount;
}

//! Smoothly interpolate between two values, easing both in and out.
/*!
* Reference: http://wiki.unity3d.com/index.php?title=Mathfx
*
* \param from The first value.
* \param to The second value.
* \param amount The interpolation amount.
* \return The interpolated value.
*/
template <typename T>
inline T easeInOutLerp(T from, T to, float amount)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");
    return lerp(from, to, amount * amount * (3.0f - 2.0f * amount));
}

//! Smoothly interpolate between two values, easing in.
/*!
* Reference: http://wiki.unity3d.com/index.php?title=Mathfx
*
* \param from The first value.
* \param to The second value.
* \param amount The interpolation amount.
* \return The interpolated value.
*/
template <typename T>
inline T easeInLerp(T from, T to, float amount)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");
    return lerp(from, to, sin(amount * ECSE::halfPi));
}

//! Smoothly interpolate between two values, easing out.
/*!
* Reference: http://wiki.unity3d.com/index.php?title=Mathfx
*
* \param from The first value.
* \param to The second value.
* \param amount The interpolation amount.
* \return The interpolated value.
*/
template <typename T>
inline T easeOutLerp(T from, T to, float amount)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");
    return lerp(from, to, 1.f - cos(amount * ECSE::halfPi));
}

//! Find the midpoint between two values.
/*!
* \param from The first value.
* \param to The second value.
* \return The interpolated value.
*/
template <typename T>
inline T midpoint(T from, T to)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");
    return lerp(from, to, 0.5f);
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

//! Find the smallest difference between two values given that they "wrap" around a third value back to zero.
/*!
* E.g. from = 0.5, to = 2.5, wrap = 3.0 -> the shortest distance is -1.0.
*
* \param from The starting value.
* \param to The value to move toward.
* \param wrap The value to wrap around.
* \return The smallest difference between the values.
*/
template <typename T>
T wrapDifference(T from, T to, T wrap)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");

    from = intervalMod(from, wrap);
    to = intervalMod(to, wrap);

    T diffA = to - from;
    T diffB = std::abs(std::abs(diffA) - std::abs(wrap));

    if (diffA > 0)
    {
        diffB = -diffB;
    }

    return (std::abs(diffA) < std::abs(diffB)) ? diffA : diffB;
}

//! Linearly interpolate between two values, wrapping over a given point.
/*!
* \param from The first value.
* \param to The second value.
* \param wrap The wrapping point.
* \param amount The interpolation amount.
* \return The interpolated value.
*/
template <typename T>
inline T wrapLerp(T from, T to, T wrap, float amount)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");

    T diff = wrapDifference(from, to, wrap);
    return lerp(from, from + diff, amount);
}

//! Linearly interpolate an angle taking into account periodicity.
/*!
* \param from The first angle.
* \param to The second angle.
* \param amount The interpolation amount.
* \return The interpolated value.
*/
template <typename T>
inline T angularLerp(T from, T to, float amount)
{
    static_assert(!std::numeric_limits<T>::is_exact, "Floating-point type expected");

    return wrapLerp(from, to, T(twoPi), amount);
}

}
