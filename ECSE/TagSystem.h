#pragma once

#include "SetSystem.h"

namespace ECSE {

//! A System that holds information about entities' tags.
class TagSystem :
    public SetSystem
{
public:
    //! Construct the TransformSystem.
    explicit TagSystem(World* world) : SetSystem(world) {}

    //! Find all entities in the system with a given tag.
    /*!
    * \param tag The tag to check.
    * \return The set of entities with that tag.
    */
    std::set<Entity*> findWithTag(size_t tag) const;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const Entity& e) const override;
};

}
