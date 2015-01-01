#pragma once

#include "SetSystem.h"
#include "TransformComponent.h"

class TransformSystem :
    public SetSystem
{
public:
    // Construct the TransformSystem.
    TransformSystem(World* world) : SetSystem(world) {}

    //! Called on an advance step.
    /*!
    * Advances TransformComponents to their next states.
    */
    void advance();

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const Entity& e) const;
};
