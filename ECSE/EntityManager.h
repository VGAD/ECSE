#pragma once

#include <unordered_map>
#include <boost\pool\object_pool.hpp>
#include "Entity.h"

namespace ECSE
{

//! Handles allocation and deallocation of Entities, as well as distribution of IDs.
/*!
* This class allows Entities to remain more or less tightly-packed. Ownership of the allocated
* Entities will always remain within the EntityManager.
*/
class EntityManager
{
public:
    //! Destroy the EntityManager.
    virtual ~EntityManager() {}

    //! Create a new Entity.
    /*!
    * \return The ID of the new Entity.
    */
    virtual Entity::ID createEntity();

    //! Get a pointer to an Entity by its ID.
    /*!
    * Note that Entity::invalidID (0) is never a valid ID.
    * 
    * \param id The ID of the entity to be removed.
    * \return The Entity with this ID. If it's not in the EntityManager, nullptr is returned.
    */
    virtual Entity* getEntity(Entity::ID id);

    //! Destroy an Entity, removing it from the simulation.
    /*!
    * Note that this will NOT delete the Entity's Components, as this job belongs
    * to the ComponentManager.
    * 
    * \param id The ID of the entity to be removed.
    */
    virtual void destroyEntity(Entity::ID id);

    //! Destroy an Entity, removing it from the simulation.
    /*!
    * \param entity A pointer to the Entity to remove.
    */
    virtual void destroyEntity(Entity* entity);

    //! Get a vector of all the Entities.
    /*!
    * \return A vector of all Entities.
    */
    virtual inline const std::vector<Entity*>& getEntities() const
    {
        return entities;
    }

    //! Get the maximum number of IDs, including Entity::invalidID.
    /*!
    * \return The maximum number of IDs.
    */
    virtual Entity::ID getMaxIDCount() const
    {
        return std::numeric_limits<Entity::ID>::max();
    }

private:
    //! The next entity ID to use.
    Entity::ID nextID = Entity::invalidID + 1;
    
    //! Map from Entity ID to a pointer to the actual Entity.
    std::unordered_map<Entity::ID, Entity*> idMap;

    //! Vector of all entities.
    /*!
    * This is redundant, but saves us from having to reconstruct it every time we
    * want all entities without the IDs attached as pairs.
    */
    std::vector<Entity*> entities;

    //! Pool from which Entities are allocated.
    boost::object_pool<Entity> entityPool;
};


}
