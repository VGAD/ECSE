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

void World::registerEntity(Entity::ID id)
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
}

void World::destroyEntity(Entity::ID id)
{
    Entity* e = getEntity(id);
    for (const auto& pair : e->getComponents())
    {
        destroyComponent(pair.second);
    }

    EntityManager::destroyEntity(id);
}

Engine* World::getEngine() const
{
    return worldState->getEngine();
}

}
