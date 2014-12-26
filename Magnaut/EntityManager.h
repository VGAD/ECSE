#pragma once

#include <cstdint>
#include <set>
#include <unordered_map>
#include <boost\pool\object_pool.hpp>
#include "Entity.h"

//! Handles allocation and deallocation of Entities, as well as distribution of IDs.
/*!
* This class allows Entities to remain more or less tightly-packed. Ownership of the allocated
* Entities will always remain within the EntityManager.
*/
class EntityManager
{
public:
    //! Construct the EntityManager.
    EntityManager();

    //! Destroy the EntityManager.
    ~EntityManager();

    //! Create a new Entity.
    /*!
    * \return The ID of the new Entity.
    */
    Entity::ID createEntity();

    //! Get a pointer to an Entity by its ID.
    /*!
    * Note that Entity::invalidID (0) is never a valid ID.
    * 
    * \param id The ID of the entity to be removed.
    * \return The Entity with this ID. If it's not in the EntityManager, nullptr is returned.
    */
    Entity* getEntity(Entity::ID id);

    //! Destroy an Entity, removing it from the simulation.
    /*!
    * Note that this will NOT delete the Entity's Components, as this job belongs to the ComponentManager.
    * 
    * \param id The ID of the entity to be removed.
    */
    void destroyEntity(Entity::ID id);

private:
    //! The next entity ID to use.
    Entity::ID nextID = Entity::invalidID + 1;
    
    //! Map from Entity ID to a pointer to the actual Entity.
    std::unordered_map<Entity::ID, Entity*> idMap;

    //! Pool from which Entities are allocated.
    boost::object_pool<Entity> entityPool;
};
