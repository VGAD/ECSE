#include "EntityManager.h"

namespace ECSE
{

Entity::ID EntityManager::createEntity()
{
    Entity::ID newID = (++nextID) % getMaxIDCount();
    if (newID == Entity::invalidID)
    {
        ++newID;
    }

    if (idMap[newID])
    {
        bool found = false;

        // This ID is taken; try to find a new one
        for (Entity::ID i = Entity::invalidID + 1; i < getMaxIDCount(); ++i)
        {
            if (!idMap[i])
            {
                newID = i;
                nextID = i + 1;
                found = true;
                break;
            }
        }

        if (!found)
        {
            throw std::runtime_error("Out of Entity IDs!");
        }
    }

    Entity* e = entityPool.construct();
    e->id = newID;
    idMap[newID] = e;
    entities.push_back(e);

    return newID;
}

Entity* EntityManager::getEntity(Entity::ID id)
{
    if (idMap.find(id) == idMap.end())
    {
        return nullptr;
    }

    return idMap[id];
}

void EntityManager::destroyEntity(Entity::ID id)
{
    Entity* e = getEntity(id);

    if (!e)
    {
        throw std::runtime_error("Tried to remove entity with ID #" + std::to_string(id) + " which does not exist!");
    }

    idMap[id] = nullptr;
    entities.erase(find(entities.begin(), entities.end(), e));
    entityPool.destroy(e);
}

void EntityManager::destroyEntity(Entity* entity)
{
    destroyEntity(entity->id);
}

}
