#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <cmath>

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

//! Set a vector's angle of rotation, maintaining its magnitude.
/*!
* \param v The vector.
* \param angle The new angle.
* \return The vector's new angle.
*/
inline float setHeading(sf::Vector2f& v, float angle)
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
    v /= getMagnitude(v);
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
}