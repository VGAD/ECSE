#include "World.h"
#include "WorldState.h"
#include "Engine.h"

namespace ECSE
{

World::World(WorldState* worldState)
    : worldState(worldState)
{
}

World::~World()
{
}

void World::update(sf::Time deltaTime)
{
    // Notify systems they've been added on first update
    if (!systemsAdded)
    {
        for (auto& system : orderedSystems)
        {
            system->added();
        }

        systemsAdded = true;
    }

    for (auto& system : orderedSystems)
    {
        system->update(deltaTime);
    }
}

void World::advance()
{
    for (auto& system : orderedSystems)
    {
        system->advance();
    }
}

void World::render(float alpha, sf::RenderTarget& renderTarget)
{
    for (auto& system : orderedSystems)
    {
        system->render(alpha, renderTarget);
    }
}

Entity* World::registerEntity(Entity::ID id)
{
    Entity* entity = getEntity(id);

    if (entity->registered)
    {
        std::stringstream ss;
        ss << "Entity already registered (id " << id << ")";
        throw std::runtime_error(ss.str());
    }

    if (!entity)
    {
        std::stringstream ss;
        ss << "Tried to register an Entity with an invalid ID (" << id << ")";
        throw std::runtime_error(ss.str());
    }

    for (auto& pair : systems)
    {
        pair.second->inspectEntity(*entity);
    }

    entity->registered = true;

    return entity;
}

void World::destroyEntity(Entity::ID id)
{
    Entity* e = getEntity(id);

    if (!e)
    {
        std::stringstream ss;
        ss << "Tried to destroy an Entity with an invalid ID (" << id << ")";
        throw std::runtime_error(ss.str());
    }

    for (const auto& pair : e->getComponents())
    {
        destroyComponent(pair.second);
    }

    for (auto system : orderedSystems)
    {
        if (system->hasEntity(*e))
        {
            system->markToRemove(*e);
        }
    }

    EntityManager::destroyEntity(id);
}

Engine* World::getEngine() const
{
    return worldState->getEngine();
}

}
