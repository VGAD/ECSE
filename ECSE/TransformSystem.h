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
    sf::Vector2f getPosition(const Entity& e) const;

    //! Get the current global angle of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's current global angle.
    */
    float getAngle(const Entity& e) const;

    //! Get the next global position of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's next global position.
    */
    sf::Vector2f getNextPosition(const Entity& e) const;

    //! Get the next global angle of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's next global angle.
    */
    float getNextAngle(const Entity& e) const;

    //! Get an Entity's global position interpolated between its current and next value.
    /*!
    * \param e The Entity.
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated position.
    */
    sf::Vector2f interpolatePosition(const Entity& e, float alpha) const;

    //! Get an Entity's global angle interpolated between its current and next value.
    /*!
    * \param e The Entity.
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated angle.
    */
    float interpolateAngle(const Entity& e, float alpha) const;

    //! Set an entity's parent.
    /*!
    * The child entity's position and angle will now be relative to the parent's
    * position and angle. This function will automatically convert the child's current
    * position to be accurate relative to the parent.
    *
    * Both entities must have TransformComponents.
    *
    * \param child The child entity.
    * \param parent The entity which will become its parent.
    */
    void parentEntity(const Entity& child, const Entity& parent) const;

    //! Remove an entity's parent.
    /*!
    * The entity's position and angle will now be global rather than relative to the parent's
    * position and angle. This function will automatically convert the child's current
    * position to be accurate relative to the world.
    *
    * \param child The entity to unparent. Must have a TransformComponent.
    */
    void unparentEntity(const Entity& child) const;

private:
    //! Convert a position and angle relative to a parent anchor position and angle.
    /*!
    * \param pos The position.
    * \param angle The angle.
    * \param anchorPos The anchor position.
    * \param anchorAngle The anchor angle.
    */
    static void convertRelativeToAnchor(sf::Vector2f& pos, float& angle, sf::Vector2f anchorPos, float anchorAngle);

    //! Get the transform of an entity's parent.
    /*!
    * This will throw an exception if the parent does not exist.
    *
    * \param e The child's TransformComponent.
    */
    TransformComponent* getParentTransform(const TransformComponent& trans) const;
};

}
