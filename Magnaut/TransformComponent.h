#pragma once

#include <SFML/System/Vector2.hpp>
#include "Component.h"
#include "Common.h"

//! A Component which stores local transform data (e.g. local position and angle).
class TransformComponent : public Component
{
public:
    sf::Vector2f nextPosition;  //!< Next position in pixels.
    float nextAngle;            //!< Next angle in radians.

    //! Get the position interpolated between its current and next value.
    /*!
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated position.
    */
    inline sf::Vector2f interpolatePosition(float alpha) const
    {
        return lerp(position, nextPosition, alpha);
    }

    //! Get the angle interpolate between its current and next value.
    /*!
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated angle.
    */
    inline float interpolateAngle(float alpha) const
    {
        return lerp(angle, nextAngle, alpha);
    }

    //! Set the current values to their next values.
    inline void advance()
    {
        position = nextPosition;
        angle = nextAngle;
    }

    //! Get the current position.
    /*!
    * \return A reference to the current position.
    */
    inline const sf::Vector2f& getPosition() const
    {
        return position;
    }

    //! Get the current angle.
    /*!
    * \return The current angle.
    */
    inline float getAngle() const
    {
        return angle;
    }

private:
    sf::Vector2f position;  //!< Current position in pixels.
    float angle;            //!< Current angle in radians.
};

