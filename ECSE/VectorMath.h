//! \file VectorMath.h Contains functions for doing math on sf::Vector2f.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace ECSE
{

//! Rotate a vector 90 degrees counter-clockwise.
/*!
* This is faster than using rotate(v, pi * 0.5).
* \param v The vector.
* \return The vector rotated 90 degrees counter-clockwise.
*/
inline sf::Vector2f& rotate90(sf::Vector2f& v)
{
    float oldX = v.x;
    v.x = -v.y;
    v.y = oldX;

    return v;
}

//! Rotate a vector by an angle.
/*!
* \param v The vector.
* \param angle The angle by which to rotate it.
* \return A reference to the vector.
*/
inline sf::Vector2f& rotate(sf::Vector2f& v, float angle)
{
    sf::Vector2f oldV(v);

    v.x = oldV.x * cos(angle) - oldV.y * sin(angle);
    v.y = oldV.x * sin(angle) + oldV.y * cos(angle);

    return v;
}

//! Get a vector's angle of rotation.
/*!
* \param v The vector.
* \return The vector's angle of rotation.
*/
inline float getHeading(const sf::Vector2f& v)
{
    return atan2(v.y, v.x);
}

//! Get a vector's square magnitude.
/*!
* \param v The vector.
* \return The vector's square magnitude.
*/
inline float getSqrMagnitude(const sf::Vector2f& v)
{
    return v.x * v.x + v.y * v.y;
}

//! Get a vector's magnitude.
/*!
* \param v The vector.
* \return The vector's magnitude.
*/
inline float getMagnitude(const sf::Vector2f& v)
{
    return sqrt(getSqrMagnitude(v));
}

//! Get the dot product of two vectors.
/*!
* \param v1 The first vector.
* \param v2 The second vector.
* \return The dot product.
*/
inline float getDotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

//! Get the 2D cross product of two vectors.
/*!
* This returns the magnitude of the 3D cross product where Z=0 for both vectors.

* \param v1 The first vector.
* \param v2 The second vector.
* \return The cross product.
*/
inline float get2DCrossProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

//! Set a vector's angle of rotation, maintaining its magnitude.
/*!
* \param v The vector.
* \param angle The new angle.
*/
inline void setHeading(sf::Vector2f& v, float angle)
{
    v.x = getMagnitude(v);
    v.y = 0;
    rotate(v, angle);
}

//! Normalize a vector.
/*!
* \param v The vector.
* \return A reference to the vector.
*/
inline sf::Vector2f& normalize(sf::Vector2f& v)
{
    auto magnitude = getMagnitude(v);

    if (magnitude > 0.f) v /= getMagnitude(v);
    
    return v;
}

//! Set a vector's magnitude, maintaining its angle of rotation.
/*!
* \param v The vector.
* \param magnitude The new magnitude.
* \return A reference to the vector.
*/
inline sf::Vector2f& setMagnitude(sf::Vector2f& v, float magnitude)
{
    normalize(v);
    v *= magnitude;

    return v;
}

//! Project a vector onto another vector.
/*!
* \param v1 The first vector.
* \param v2 The vector onto which to project it.
* \return A reference to v1.
*/
inline sf::Vector2f& project(sf::Vector2f& v1, const sf::Vector2f& v2)
{
    v1 = (getDotProduct(v1, v2) / getDotProduct(v2, v2)) * v2;

    return v1;
}

//! Reject a vector onto another vector.
/*!
* This is also known as the orthogonal projection.
* \param v1 The first vector.
* \param v2 The vector onto which to reject it.
* \return A reference v1.
*/
inline sf::Vector2f& reject(sf::Vector2f& v1, const sf::Vector2f& v2)
{
    auto projection(v1);
    project(projection, v2);

    v1 -= projection;

    return v1;
}

}
