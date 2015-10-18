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
            system->addAndRemove();
        }

        systemsAdded = true;
    }

    for (auto& system : orderedSystems)
    {
        system->update(deltaTime);
    }

    for (auto& system : orderedSystems)
    {
        system->addAndRemove();
    }

    for (auto eId : toDestroy)
    {
        Entity* e = EntityManager::getEntity(eId);

        // Move components into a set so duplicate components don't get destroyed twice
        // (which happens if a component was added polymorphically)
        std::set<Component*> componentsToDestroy;
        for (const auto& pair : e->getComponents())
        {
            componentsToDestroy.insert(pair.second);
        }

        for (auto* component : componentsToDestroy)
        {
            destroyComponent(component);
        }

        EntityManager::destroyEntity(eId);
    }
    toDestroy.clear();
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

Entity* World::getEntity(Entity::ID id)
{
    if (toDestroy.empty()) return EntityManager::getEntity(id);
    if (toDestroy.find(id) != toDestroy.end()) return nullptr;
        
    return EntityManager::getEntity(id);
}

const std::vector<Entity*> World::getEntities() const
{
    auto entities = EntityManager::getEntities();
    if (toDestroy.empty()) return entities;

    auto validEntities = EntityManager::getEntities();

    for (auto ent : entities)
    {
        if (toDestroy.find(ent->getID()) != toDestroy.end()) continue;

        validEntities.push_back(ent);
    }

    return validEntities;
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

    // Disable all components
    for (const auto& pair : e->getComponents())
    {
        pair.second->enabled = false;
    }

    for (auto system : orderedSystems)
    {
        if (system->hasEntity(*e))
        {
            system->markToRemove(*e);
        }
    }

    toDestroy.insert(id);
}

int World::getSystemCount()
{
    return static_cast<int>(systems.size());
}

Engine* World::getEngine() const
{
    return worldState->getEngine();
}

}
