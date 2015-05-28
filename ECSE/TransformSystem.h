#pragma once

#include "SetSystem.h"
#include "TransformComponent.h"

namespace ECSE
{

//! A System that handles updating TransformComponents as well as determining an Entity's global (not relative/local) transform data.
// TODO: add a Component allowing Entities to attach to a parent.
class TransformSystem :
    public SetSystem
{
public:
    //! Construct the TransformSystem.
    explicit TransformSystem(World* world) : SetSystem(world) {}

    //! Called on an advance step.
    /*!
    * Advances TransformComponents to their next states.
    */
    void advance() override;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const Entity& e) const override;

    //! Get the current global position of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's current global position.
    */
    inline sf::Vector2f getPosition(const Entity& e) const
    {
        return e.getComponent<TransformComponent>()->getPosition();
    }

    //! Get the current global angle of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's current global angle.
    */
    inline float getAngle(const Entity& e) const
    {
        return e.getComponent<TransformComponent>()->getAngle();
    }

    //! Get the next global position of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's next global position.
    */
    inline sf::Vector2f getNextPosition(const Entity& e) const
    {
        return e.getComponent<TransformComponent>()->getNextPosition();
    }

    //! Get the next global angle of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's next global angle.
    */
    inline float getNextAngle(const Entity& e) const
    {
        return e.getComponent<TransformComponent>()->getNextAngle();
    }

    //! Get an Entity's global position interpolated between its current and next value.
    /*!
    * \param e The Entity.
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated position.
    */
    inline sf::Vector2f interpolatePosition(const Entity& e, float alpha) const
    {
        return e.getComponent<TransformComponent>()->interpolatePosition(alpha);
    }

    //! Get an Entity's global angle interpolated between its current and next value.
    /*!
    * \param e The Entity.
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated angle.
    */
    inline float interpolateAngle(const Entity& e, float alpha) const
    {
        return e.getComponent<TransformComponent>()->interpolateAngle(alpha);
    }
};

}
