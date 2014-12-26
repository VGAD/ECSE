#pragma once

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "ComponentManager.h"
#include "EntityManager.h"

//! Holds System and executes their functions appropriately.
/*!
* Also responsible for allocating and deallocating both Entities and Components.
*/
class World : public EntityManager, public ComponentManager
{
public:
    //! Create an empty World.
    World();

    //! Destroy the World and its contents.
    virtual ~World();

    //! Perform the update step for all contents.
    /*!
    * \param deltaTime The time elapsed in this update.
    */
    virtual void update(double deltaTime);

    //! Destroy an Entity, removing it from the simulation.
    /*!
    * Automatically removes the Entity's Components.
    * 
    * \param id The ID of the entity to be removed.
    */
    void destroyEntity(Entity::ID id);

private:
};
