#include "World.h"
#include "WorldState.h"
#include "Engine.h"

World::World(WorldState* worldState)
    : worldState(worldState)
{
}

World::~World()
{
}

void World::update(sf::Time deltaTime)
{
    for (auto& system : systemOrder)
    {
        system->update(deltaTime);
    }
}

void World::advance()
{
    for (auto& system : systemOrder)
    {
        system->advance();
    }
}

void World::render(float alpha)
{
    for (auto& system : systemOrder)
    {
        system->render(alpha);
    }
}

void World::registerEntity(Entity::ID id)
{
    Entity* entity = getEntity(id);

    if (!entity)
    {
        throw std::runtime_error("Tried to register an Entity with an invalid ID!");
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