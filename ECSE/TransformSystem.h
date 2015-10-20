#pragma once

#include "SetSystem.h"
#include "TransformComponent.h"

namespace ECSE
{

//! A System that handles updating TransformComponents as well as determining an Entity's global (not relative/local) transform data.
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

    //! Mark an Entity to be removed from the System.
    void markToRemove(Entity& e) override;

    //! Get the current global position of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's current global position.
    */
    sf::Vector2f getGlobalPosition(const Entity& e) const;

    //! Get the current global angle of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's current global angle.
    */
    float getGlobalAngle(const Entity& e) const;

    //! Get the next global position of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's next global position.
    */
    sf::Vector2f getNextGlobalPosition(const Entity& e) const;

    //! Get the next global angle of a given Entity.
    /*!
    * \param e The Entity.
    * \return The Entity's next global angle.
    */
    float getNextGlobalAngle(const Entity& e) const;

    //! Get an Entity's global position interpolated between its current and next value.
    /*!
    * \param e The Entity.
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated position.
    */
    sf::Vector2f getInterpGlobalPosition(const Entity& e, float alpha) const;

    //! Get an Entity's global angle interpolated between its current and next value.
    /*!
    * \param e The Entity.
    * \param alpha The amount of interpolation (0.0 - 1.0).
    * \return The interpolated angle.
    */
    float getInterpGlobalAngle(const Entity& e, float alpha) const;

    //! Set the next global position of a given Entity.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next position.
    *
    * \param e The Entity.
    * \param newPosition The position to move to.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    */
    void setNextGlobalPosition(const Entity& e, sf::Vector2f newPosition, bool discrete = false) const;

    //! Set the next global angle of a given Entity.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next position.
    *
    * \param e The Entity.
    * \param newAngle The angle to rotate to.
    * \param discrete Whether the move should be a discrete jump. If false, the movement is linearly interpolated.
    * \param clockwise If true, the angle will be rotated to clockwise; otherwise, it will be rotated to counter-clockwise.
    */
    void setNextGlobalAngle(const Entity& e, float newAngle, bool discrete = false, bool clockwise = false) const;

    //! Set the current global position of a given Entity.
    /*!
    * \param e The Entity.
    * \param newPosition The position to move to.
    * \param setNext If true, set the next position to this position as well.
    */
    void setGlobalPosition(const Entity& e, sf::Vector2f newPosition, bool setNext = true) const;

    //! Set the current global angle of a given Entity.
    /*!
    * Only the last call to this function in a given timestep will actually affect the next position.
    *
    * \param e The Entity.
    * \param newAngle The angle to rotate to.
    * \param setNext If true, set the next angle to this angle as well.
    */
    void setGlobalAngle(const Entity& e, float newAngle, bool setNext = true) const;

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
    //! Convert an angle relative to a parent anchor position and angle.
    /*!
    * \param angle The angle.
    * \param anchorPos The anchor position.
    * \param anchorAngle The anchor angle.
    */
    static void convertAngleRelativeToAnchor(float& angle, float anchorAngle);

    //! Convert a position relative to a parent anchor position and angle.
    /*!
    * \param position The position.
    * \param anchorPos The anchor position.
    * \param anchorAngle The anchor angle.
    */
    static void convertPositionRelativeToAnchor(sf::Vector2f& position, const sf::Vector2f& anchorPos, float anchorAngle);

    //! Get the transform of an entity's parent.
    /*!
    * This will throw an exception if the parent does not exist.
    *
    * \param e The child's TransformComponent.
    */
    TransformComponent* getParentTransform(const TransformComponent& trans) const;
};

}
