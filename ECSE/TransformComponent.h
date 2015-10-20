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
        return discretePosition ? getNextLocalPosition() : (position + deltaPosition * alpha);
    }

    //! Get the angle interpolated between its current and next value.
    /*!
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated angle.
    */
    inline float getInterpLocalAngle(float alpha) const
    {
        return discreteAngle ? getNextLocalAngle() : (angle + deltaAngle * alpha);
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
        deltaPosition = newPosition - position;
        discretePosition = discrete;
    }

    //! Set the next angle.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next angle.
    *
    * \param newAngle The angle to rotate to.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    * \param clockwise If true, the angle will be rotated to clockwise; otherwise, it will be rotated to counter-clockwise.
    */
    inline void setNextLocalAngle(float newAngle, bool discrete = false, bool clockwise = false)
    {
        deltaAngle = newAngle - angle;
        discreteAngle = discrete;
    }

    //! Set the change in position.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next position.
    *
    * \param newDeltaPosition The change in position.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    */
    inline void setDeltaPosition(const sf::Vector2f& newDeltaPosition, bool discrete = false)
    {
        deltaPosition = newDeltaPosition;
        discretePosition = discrete;
    }

    //! Set the change in angle.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next angle.
    *
    * \param newDeltaAngle The change in angle.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    */
    inline void setDeltaAngle(float newDeltaAngle, bool discrete = false)
    {
        deltaAngle = newDeltaAngle;
        discreteAngle = discrete;
    }

    //! Set the current position.
    /*!
    * \param newPosition The position to move to.
    * \param setNext If true, set the next position to this position as well.
    */
    inline void setLocalPosition(const sf::Vector2f& newPosition, bool setNext = true)
    {
        position = newPosition;
        if (setNext) setDeltaPosition(sf::Vector2f());
    }

    //! Set the current angle.
    /*!
    * \param newAngle The angle to rotate to.
    * \param setNext If true, set the next angle to this angle as well.
    */
    inline void setLocalAngle(float newAngle, bool setNext = true)
    {
        angle = newAngle;
        if (setNext) setDeltaAngle(0.f);
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
    * \return The next position.
    */
    inline sf::Vector2f getNextLocalPosition() const
    {
        return position + deltaPosition;
    }

    //! Get the next angle.
    /*!
    * \return The next angle.
    */
    inline float getNextLocalAngle() const
    {
        return angle + deltaAngle;
    }

    //! Get the change in position.
    /*!
    * \return A reference to the next position.
    */
    inline sf::Vector2f getLocalDeltaPosition() const
    {
        return deltaPosition;
    }

    //! Get the change in angle.
    /*!
    * \return The change in angle.
    */
    inline float getLocalDeltaAngle() const
    {
        return deltaAngle;
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
        position = getNextLocalPosition();
        angle = getNextLocalAngle();

        deltaPosition = sf::Vector2f();
        deltaAngle = 0.f;

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
    sf::Vector2f deltaPosition;                 //!< Change in position in pixels.
    float deltaAngle = 0.f;                     //!< Change in angle in radians.

    sf::Vector2f position;                      //!< Current position in pixels.
    float angle = 0.f;                          //!< Current angle in radians.

    bool discretePosition = false;              //!< Whether the position change in this timestep should be a discrete jump.
    bool discreteAngle = false;                 //!< Whether the angle change in this timestep should be a discrete jump.

    Entity::ID parent = Entity::invalidID;      //!< The id of the Entity to which this is parented.
    std::vector<Entity::ID> children;           //!< Entity ids parented to this component.
};

}
