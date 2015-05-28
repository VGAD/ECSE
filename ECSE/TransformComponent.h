#pragma once

#include <SFML/System/Vector2.hpp>
#include "Component.h"
#include "Common.h"

namespace ECSE
{

//! A Component which stores local transform data (e.g. local position and angle).
class TransformComponent : public Component
{
public:
    //! Get the position interpolated between its current and next value.
    /*!
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated position.
    */
    inline sf::Vector2f interpolatePosition(float alpha) const
    {
        return discretePosition ? nextPosition : lerp(position, nextPosition, alpha);
    }

    //! Get the angle interpolated between its current and next value.
    /*!
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated angle.
    */
    inline float interpolateAngle(float alpha) const
    {
        return discreteAngle ? nextAngle : angularLerp(angle, nextAngle, alpha);
    }

    //! Set the current values to their next values and sets movement back to linear for the new timestep.
    inline void advance()
    {
        position = nextPosition;
        angle = nextAngle;

        discretePosition = false;
        discreteAngle = false;
    }

    //! Set the next position.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next position.
    *
    * \param newPosition The position to move to.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    */
    inline void setNextPosition(const sf::Vector2f& newPosition, bool discrete = false)
    {
        nextPosition = newPosition;
        discretePosition = discrete;
    }

    //! Set the next angle.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next position.
    *
    * \param newAngle The angle to rotate to.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    */
    inline void setNextAngle(float newAngle, bool discrete = false)
    {
        nextAngle = newAngle;
        discreteAngle = discrete;
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

    //! Get the next position.
    /*!
    * \return A reference to the next position.
    */
    inline const sf::Vector2f& getNextPosition() const
    {
        return nextPosition;
    }

    //! Get the next angle.
    /*!
    * \return The next angle.
    */
    inline float getNextAngle() const
    {
        return nextAngle;
    }

    //! Check if the position change is discrete.
    /*!
    * \return Whether the position change is discrete.
    */
    inline bool getDiscretePosition() const
    {
        return discretePosition;
    }

    //! Check if the angle change is discrete.
    /*!
    * \return Whether the angle change is discrete.
    */
    inline bool getDiscreteAngle() const
    {
        return discreteAngle;
    }

private:
    sf::Vector2f nextPosition = sf::Vector2f(); //!< Next position in pixels.
    float nextAngle = 0.f;                      //!< Next angle in radians.

    sf::Vector2f position = sf::Vector2f();     //!< Current position in pixels.
    float angle = 0.f;                          //!< Current angle in radians.

    bool discretePosition = false;              //!< Whether the position change in this timestep should be a discrete jump.
    bool discreteAngle = false;                 //!< Whether the angle change in this timestep should be a discrete jump.
};

}
