#pragma once

#include <SFML/System/Vector2.hpp>
#include "Component.h"
#include "Common.h"

namespace ECSE
{

//! A Component which stores local transform data (e.g. local position and angle).
class TransformComponent : public Component
{
    friend class TransformSystem;

public:
    //! Get the position interpolated between its current and next value.
    /*!
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated position.
    */
    inline sf::Vector2f getInterpLocalPosition(float alpha) const
    {
        return discretePosition ? nextPosition : lerp(position, nextPosition, alpha);
    }

    //! Get the angle interpolated between its current and next value.
    /*!
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated angle.
    */
    inline float getInterpLocalAngle(float alpha) const
    {
        return discreteAngle ? nextAngle : angularLerp(angle, nextAngle, alpha);
    }

    //! Set the next position.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next position.
    *
    * \param newPosition The position to move to.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    */
    inline void setNextLocalPosition(const sf::Vector2f& newPosition, bool discrete = false)
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
    inline void setNextLocalAngle(float newAngle, bool discrete = false)
    {
        nextAngle = newAngle;
        discreteAngle = discrete;
    }

    //! Get the current position.
    /*!
    * \return A reference to the current position.
    */
    inline const sf::Vector2f& getLocalPosition() const
    {
        return position;
    }

    //! Get the current angle.
    /*!
    * \return The current angle.
    */
    inline float getLocalAngle() const
    {
        return angle;
    }

    //! Get the next position.
    /*!
    * \return A reference to the next position.
    */
    inline const sf::Vector2f& getNextLocalPosition() const
    {
        return nextPosition;
    }

    //! Get the next angle.
    /*!
    * \return The next angle.
    */
    inline float getNextLocalAngle() const
    {
        return nextAngle;
    }

    //! Check if the position change is discrete.
    /*!
    * \return Whether the position change is discrete.
    */
    inline bool isPositionDiscrete() const
    {
        return discretePosition;
    }

    //! Check if the angle change is discrete.
    /*!
    * \return Whether the angle change is discrete.
    */
    inline bool isAngleDiscrete() const
    {
        return discreteAngle;
    }

    //! Set the current values to their next values and sets movement back to linear for the new timestep.
    inline void advance()
    {
        position = nextPosition;
        angle = nextAngle;

        discretePosition = false;
        discreteAngle = false;
    }

    //! Get the children parented to this component.
    inline const std::vector<Entity::ID> getChildren() const
    {
        return children;
    }

    bool destroyWithParent = true;              //!< If true, this will be removed when the parent is removed from TransformSystem. If false, it will just be unparented.

private:
    sf::Vector2f nextPosition = sf::Vector2f(); //!< Next position in pixels.
    float nextAngle = 0.f;                      //!< Next angle in radians.

    sf::Vector2f position = sf::Vector2f();     //!< Current position in pixels.
    float angle = 0.f;                          //!< Current angle in radians.

    bool discretePosition = false;              //!< Whether the position change in this timestep should be a discrete jump.
    bool discreteAngle = false;                 //!< Whether the angle change in this timestep should be a discrete jump.

    Entity::ID parent = Entity::invalidID;      //!< The id of the Entity to which this is parented.
    std::vector<Entity::ID> children;           //!< Entity ids parented to this component.
};

}
